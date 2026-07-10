#pragma once
#include <unordered_map>
#include "CombatantAttributeSetEnum.h"
#include "CoreMinimal.h"

class Item {
protected:
	FString _name = "Item";
	FName _ID = "item";
public:
	Item() {};
	// not automatic
	virtual void tick(float delta, const std::unordered_map<CombatantAttribute::MyEnum, float>& attributeSnapshot) = 0;
};