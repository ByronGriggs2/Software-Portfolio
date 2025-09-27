#pragma once

#include <vector>
#include <DirectXMath.h>
#include <memory>
#include <atlbase.h>
#include "GameObject.h"
#include "ObjectSpawner.h"

namespace helpers {
	struct ObjectPtrs;
	struct ObjectPresets;
	struct shaderListStruct;
	enum peerColor;
	enum IntegrationType;
}

// Scenario is responsible for the initial conditions of the simulation as well as creating, maintaining, and destroying all objects in the loaded scene, including moving them and simulating collision
class Scenario
{
	DirectX::XMVECTOR _cameraPos = DirectX::XMVectorSet(0, 1.0f, -1.5f, 1.0f);
	DirectX::XMVECTOR _lightPos = DirectX::XMVectorSet(0, 1000, 0, 1);
	std::shared_ptr<float[]> _backgroundColor = std::make_shared<float[4]>(0.0f);
protected:
	ObjectSpawner _objSpawner;
	const helpers::peerColor _myColor;
	std::vector<GameObject> _objectsToDraw;
	std::vector<helpers::ObjectPresets> _objectPresets;
	bool _forceGravityOff = false;
public:
	Scenario(CComPtr<ID3D11Device> device, helpers::peerColor color);
	Scenario() = delete;
	virtual ~Scenario() = default;

	virtual void onLoad();
	virtual void onUpdate(float deltaT, helpers::IntegrationType type, float gravity);
	virtual void onUnload() { _objectsToDraw.clear(); _objSpawner.onUnload(); }

protected:
	void setShaderDefault(helpers::ObjectPresets& presets);
	void setShaderTexture(helpers::ObjectPresets& presets);
public:
	const std::vector<GameObject>& getObjects() const { return _objectsToDraw; }
	const std::vector<helpers::ObjectPresets>& getObjectPresets() const { return _objectPresets; }
	const DirectX::XMVECTOR& getCameraPos() const { return _cameraPos; }
	const DirectX::XMVECTOR& getLightPos() const { return _lightPos; }
	float* getBackgroundColorReference() { return _backgroundColor.get(); }
	const float* getBackgroundColorValue() const { return _backgroundColor.get(); }

	std::vector<GameObject>& getObjectsByReference() { return _objectsToDraw; }
};

