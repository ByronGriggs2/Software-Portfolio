#pragma once
#include "Scenario.h"

namespace helpers {
	enum peerColor;
	enum IntegrationType;
}

class ScenarioH final : public Scenario
{
public:
	ScenarioH(CComPtr<ID3D11Device> device, helpers::peerColor color);
	ScenarioH() = delete;

	static const char* myName() { return "Somewhat Inelastic Collision"; }
};

