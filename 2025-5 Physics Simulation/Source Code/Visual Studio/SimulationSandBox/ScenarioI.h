#pragma once
#include "Scenario.h"

namespace helpers {
	enum peerColor;
	enum IntegrationType;
}

class ScenarioI final : public Scenario
{
public:
	ScenarioI(CComPtr<ID3D11Device> device, helpers::peerColor color);
	ScenarioI() = delete;

	static const char* myName() { return "Different Elasticities"; }
};

