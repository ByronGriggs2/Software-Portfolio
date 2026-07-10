#pragma once
#include "CoreMinimal.h"
#include "MyAttributeSet.h"
#include "ProjectileAttributes.h"
class UMyCombatantAttributeSet;

class UMyProjectileAttributeSet : public UMyAttributeSet<FProjectileAttributes>
{
protected:
	virtual void updateFromModifiers() override;

public:
	UMyProjectileAttributeSet() = default;
	void initialise_UMyProjectileAttributeSet(const FProjectileAttributes*, TSharedPtr<UMyCombatantAttributeSet>&);
};