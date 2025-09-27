#pragma once
#include "Scenario.h"
class Lab4_Q2B final : public Scenario
{
	//void onLoad() override;
	//void onUpdate() override;
	//void onUnload() override;
public:
	Lab4_Q2B();
	const char* myName() const override { return "Lab4_Q2B"; }

	void generateObjects() const override;
};

