#pragma once
#include "Scenario.h"
class Lab4_Q3A final : public Scenario
{
	//void onLoad() override;
	//void onUpdate() override;
	//void onUnload() override;
public:
	Lab4_Q3A();
	const char* myName() const override { return "Lab4_Q3A"; }

	void generateObjects() const override;
};

