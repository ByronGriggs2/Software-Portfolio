#pragma once

#include <vector>
#include <memory>
#include <DirectXMath.h>
#include <atlbase.h>
#include <d3d11.h>
#include "Scenario.h"
#include "unordered_map"
#include "CollisionExemptionRecord.h"

class GameObject;
namespace helpers {
	struct shaderListStruct;
	struct ObjectPtrs;
	enum IntegrationType;
	enum peerColor;
}
struct GlobalObjectStruct;
class NetCompatibleBuffers;

/////////////////////
// Singleton

// This class serves to handle the majority of the physics simulation while ignoring threads.
class ScenarioManager
{
	std::vector <std::unique_ptr<Scenario>> _scenarioList = {};
	int _currentSceneIndex = 0;
	Scenario& currentScene() const { return *_scenarioList[_currentSceneIndex]; }

	CollisionExemptionRecord _record;
	float _idealMomentum = 0.0f;
	float _idealEnergy = 0.0f;
	float _globalFriction = 0.0f;

	void updateRecords(bool invertedGravity);
	bool isOutsideBox(const GameObject& other) const;
	void stabilizeMomentum();
	void stabilizeEnergy(bool invertedGravity);
	float calculateEnergy(bool invertedGravity);
	float calculateMomentum();

	ScenarioManager() = default;
public:
	ScenarioManager(const ScenarioManager& other) = delete;
	ScenarioManager& operator=(const ScenarioManager& other) = delete;
	ScenarioManager(const ScenarioManager&& other) = delete;
	ScenarioManager& operator=(const ScenarioManager&&) = delete;

	static std::shared_ptr<ScenarioManager> getInstance() { 
		static std::shared_ptr<ScenarioManager> instance(new ScenarioManager);
		return instance;
	}
	// Creates a list containing one of every scenario. This list is maintained until this class is destroyed.
	void initializeScenarioList(CComPtr<ID3D11Device> device, helpers::peerColor color);
	// Changes from one scene to another. Deletes all GameObjects, including their vertex/index buffers and geometry.
	// 'Reset' is just swapping from a scenario to itself.
	void swapScene(int newSceneIndex, bool invertedGravity);

	std::vector<GlobalObjectStruct> getNetCompatibleObjects(std::unordered_map<int, std::shared_ptr<NetCompatibleBuffers>>& buf) const;
	std::vector<GameObject> getGameObjects() const;
	DirectX::XMVECTOR getCameraPos() const;
	DirectX::XMVECTOR getLightPos() const;
	const float* getBackgroundColor() const;
	void onUpdate(float deltaT, helpers::IntegrationType type, bool gravityOn, bool gravityInverted);
};

