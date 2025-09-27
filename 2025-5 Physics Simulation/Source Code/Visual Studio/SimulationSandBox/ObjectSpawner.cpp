#include "pch.h"
#include "ObjectSpawner.h"
#include "GameObject.h"

using namespace DirectX;
using namespace helpers;

ObjectSpawner::ObjectSpawner(const CComPtr<ID3D11Device>& device) : _device(device), _nextColor(undef) {
	srand(static_cast<unsigned int>(time(0)));
}

// Don't need this factory function anymore
ObjectPresets ObjectSpawner::newObjPreset(shapeHelpers::ObjType type) const {
	ObjectPresets temp(type);
	//temp._ptrs = _defaultPtrs;
	return temp;
}

void ObjectSpawner::spawnRandomSphere(ObjectPresets& preset, std::vector<GameObject>& sceneObjects, peerColor computerColor) {
	randomiseObjectPosition(preset, sceneObjects);
	addGameObjectToScene(sceneObjects, preset, computerColor);
}

void ObjectSpawner::randomiseObjectPosition(ObjectPresets& preset, std::vector<GameObject>& sceneObjects) const {
	// Boundaries of cube
	const float minimumPos = -1.5f;
	const float posRange = 3.0f;

	bool colliding = true;
	while (colliding) {
		preset._params._position = XMVectorSet(
			minimumPos + (posRange / 1000.0f) * static_cast<float>(rand() % 1000),
			minimumPos + (posRange / 1000.0f) * static_cast<float>(rand() % 1000),
			minimumPos + (posRange / 1000.0f) * static_cast<float>(rand() % 1000),
			0);
		colliding = isColliding(preset, sceneObjects);
	}
}

// Very inefficient but simple
void ObjectSpawner::initialiseRandomObject(ObjectPresets& preset, const std::vector<ObjectPresets>& scenePresets) const {
	std::vector<GameObject> testVector;
	for (const auto& obj : scenePresets) {
		testVector.push_back(GameObject(obj));
	}
	randomiseObjectPosition(preset, testVector);
}

bool ObjectSpawner::isColliding(const ObjectPresets& preset, const std::vector<GameObject>& others) const {
	const GameObject presetTest = GameObject(preset);
	for (auto& other : others) {
		if (presetTest.testIntersect(other))
			return true;
	}
	return false;
}

#ifdef NOT_DISTRIBUTED
void ObjectSpawner::onUnload() { srand(time(0)); }
void ObjectSpawner::addGameObjectToScene(std::vector<GameObject>& sceneObjects, const ObjectPresets& obj, peerColor computerColor) {
	sceneObjects.push_back(GameObject(obj));
	sceneObjects.back().setBuffers(_device);
	sceneObjects.back().setColor(computerColor);
}
void ObjectSpawner::makeNextObjectRed() {}
#else
void ObjectSpawner::onUnload() { _nextColor = red; srand(static_cast<unsigned int>(time(0))); }
void ObjectSpawner::addGameObjectToScene(std::vector<GameObject>& sceneObjects, const ObjectPresets& obj, peerColor computerColor) {
	// All scenes own their own cube
	if (computerColor == _nextColor || obj._type == shapeHelpers::hollowCube) {
		GameObject temp(obj);
		temp.setBuffers(_device);
		temp.setColor(_nextColor);
		sceneObjects.push_back(temp);
	}
	incrementNextColor();
}
void ObjectSpawner::makeNextObjectRed() { _nextColor = red; }
#endif

#ifndef NOT_DISTRIBUTED
peerColor ObjectSpawner::incrementNextColor() {
	if (_nextColor == blue)
		_nextColor = red;
	else
		_nextColor = static_cast<enum peerColor>(_nextColor + 1);
	return _nextColor;
}
#endif