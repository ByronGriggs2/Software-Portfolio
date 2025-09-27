#pragma once

#include <vector>
#include <memory>
#include <filesystem>
#include "d3d11.h"
#include "atlbase.h"

class GameObject;
namespace helpers {
	struct ObjectPresets;
	enum peerColor;
}
namespace shapeHelpers {
	enum ObjType;
}

class ObjectSpawner final
{
	CComPtr<ID3D11Device> _device = nullptr;
	helpers::peerColor _nextColor;

	bool isColliding(const helpers::ObjectPresets& preset, const std::vector<GameObject>& others) const;
	helpers::peerColor incrementNextColor();

public:

	ObjectSpawner(const CComPtr<ID3D11Device>& device);
	ObjectSpawner() = delete;
	void onUnload();

	void addGameObjectToScene(std::vector<GameObject>& sceneObjects, const helpers::ObjectPresets& obj, helpers::peerColor computerColor);
	void spawnRandomSphere(helpers::ObjectPresets& preset, std::vector<GameObject>& sceneObjects, helpers::peerColor computerColor);
	void randomiseObjectPosition(helpers::ObjectPresets& preset, std::vector<GameObject>& sceneObjects) const;
	void initialiseRandomObject(helpers::ObjectPresets& preset, const std::vector<helpers::ObjectPresets>& scenePresets) const;
	void makeNextObjectRed();
	
	helpers::ObjectPresets newObjPreset(shapeHelpers::ObjType type) const;
};

