#include "pch.h"
#include "ScenarioA.h"

using namespace DirectX;
using namespace helpers;
using namespace shapeHelpers;


ScenarioA::ScenarioA(CComPtr<ID3D11Device> device, peerColor color) : Scenario(device, color) {
	srand(static_cast<unsigned int>(time(0)));
	// static balls
	{
		for (auto i = 0; i < 9; i++) {
			ObjectPresets tempSphere = _objSpawner.newObjPreset(sphere);
			tempSphere._initData._sphere._radius = 0.1f + 0.001f * static_cast<float>(rand() % 350);
			tempSphere._initData._sphere._tesselation = 100;
			_objSpawner.initialiseRandomObject(tempSphere, _objectPresets);
			tempSphere._params._fixed = true;
			_objectPresets.push_back(tempSphere);
		}
	}
}

void ScenarioA::onUpdate(float deltaT, IntegrationType type, float gravity) {
	if (_totalTime < 9.0f)
		_totalTime += deltaT;
	if (static_cast<int>(_totalTime) > _spheresSpawned) {
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