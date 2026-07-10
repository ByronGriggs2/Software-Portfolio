#pragma once
#include "CoreMinimal.h"
#include "MyAttributeSet.h"
#include "WeaponAttributes.h"
class UMyCombatantAttributeSet;

class UMyWeaponAttributeSet : public UMyAttributeSet<FWeaponAttributes>
{
protected:
	virtual void updateFromModifiers() override;

public:
	UMyWeaponAttributeSet() = default;
	void initialise_UMyWeaponAttributeSet(const FWeaponAttributes*, TSharedPtr<UMyCombatantAttributeSet>&);
};