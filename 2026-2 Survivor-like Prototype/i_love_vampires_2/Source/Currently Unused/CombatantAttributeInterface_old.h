#pragma once
#include "CoreMinimal.h"
#include "MyCombatantAttributeSet.h"


class CombatantAttributeInterface {
	MyCombatantAttributeSet* _attributes = nullptr;

public:
	CombatantAttributeInterface() = delete;
	CombatantAttributeInterface(const CombatantAttributeInterface&) = delete;
	CombatantAttributeInterface(CombatantAttributeInterface&&) = delete;
	CombatantAttributeInterface& operator=(const CombatantAttributeInterface&) = delete;
	CombatantAttributeInterface& operator=(CombatantAttributeInterface&&) = delete;

	CombatantAttributeInterface(MyCombatantAttributeSet* attributes) : _attributes(attributes) {}
	TArray<FName> getNames() const;
	TArray<float> getValues() const;
	TMap<FName, float> getMap() const;
	void setBaseAttributes(const TMap<FName, float>& newBaseAttributes);

private:
	bool safetyCheck() const;
};