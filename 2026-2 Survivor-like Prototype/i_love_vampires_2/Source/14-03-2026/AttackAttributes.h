#pragma once
#include "CoreMinimal.h"
#include "BaseAttributes.h"
#include "AttackAttributes.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UAttackAttributes : public UBaseAttributes
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float _damage = 0.f;
	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float _critChance = 0.f;
	UPROPERTY(EditAnywhere, Category = "WeaponAttributes")
	float _critMultiplier = 2.f;
};