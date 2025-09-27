#pragma once

#include "Scenario.h"

namespace helpers {
	enum peerColor;
}

class ScenarioC2 final : public Scenario
{
	float _totalTime = -1.0f;
	int _spheresSpawned = 0;

	void onUpdate(float deltaT, helpers::IntegrationType type, float gravity) override;

	void onUnload() override {
		_totalTime = -1.0f;
		_spheresSpawned = 0;
		Scenario::onUnload();
	}

public:
	ScenarioC2(CComPtr<ID3D11Device> device, helpers::peerColor color);
	ScenarioC2() = delete;

	static const char* myName() { return "Scenario C - 2k objects"; }
};

