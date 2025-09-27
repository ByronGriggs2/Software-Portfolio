#include "pch.h"
#include "ScenarioB.h"

using namespace DirectX;
using namespace helpers;
using namespace shapeHelpers;

ScenarioB::ScenarioB(CComPtr<ID3D11Device> device, helpers::peerColor color) : Scenario(device, color) {
	srand(0);
	// static capsules
	for (auto i = 0; i < 9; i++) {
		ObjectPresets tempCapsule = _objSpawner.newObjPreset(capsule);
		tempCapsule._initData._capsule._radius = 0.1f + 0.001f * static_cast<float>(rand() % 350);
		tempCapsule._initData._capsule._height = 0.15f + 0.0015f * static_cast<float>(rand() % 350);
		tempCapsule._initData._capsule._tesselation = 100;
		tempCapsule._params._rotation = XMQuaternionRotationRollPitchYaw(
			0.01f * XM_2PI * static_cast<float>(rand() % 100),
			0.01f * XM_2PI * static_cast<float>(rand() % 100),
			0.01f * XM_2PI * static_cast<float>(rand() % 100)
		);
		_objSpawner.initialiseRandomObject(tempCapsule, _objectPresets);
		tempCapsule._params._fixed = true;

		_objectPresets.push_back(tempCapsule);
	}
}

void ScenarioB::onUpdate(float deltaT, IntegrationType type, float gravity) {
	if (_totalTime < 9.0f)
		_totalTime += deltaT;
	if (static_cast<int>(_totalTime) > _spheresSpawned) {
		srand(static_cast<unsigned int>(time(0)));
		ObjectPresets newSphere = _objSpawner.newObjPreset(sphere);
		newSphere._initData._sphere._radius = 0.25f;
		newSphere._initData._sphere._tesselation = 100;
		newSphere._params._linearVelocity = XMVectorSet(-2.0f + 0.01f * static_cast<float>(rand() % 400),
			-2.0f + 0.01f * static_cast<float>(rand() % 400),
			-2.0f + 0.01f * static_cast<float>(rand() % 400),
			0);
		_objSpawner.spawnRandomSphere(newSphere, _objectsToDraw, _myColor);
		_spheresSpawned++;
	}
	Scenario::onUpdate(deltaT, type, gravity);
}