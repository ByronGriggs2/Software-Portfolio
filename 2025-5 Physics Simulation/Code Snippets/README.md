# Codebase Structure
Most of the functionality is in the form of classes seperated into header and .cpp files. There is also a .pch file that greatly speeds up compilation, a .fx file for vertex and pixel shaders, and a static library for most of the vectorised physics maths.
![Alt text](code_structure)
This project exhibits much better object oriented design than the 2.5D platformer, and encapsulates each thread to minimise the potential for race conditions. </br>
Something worth noting is the heavy use of singletons that seemingly damages encapsulation. At the time I was not aware that singletons are primarily used 
as global variables, and I instead used them to emphasise that instantiating more than 1 of each would fundamentally undermine the purpose of the class.

# Game objects
The game object class (here called GameObject like in Unity rather than CompleteObject as in the 2.5D platformer) is significantly more efficient and advanced.
Every game object is now a primitive shape, such as a sphere or rectangular prism, and is simulated with gravity, kinematics, and collisions with other objects of any type.
```C++
class GameObject {
	ObjectGeometry _geometry;
	std::unique_ptr<PhysicsObject> _physics;
	helpers::staticPtrPreset _ptrPreset;
	helpers::peerColor _myColor;
	CComPtr<ID3D11Buffer> _vertexBuffer;
	CComPtr<ID3D11Buffer> _indexBuffer;

public:
	// Constructor with specified primitive
	GameObject(const helpers::ObjectPresets& preset);
	// Gang of 5
	GameObject() = delete;
	GameObject(const GameObject& other);
	GameObject(GameObject&& other) noexcept;
	GameObject& operator=(const GameObject& other);
	GameObject& operator=(GameObject&& other) noexcept;

	bool testIntersect(const GameObject& other) const;
	void updateEuler(float deltaT);
	void updateSemiEuler(float deltaT);
	void updateRungeKutta(float deltaT);
	void setBuffers(const CComPtr<ID3D11Device>&);
	//...
}
```
## Improved mesh creation
This project does away with the .obj files entirely, and instead creats the vertex and index lists for each GameObject whenever a scene is loaded. </br>
The best example to showcase this is the parameterised constructor for GameObject, which sets its vertex and index lists (_geometry) to a value given 
by GeometryGenerator. The creation of vertex and index buffers is done later, but typically right after the construction.
```C++
GameObject::GameObject(const ObjectPresets& preset) :
	_geometry(),
	_physics(nullptr),
	_ptrPreset(preset._ptrs),
	_myColor(undef),
	_vertexBuffer(nullptr),
	_indexBuffer(nullptr)
{
	switch (preset._type) {
	case capsule:
	{
		const float height = preset._initData._capsule._height;
		const float radius = preset._initData._capsule._radius;
		const int tesselation = preset._initData._capsule._tesselation;
		_physics = std::make_unique<Capsule>(preset._params, height, radius);
		_geometry = GeometryGenerator::createCapsule(height, radius, tesselation);
	}
		break;
	case hollowCube:
	{
		const float sideLength = preset._initData._hollowCube._sideLength;
		_physics = std::make_unique<HollowCube>(preset._params, sideLength);
		_geometry = GeometryGenerator::createRectangularPrism(sideLength, sideLength, sideLength);
	}
		break;
	case plane:
	{
		_physics = std::make_unique<Plane>(preset._params);
		_geometry = GeometryGenerator::createPlane(1);
	}
		break;
	case sphere:
	{
		const float radius = preset._initData._sphere._radius;
		const int tesselation = preset._initData._sphere._tesselation;
		_physics = std::make_unique<Sphere>(preset._params, radius);
		_geometry = GeometryGenerator::createSphere(radius, tesselation);
	}
		break;
#ifdef _DEBUG
	case physObj:
	{ throw AbstractClassException(); }
	break;
	default:
	{ throw NotImplementedException(); }
	break;
#endif
	}
}
```


## GameObject physics
The physical behaviour of a GameObject is defined by its pointer to a PhysicsObject, which can point to any inheritor of PhysicsObject (sphere, cube, etc.).
```C++
class PhysicsObject
{
	///////////////////////////////////////
	// Member Variables
private:
	DirectX::XMVECTOR _position = DirectX::XMVectorZero();
	DirectX::XMVECTOR _rotation = DirectX::XMQuaternionIdentity();
	// _prevPosition and _prevRotation are needed to check intersection with the hollow cube.
	DirectX::XMVECTOR _prevPosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR _prevRotation = DirectX::XMVectorZero();
	DirectX::XMVECTOR _linearVelocity = DirectX::XMVectorZero();
	// vector in direction of rotation axis
	DirectX::XMVECTOR _angularVelocity = DirectX::XMVectorZero();
	DirectX::XMVECTOR _frameForce = DirectX::XMVectorZero();
	// If the torque were pointing directly at a viewer, the object would rotate counterclockwise from that perspective.
	DirectX::XMVECTOR _frameTorque = DirectX::XMVectorZero();

	float _mass = 1.0f;
	float _inverseMass = 1.0f;
	float _elasticity = 1.0f;

protected:
	bool _fixed = false;
	DirectX::XMMATRIX _inertia = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX _inverseInertia = DirectX::XMMatrixIdentity();
	bool _simpleInertia = false;
	shapeHelpers::ObjType _myType = shapeHelpers::physObj;

	/////////////////////////////////////
	// Construction
public:
	// These are member variables that should only be directly modified during construction. You don't want the mass of an object changing between frames.
	struct constructParams {
		DirectX::XMVECTOR _position = DirectX::XMVectorZero();
		DirectX::XMVECTOR _linearVelocity = DirectX::XMVectorZero();
		DirectX::XMVECTOR _angularVelocity = DirectX::XMVectorZero();
		DirectX::XMVECTOR _rotation = DirectX::XMQuaternionIdentity();
		float _mass = 1.0f;
		float _elasticity = 1.0f;
		bool _fixed = false;
	};
	PhysicsObject(const constructParams& params) : 
		_position(params._position), 
		_prevPosition(params._position), 
		_linearVelocity(params._linearVelocity), 
		_angularVelocity(params._angularVelocity),
		_rotation(params._rotation), 
		_prevRotation(params._rotation),
		_mass(params._mass), 
		_inverseMass(1.0f / params._mass), 
		_elasticity(params._elasticity), 
		_fixed(params._fixed) {
	}
	PhysicsObject() = default;
	virtual ~PhysicsObject() = default;

	////////////////////////////////////
	// Private Methods
private:
	// For internal use and testing
	friend class PhysicsObjectTestHelper;
	void kinematics(float timeStep);
	void resolveForceAndTorque(float deltaT);
	// ...
}
```
Collision detection is done by testing whether each pair of objects in the scene intersects on each frame. To implement this, each unique pair of inheritors of PhysicsObject must have its own intersection function. <br />
Here is the intersection function between two PhysicsObject inheritors of type Capsule.
```C++
bool Intersect::testIntersect(const Capsule& c1, const Capsule& c2) {
	const float c1Radius = c1.getRadius();
	const LineSegment c1Axis = c1.getAxis();
	const XMVECTOR c1P1 = c1Axis.getPosition();
	const XMVECTOR c1P2 = c1Axis.getP2();
	const float c2Radius = c2.getRadius();
	const LineSegment c2Axis = c2.getAxis();
	const XMVECTOR c2P1 = c2Axis.getPosition();
	const XMVECTOR c2P2 = c2Axis.getPosition();

	const XMVECTOR closestC1P1 = c2Axis.closestPointOnLineSegment(c1P1);
	if (distance(closestC1P1, c1P1) <= c1Radius + c2Radius)
		return true;
	const XMVECTOR closestC1P2 = c2Axis.closestPointOnLineSegment(c1P2);
	if (distance(closestC1P2, c1P2) <= c1Radius + c2Radius)
		return true;
	return false;
}
```
# Multithreading
The simulation runs in 4 threads: main, render, physics, and network. The main thread handles communication with Windows, the render thread
handles user inputs and uses transformation matrices, vertex and index buffers, and DirectX11 3D to draw each object. The physics thread moves objects,
thereby chaging their transformation matrices, as well as detecting collisions and applying forces to objects. The network thread unfortunately does nothing of note, due to time constraints. </br>
The 4 threads run asynchronously and do not pause execution to wait for another thread unless accessing shared memory.
# Shared memory
SharedMemory, ostensibly owned by the main thread, contains all data that multiple threads can access. Thread safety is enforced by placing the entire struct in a mutex, and blocking is reduced by 
only allowing each thread to acquire the mutex twice per runtime loop- once to read, and once to write. </br>
The two main members of SharedMemory are 
_ImGuiState and _globalObjects. _ImGuiState is really just the current settings of the simulator, such as simulation timestep or integration type.
_globalObjects is a structure containing each object in the scene, but uses the significantly lighter weight GlobalObjectStruct rather than 
GameObject, which notably does not include the vertex and index lists/buffers, and is much more efficient to regularly copy and move around.

# Render loop
Here you can see the main runtime loop of the render thread, which uses vertex/index buffers created during initialisation and transformation
matrices calculated by the physics thread along with DirectX11 3D to render each object in the scene. </br>
Note that getInputs() and endOfFrame() are the "read" and "write" steps mentioned earlier.
```C++
void D3DFramework::render() {
	static auto ImGui = ImGuiHandler::getInstance();

	QueryPerformanceCounter(&_frameStart);
	// ImGui start of loop
	ImGui->renderStart();

	getInputs();
	if (_renderReset) {
		ImGui->setBackgroundColor(_defaultColor);
		ImGui->resetCamera();
		_renderResetThisLoop = true;
	}
	ImGui->calculateActualSimulationFrequency(_actualSimulationSpeed);	
	ImGui->calculateActualNetworkFrequency(_actualNetworkSpeed);
		
	// Calculate camera and light position (could be variable later on)
	const XMVECTOR Eye = XMVector3Transform(_cameraPosition, ImGui->getCameraOrientation());
	const XMVECTOR At = XMVectorSet(0, 0, 0.0f, 0.0f);
	const XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_View = XMMatrixLookAtLH(Eye, At, Up);
	const XMVECTOR& lightPos = _lightPosition;

	// Clear the back buffer
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ImGui->getBackgroundColor());

	// Set depth testing
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView_default, D3D11_CLEAR_DEPTH, 1.0f, 0);
	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView.p, _pDepthStencilView_default);

	// Render objects
	for (const auto& obj : _objectsToDraw) {
		drawSingle(lightPos, Eye, obj);
	}
	{
		// ImGui render and end of loop. Returns true iff scene is reset or switched
		auto currentlyRenderedObjects = calculateRenderedObjects();
		_resetPeers |= ImGui->render(currentlyRenderedObjects, _ImGuiState);

		_physicsReset |= _resetPeers;
		_networkReset |= _resetPeers;
	}

	// Present our back buffer to our front buffer
	_swapChain->Present(0, 0);

	endOfFrame();
	sleepUntilNextFrame();
}
```
# Physics Loop
The physics loop is structured similarly, but delegates the actual physics calculations to scenarioManager.
```C++
void PhysicsSimulation::simLoop() {
	static auto scenarioManager = ScenarioManager::getInstance();
	QueryPerformanceCounter(&_start);
	bool tempGravity = _ImGuiState._gravityOn;
	bool tempInvert = _ImGuiState._gravityInverted;
	getInputs();
	if (_physicsReset) {
		scenarioManager->swapScene(_ImGuiState._currentScenarioIndex, _ImGuiState._gravityInverted);
		_physicsBufferStorage.clear();
		_physicsResetThisLoop = true;
		_renderReset |= true;
	}
	if (!_ImGuiState._paused) {
		scenarioManager->onUpdate(_ImGuiState._timestep, _ImGuiState._integrationType, _ImGuiState._gravityOn, _ImGuiState._gravityInverted);
	}
	endOfFrame();
	sleepUntilNextFrame();
}
```
This loop is where the majority of the physics calculations take place, and you may notice that objects are prevented from sticking together
via a "collision exemption list" rather than a backstep. I found that this worked much better for multiple collisions in rapid succession. Objects
out of bounds are also always immediately pushed towards the origin, so that even high velocity objects at a large time step will always stay in bounds.
```C++
for (auto i = 0; i < currentObjects.size() - 1; i++) {
	for (auto j = i + 1; j < currentObjects.size(); j++) {
		// Box is not exempt from collision
		if (i == 0) {
			const float boxLength = currentScene().getObjectPresets()[0]._initData._hollowCube._sideLength;
			if (currentObjects[0].testIntersect(currentObjects[j]))
				GameObject::addCollision(currentObjects[0], currentObjects[j], _globalFriction);
			while (isOutsideBox(currentObjects[j]))
				currentObjects[j].pushTowardsOrigin(0.01f * boxLength);
			while (currentObjects[0].testIntersect(currentObjects[j]))
				currentObjects[j].pushTowardsOrigin(0.01f * boxLength);
		}
		const bool& collisionExempt = _record.get(i, j);
		if (i != 0 && currentObjects[i].testIntersect(currentObjects[j])) {
			// Two colliding objects are exempt from collision until they have been observed not colliding for at least one frame
			if (!collisionExempt) {
				GameObject::addCollision(currentObjects[i], currentObjects[j], _globalFriction);
				_record.set(i, j);
			}
		}
		else {
			_record.clear(i, j);
		}
	}
```
Finally, floating point and integration errors are compensated for by keeping the sum of all kinetic and potential energy in the scene constant.
```C++
void ScenarioManager::stabilizeEnergy(bool invertedGravity) {
	const std::vector<GameObject>& objects = currentScene().getObjects();
	const ObjectPresets& boxPreset = currentScene().getObjectPresets()[0];

	const float currentEnergy = calculateEnergy(invertedGravity);
	const float difference = currentEnergy - _idealEnergy;

	if (difference <= -0.05 * _idealMomentum)
		_globalFriction = -0.05f;
	else if (difference >= 0.05 * _idealMomentum)
		_globalFriction = 0.05f;
	else
		_globalFriction = 0.00f;
}
```