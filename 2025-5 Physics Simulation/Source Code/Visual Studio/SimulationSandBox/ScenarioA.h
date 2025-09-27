#pragma once
#include "Scenario.h"

namespace helpers {
	enum peerColor;
}

class ScenarioA final : public Scenario
{
	float _totalTime = 1.0f;
	int _spheresSpawned = 0;

	void onUpdate(float deltaT, helpers::IntegrationType type, float gravity) override;

	void onUnload() override {
		_totalTime = 1.0f;
		_spheresSpawned = 0;
		Scenario::onUnload();
	}

public:
	ScenarioA(CComPtr<ID3D11Device> device, helpers::peerColor color);
	ScenarioA() = delete;

	static const char* myName() { return "Scenario A"; }
};

