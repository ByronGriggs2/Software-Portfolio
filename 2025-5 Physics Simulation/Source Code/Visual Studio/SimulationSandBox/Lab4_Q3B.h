#pragma once
#include "Scenario.h"
class Lab4_Q3B final : public Scenario
{
	//void onLoad() override;
	//void onUpdate() override;
	//void onUnload() override;
public:
	Lab4_Q3B();
	const char* myName() const override { return "Lab4_Q3B"; }

	void generateObjects() const override;
};

