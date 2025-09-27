#include "pch.h"
#include "Scenario.h"
#include "GameObject.h"

using namespace helpers;
using namespace shapeHelpers;
using namespace DirectX;

void Scenario::setShaderDefault(ObjectPresets& presets) { presets._ptrs = def; }
void Scenario::setShaderTexture(ObjectPresets& presets) { presets._ptrs = texture; }

Scenario::Scenario(CComPtr<ID3D11Device> device, peerColor color) : _objSpawner(device), _myColor(color) {
	//some boring color
	_backgroundColor.get()[0] = 0;
	_backgroundColor.get()[1] = 0.5;
	_backgroundColor.get()[2] = 0;
	// Every scene has this room. It takes up slot 0 in ObjectPresets (initial conditions vector) and the GameObject vector
	ObjectPresets cube = _objSpawner.newObjPreset(hollowCube);
	cube._initData._hollowCube._sideLength = 3.0f;
	setShaderTexture(cube);
	_objectPresets.push_back(cube);
		
}

// Creates GameObjects based on presets. Change presets in constructor or OnLoad in Scenario's children.
void Scenario::onLoad() {
	for (const auto& obj : _objectPresets) {
		if (obj._forcedRed)
			_objSpawner.makeNextObjectRed();
		_objSpawner.addGameObjectToScene(_objectsToDraw, obj, _myColor);
	}
}

void Scenario::onUpdate(float deltaT, IntegrationType type, float gravity) {
	for (auto& obj : _objectsToDraw) {
#ifndef NOT_DISTRIBUTED
#ifdef _DEBUG
		if (obj.getColor() != _myColor && obj.getType() != shapeHelpers::hollowCube) {
			throw ShouldNotRunException();
			return;
		}
#endif
#endif
		if (!_forceGravityOff)
			obj.addLinearForce(XMVectorSet(0, gravity, 0, 0));
		if (type == euler)
			obj.updateEuler(deltaT);
		else if (type == semiEuler)
			obj.updateSemiEuler(deltaT);
		else if (type == runge_kutta)
			obj.updateRungeKutta(deltaT);
	}
}