#pragma once
#include "Scenario.h"

namespace helpers {
	enum peerColor;
	enum IntegrationType;
}

class ScenarioF final : public Scenario
{
public:
	ScenarioF(CComPtr<ID3D11Device> device, helpers::peerColor color);
	ScenarioF() = delete;

	static const char* myName() { return "Different Density Spheres"; }
};

