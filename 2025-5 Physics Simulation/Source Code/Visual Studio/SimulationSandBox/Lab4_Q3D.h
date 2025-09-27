#pragma once
#include "Scenario.h"
class Lab4_Q3D final : public Scenario
{
	//void onLoad() override;
	//void onUpdate() override;
	//void onUnload() override;
public:
	Lab4_Q3D();
	const char* myName() const override { return "Lab4_Q3D"; }

	void generateObjects() const override;
};

