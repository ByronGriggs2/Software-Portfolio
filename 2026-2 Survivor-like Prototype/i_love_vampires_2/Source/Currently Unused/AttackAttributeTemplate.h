#pragma once
#include "CoreMinimal.h"
#include "BaseAttributeTemplate.h"
#include "AttackAttributeTemplate.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UAttackAttributeTemplate : public UBaseAttributeTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float _damage = 0.f;
	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float _critChance = 0.f;
	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float _critMultiplier = 2.f;

	virtual void modifyAttributes(const UCombatantAttributes* modifiers, const UAttackAttributes* baseAttributes, UAttackAttributes* finalAttributes);
};