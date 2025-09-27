#pragma once
#include "Scenario.h"
class Lab4_Q3C final : public Scenario
{
	//void onLoad() override;
	//void onUpdate() override;
	//void onUnload() override;
public:
	Lab4_Q3C();
	const char* myName() const override { return "Lab4_Q3C"; }

	void generateObjects() const override;
};

