#include "pch.h"
#include "ScenarioC2.h"

using namespace DirectX;
using namespace helpers;
using namespace shapeHelpers;

ScenarioC2::ScenarioC2(CComPtr<ID3D11Device> device, helpers::peerColor color) : Scenario(device, color) {
	srand(static_cast<unsigned int>(time(0)));
}

void ScenarioC2::onUpdate(float deltaT, IntegrationType type, float gravity) {
	if (_totalTime < 9.0f)
		_totalTime += deltaT;
	if (_totalTime > static_cast<float>(_spheresSpawned / 100.0f)) {

	}
	while (_spheresSpawned < 2000) {
		ObjectPresets newSphere = _objSpawner.newObjPreset(sphere);
		newSphere._initData._sphere._radius = 0.0004f * static_cast<float>(rand() % 100);
		newSphere._initData._sphere._tesselation = 100;
		newSphere._params._linearVelocity = XMVectorSet(-2.0f + 0.01f * static_cast<float>(rand() % 400),
			-2.0f + 0.01f * static_cast<float>(rand() % 400),
			-2.0f + 0.01f * static_cast<float>(rand() % 400),
			0);
		_objSpawner.spawnRandomSphere(newSphere, _objectsToDraw, _myColor);
#ifdef _DEBUG
		if (newSphere._initData._sphere._radius > 0.04)
			myExcept("Sphere random radius too big 1");
		if (_objectsToDraw.back().getType() == sphere && _objectsToDraw.back().getSphereRadius() > 0.04)
			myExcept("Sphere random radius too big 2");
#endif
		_spheresSpawned++;
	}
	Scenario::onUpdate(deltaT, type, gravity);
}