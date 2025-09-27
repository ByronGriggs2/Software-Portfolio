#include "pch.h"
#include "GameObject.h"
#include "GeometryGenerator.h"

using namespace shapeHelpers;
using namespace helpers;

bool GameObject::testIntersect(const GameObject& other) const { return _physics->testIntersect(other._physics.get()); }
void GameObject::updateEuler(float deltaT) { _physics->updateEuler(deltaT); }
void GameObject::updateSemiEuler(float deltaT) { _physics->updateSemiEuler(deltaT); }
void GameObject::updateRungeKutta(float deltaT) { _physics->updateRungeKutta(deltaT); }
void GameObject::addLinearForce(const DirectX::XMVECTOR& force) { _physics->addLinearForce(force); }
void GameObject::pushTowardsOrigin(float distanceToPush) { _physics->pushTowardsOrigin(distanceToPush); }

int GameObject::getIndexCount() const { return _geometry.indexCount(); }
ObjType GameObject::getType() const { return _physics->getType(); }
DirectX::XMMATRIX GameObject::getOrientation() const { return _physics->getOrientation(); }
const DirectX::XMVECTOR& GameObject::getPosition() const { return _physics->getPosition(); }
bool GameObject::isFixed() const { return _physics->isFixed(); }
float GameObject::getMomentum() const { return _physics->getMomentum(); }
float GameObject::getEnergy(float potentialEnergyReference) const { return _physics->getEnergy(potentialEnergyReference); }

void GameObject::setVertices(const std::vector<SimpleVertex>& v) { _geometry.setVertices(v); }
void GameObject::setIndices(const std::vector<WORD>& i) { _geometry.setIndices(i); }

GameObject::GameObject(GameObject&& other) noexcept : 
	_geometry(std::move(other._geometry)), 
	_physics(std::move(other._physics)), 
	_ptrPreset(other._ptrPreset), 
	_myColor(other._myColor), 
	_vertexBuffer(other._vertexBuffer),
	_indexBuffer(other._indexBuffer) {}
GameObject& GameObject::operator=(const GameObject& other) {
	if (this == &other) return *this;
	_geometry = other._geometry;
	_ptrPreset = other._ptrPreset;
	_myColor = other._myColor;
	_vertexBuffer = other._vertexBuffer;
	_indexBuffer = other._indexBuffer;

	if (other._physics) {
		_physics = other._physics->clone();
	}
	else {
		_physics.reset();
	}
	return *this;
}
GameObject& GameObject::operator=(GameObject&& other) noexcept {
	if (this == &other) return *this;
	_geometry = std::move(other._geometry);
	_physics = std::move(other._physics);
	_ptrPreset = other._ptrPreset;
	_myColor = other._myColor;
	_vertexBuffer = other._vertexBuffer;
	_indexBuffer = other._indexBuffer;
	return *this;
}
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

// Copy Constructor
GameObject::GameObject(const GameObject& other) :
	_geometry(other._geometry),
	_ptrPreset(other._ptrPreset),
	_myColor(other._myColor),
	_vertexBuffer(other._vertexBuffer),
	_indexBuffer(other._indexBuffer)
{
	if (other._physics) {
		_physics = other._physics->clone();
	}
}

void GameObject::setBuffers(const CComPtr<ID3D11Device>& pd3dDevice) {
	std::vector<SimpleVertex> tempVertices = _geometry.getVertices();

	D3D11_BUFFER_DESC bd = {};
#ifdef DEBUGVERTEXBUFFER
	bd.Usage = D3D11_USAGE_STAGING;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bd.BindFlags = 0;
#else
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
#endif
	D3D11_SUBRESOURCE_DATA InitData = {};

	bd.ByteWidth = _geometry.vertexCount() * sizeof(SimpleVertex);
	InitData.pSysMem = &tempVertices[0];
#ifdef DEBUGVERTEXBUFFER
	HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_vertexBuffer);
	if (FAILED(hr))
		assert(0);
#else
	pd3dDevice->CreateBuffer(&bd, &InitData, &_vertexBuffer);
#endif

	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.ByteWidth = _geometry.indexCount() * sizeof(WORD);
	std::vector<WORD> tempIndexVector = _geometry.getIndices();
	InitData.pSysMem = &tempIndexVector[0];
	pd3dDevice->CreateBuffer(&bd, &InitData, &_indexBuffer);
}

void GameObject::addCollision(GameObject& obj1, GameObject& obj2, float extraFriction) {
	if (obj1.getType() == sphere && obj2.getType() == sphere) {
		CollisionManager::sphereMassCollision(*obj1._physics, *obj2._physics, extraFriction);
		return;
	}
	CollisionManager::collisionSimplified(*obj1._physics, *obj2._physics, extraFriction);
}

float GameObject::getSphereRadius() {
	if (_physics->getType() != sphere) {
#ifdef _DEBUG
		myExcept("Calling sphere radius on not sphere");
#endif
		return 1;
	}
	const Sphere* temp = dynamic_cast<Sphere*>(_physics.get());
	return temp->getRadius();
}