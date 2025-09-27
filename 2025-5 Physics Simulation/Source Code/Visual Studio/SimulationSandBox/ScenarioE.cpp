#include "pch.h"
#include "ScenarioE.h"

using namespace helpers;
using namespace shapeHelpers;
using namespace DirectX;

ScenarioE::ScenarioE(CComPtr<ID3D11Device> device, peerColor color) : Scenario(device, color) {
	_forceGravityOff = true;

	auto sphere1 = _objSpawner.newObjPreset(sphere);
	sphere1._initData._sphere._radius = 0.5;
	sphere1._initData._sphere._tesselation = 100;
	sphere1._forcedRed = true;
	sphere1._params._mass = 20;
	sphere1._params._linearVelocity = XMVectorSet(1, 0, 0, 0);
	sphere1._params._position = XMVectorSet(-1, -sqrt(2.0f) / 2.0f, 0, 0);
	_objectPresets.push_back(sphere1);

	auto sphere2 = _objSpawner.newObjPreset(sphere);
	sphere2._initData._sphere._radius = 0.25;
	sphere2._initData._sphere._tesselation = 100;
	sphere2._forcedRed = true;
	sphere2._params._mass = 5;
	sphere2._params._linearVelocity = XMVectorSet(-1, 0, 0, 0);
	sphere2._params._position = XMVectorSet(1, 0, 0, 0);
	_objectPresets.push_back(sphere2);
}