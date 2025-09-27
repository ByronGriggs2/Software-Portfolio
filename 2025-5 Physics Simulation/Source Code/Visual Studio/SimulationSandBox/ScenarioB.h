#pragma once
#include "Scenario.h"

namespace helpers {
	enum peerColor;
}

class ScenarioB final : public Scenario
{
	float _totalTime = 1.0f;
	int _spheresSpawned = 0;

	//void onLoad() override;
	void onUpdate(float deltaT, helpers::IntegrationType type, float gravity) override;
	void onUnload() override {
		_totalTime = 1.0f;
		_spheresSpawned = 0;
		Scenario::onUnload();
	}
public:
	ScenarioB(CComPtr<ID3D11Device> device, helpers::peerColor color);
	ScenarioB() = delete;

	static const char* myName() { return "Scenario B"; }
};

