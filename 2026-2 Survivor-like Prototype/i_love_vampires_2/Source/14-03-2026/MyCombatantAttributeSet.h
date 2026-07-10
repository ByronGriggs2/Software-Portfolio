#pragma once
#include "CoreMinimal.h"
#include "MyAttributeSet.h"
#include "MyCombatantAttributeSet.generated.h"
class UCombatantAttributes;

UCLASS()
class I_LOVE_VAMPIRES_2_API UMyCombatantAttributeSet : public UMyAttributeSet
{
	GENERATED_BODY()
protected:
	virtual bool updateFromModifiers() override { return true; }

public:
	// no other attribute set modifies combatant attribute set
	void initialise_UMyCombatantAttributeSet() { initialise_UMyAttributeSet(nullptr, UCombatantAttributes::StaticClass()); }
	void burnTick();
};