#pragma once
#include "Scenario.h"
class Lab4_Q1 final : public Scenario
{
	//void onLoad() override;
	//void onUpdate() override;
	//void onUnload() override;
public:
	Lab4_Q1();
	const char* myName() const override { return "Lab4_Q1"; }

	void generateObjects() const override;
};

