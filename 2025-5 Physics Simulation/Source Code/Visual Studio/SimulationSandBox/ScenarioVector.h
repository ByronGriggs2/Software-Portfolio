#pragma once
#include <vector>
#include <memory>
#include <atlbase.h>
#include <d3d11.h>

class Scenario;
namespace helpers {
	struct shaderListStruct;
	struct ObjectPtrs;
	enum peerColor;
}

// Convenient way to make a list(s) of all scenarios
class ScenarioVector {
public:
	ScenarioVector() = delete;
	ScenarioVector(const ScenarioVector&) = delete;
	ScenarioVector& operator=(const ScenarioVector&) = delete;
	ScenarioVector& operator=(const ScenarioVector&&) = delete;
	ScenarioVector(const ScenarioVector&&) = delete;

	static std::vector<std::unique_ptr<Scenario>> giveVector(CComPtr<ID3D11Device> device, helpers::peerColor color);
	static std::vector<const char*> getScenarioNames();
};