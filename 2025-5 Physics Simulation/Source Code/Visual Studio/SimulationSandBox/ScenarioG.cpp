#include "pch.h"
#include "ScenarioG.h"

using namespace helpers;
using namespace shapeHelpers;
using namespace DirectX;

ScenarioG::ScenarioG(CComPtr<ID3D11Device> device, peerColor color) : Scenario(device, color) {
	_forceGravityOff = true;

	auto sphere1 = _objSpawner.newObjPreset(sphere);
	sphere1._initData._sphere._radius = 0.4f;
	sphere1._initData._sphere._tesselation = 100;
	sphere1._forcedRed = true;
	sphere1._params._elasticity = 0.1f;
	sphere1._params._linearVelocity = XMVectorSet(1, 0, 0, 0);
	sphere1._params._position = XMVectorSet(-1, 0, 0, 0);
	_objectPresets.push_back(sphere1);

	auto sphere2 = _objSpawner.newObjPreset(sphere);
	sphere2._initData._sphere._radius = 0.4f;
	sphere2._initData._sphere._tesselation = 100;
	sphere2._forcedRed = true;
	sphere2._params._elasticity = 0.1f;
	sphere2._params._linearVelocity = XMVectorSet(-1, 0, 0, 0);
	sphere2._params._position = XMVectorSet(1, 0, 0, 0);
	_objectPresets.push_back(sphere2);
}