#pragma once
#include "Scenario.h"

namespace helpers {
	enum peerColor;
	enum IntegrationType;
}

class ScenarioG final : public Scenario
{
public:
	ScenarioG(CComPtr<ID3D11Device> device, helpers::peerColor color);
	ScenarioG() = delete;

	static const char* myName() { return "Mostly Inelastic Collision"; }
};

