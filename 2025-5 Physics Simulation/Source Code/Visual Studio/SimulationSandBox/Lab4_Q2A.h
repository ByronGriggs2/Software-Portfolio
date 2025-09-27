#pragma once
#include "Scenario.h"
class Lab4_Q2A final : public Scenario
{
	//void onLoad() override;
	//void onUpdate() override;
	//void onUnload() override;
public:
	Lab4_Q2A();
	const char* myName() const override { return "Lab4 Q2A"; }

	void generateObjects() const override;
};

