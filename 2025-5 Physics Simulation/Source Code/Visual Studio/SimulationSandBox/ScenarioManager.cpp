#include "pch.h"
#include "ScenarioManager.h"
#include "ScenarioVector.h"
#include "Scenario.h"
#include "NetCompatibleGameObject.h"

using namespace DirectX;
using namespace helpers;
using namespace shapeHelpers;

void ScenarioManager::initializeScenarioList(CComPtr<ID3D11Device> device, peerColor color) {
	_scenarioList = std::move(ScenarioVector::giveVector(device, color));
}
std::vector<GameObject> ScenarioManager::getGameObjects() const { return currentScene().getObjects(); }
XMVECTOR ScenarioManager::getCameraPos() const { return currentScene().getCameraPos(); }
XMVECTOR ScenarioManager::getLightPos() const { return currentScene().getLightPos(); }
const float* ScenarioManager::getBackgroundColor() const { return currentScene().getBackgroundColorValue(); }

void ScenarioManager::swapScene(int newSceneIndex, bool invertedGravity) {
	if (_scenarioList.empty())
		return;
	currentScene().onUnload();
	_currentSceneIndex = newSceneIndex;
	_record.resetVector();
	_idealEnergy = 0;
	_idealMomentum = 0;
	currentScene().onLoad();
	updateRecords(invertedGravity);
}

void ScenarioManager::updateRecords(bool invertedGravity) {
	const std::vector<GameObject>& newObjects = currentScene().getObjects();
	const static ObjectPresets boxPreset = currentScene().getObjectPresets()[0];
	const static float boxPos = XMVectorGetY(boxPreset._params._position);
	const static float boxLength = boxPreset._initData._hollowCube._sideLength;

	const int newSize = static_cast<int>(newObjects.size());
	const int oldSize = _record._totalObjectCount;
	if (oldSize == 0) {
		_record.resizeVector(newSize);
		_idealEnergy = calculateEnergy(invertedGravity);
		_idealMomentum = calculateMomentum();
		return;
	}
	else if (newSize != oldSize) {
		_record.resizeVector(newSize);
		for (auto i = oldSize; i < newSize; i++) {
			_idealMomentum += newObjects[i].getMomentum();
			float potentialEnergyReference;
			if (invertedGravity)
				potentialEnergyReference = boxPos + boxLength / 2.0f;
			else
				potentialEnergyReference = boxPos - boxLength / 2.0f;
			_idealEnergy += newObjects[i].getEnergy(potentialEnergyReference);
		}
	}
}

bool ScenarioManager::isOutsideBox(const GameObject& other) const {
	const ObjectPresets& boxPreset = currentScene().getObjectPresets()[0];
	XMFLOAT3 otherPos;
	XMStoreFloat3(&otherPos, other.getPosition());
	XMFLOAT3 boxPos;
	XMStoreFloat3(&boxPos, boxPreset._params._position);
	const float boxLength = boxPreset._initData._hollowCube._sideLength;

	if (otherPos.x <= boxPos.x - boxLength / 2.0f || otherPos.x >= boxPos.x + boxLength / 2.0f)
		return true;
	if (otherPos.y <= boxPos.y - boxLength / 2.0f || otherPos.y >= boxPos.y + boxLength / 2.0f)
		return true;
	if (otherPos.z <= boxPos.z - boxLength / 2.0f || otherPos.z >= boxPos.z + boxLength / 2.0f)
		return true;

	return false;
}

void ScenarioManager::stabilizeMomentum() {
	const std::vector<GameObject>& objects = currentScene().getObjects();
	const float currentMomentum = calculateMomentum();
	const float difference = currentMomentum - _idealMomentum;

	if (difference <= -0.05 * _idealMomentum)
		_globalFriction = -0.05f;
	else if (difference >= 0.05 * _idealMomentum)
		_globalFriction = 0.05f;
	else
		_globalFriction = 0.05f;
}

// For gravity
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

void ScenarioManager::onUpdate(float deltaT, IntegrationType type, bool gravityOn, bool gravityInverted) {
	auto& currentObjects = currentScene().getObjectsByReference();
	if (currentObjects.empty())
		return;
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
	}
	float gravity;
	if (!gravityOn)
		gravity = 0;
	else if (gravityInverted)
		gravity = -GRAVITY;
	else
		gravity = GRAVITY;
	currentScene().onUpdate(deltaT, type, gravity);

	updateRecords(gravityInverted);
	if (gravityOn)
		stabilizeEnergy(gravityInverted);
	else
		stabilizeMomentum();
}

std::vector<GlobalObjectStruct> ScenarioManager::getNetCompatibleObjects(std::unordered_map<int, std::shared_ptr<NetCompatibleBuffers>>& buf) const {
	auto scenarioManager = ScenarioManager::getInstance();
	const std::vector<GameObject>& myObjects = currentScene().getObjects();

	std::vector<GlobalObjectStruct> tempVector;
	for (int i = 0; i < myObjects.size(); i++) {
		// Because objects are never removed from the scene or moved around in their vector, the UID 
		// is just the position in the vector
		const int UID = i;

		const auto foundID = buf.find(UID);
		const bool newData = foundID == buf.end();
		if (newData) {
			buf[UID] = std::make_shared<NetCompatibleBuffers>(myObjects[i]);
		}
		tempVector.push_back({ newData, NetCompatibleGameObject(myObjects[i], UID, buf.find(UID)->second) });
	}
	return tempVector;
}

float ScenarioManager::calculateEnergy(bool invertedGravity) {
	const static ObjectPresets boxPreset = currentScene().getObjectPresets()[0];
	const static float boxPos = XMVectorGetY(boxPreset._params._position);
	const static float boxLength = boxPreset._initData._hollowCube._sideLength;
	float potentialEnergyReference;
	if (invertedGravity)
		potentialEnergyReference = (boxPos + boxLength / 2.0f);
	else
		potentialEnergyReference = (boxPos - boxLength / 2.0f);
	float totalEnergy = 0;
	for (const auto& object : currentScene().getObjects())
		totalEnergy += object.getEnergy(potentialEnergyReference);
	return totalEnergy;
}

float ScenarioManager::calculateMomentum() {
	float totalMomentum = 0;
	for (const auto& object : currentScene().getObjects())
		totalMomentum += object.getMomentum();
	return totalMomentum;
}