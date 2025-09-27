#pragma once
#include "Scenario.h"

namespace helpers {
	enum peerColor;
	enum IntegrationType;
}

class ScenarioE final : public Scenario
{
public:
	ScenarioE(CComPtr<ID3D11Device> device, helpers::peerColor color);
	ScenarioE() = delete;

	static const char* myName() { return "Different Mass Spheres"; }
};

