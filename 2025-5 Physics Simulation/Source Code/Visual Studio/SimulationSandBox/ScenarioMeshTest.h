#pragma once
#include "Scenario.h"
class ScenarioMeshTest : public Scenario
{
public:

	ScenarioMeshTest(const helpers::shaderListStruct& list, const helpers::ObjectPtrs& ptrs);
	ScenarioMeshTest() = delete;

	void onUpdate(double deltaT, IntegrationType type) override {
		Scenario::onUpdate(deltaT, type);

		_objectsToDraw.front().setAngularVelocity(XMVectorSet(0, 1, 0, 0));
	}

	const char* myName() const override { return "Mesh Test"; }

	void generateObjects() const override;
};

