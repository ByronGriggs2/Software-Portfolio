#pragma once
#include "Scenario.h"
class Lab4_Q2D final : public Scenario
{
	//void onLoad() override;
	//void onUpdate() override;
	//void onUnload() override;
public:
	Lab4_Q2D();
	const char* myName() const override { return "Lab4_Q2D"; }

	void generateObjects() const override;
};

