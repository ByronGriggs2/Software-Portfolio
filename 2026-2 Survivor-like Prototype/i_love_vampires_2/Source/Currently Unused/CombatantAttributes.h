#pragma once

#include "CoreMinimal.h"
#include "CombatantAttributes.generated.h"

// additive and multiplicative indicates the effect this attribute should have on a corresponding attribute on a weapon, projectile, etc.
// there hopefully will not be any cases in which an attribute modifies two corresponding attributes differently, but if there is, I will
// specify per object.

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FCombatantAttributes
{
	GENERATED_USTRUCT_BODY()

public:
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _maxHP = 1;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _currentHP = 1;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _damageReduction_flat = 0;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _damageReduction_percent = 0;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _healthRegen_flat = 0;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _healthRegen_percent = 0;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _critChance = 0;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _critMultiplier = 0;
	//multiplicative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _attackSpeed = 1;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _bonusBounces = 0;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _bonusPierce = 0;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _bonusProjectiles = 0;
	//multiplicative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _projectileSpeed = 1;
	//multiplicative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _projectileSize = 1;
	//multiplicative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _movementSpeed = 1;
	//multiplicative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _range = 1;
	//additive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _contactDamage = 0;
	//multiplicative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _selfSize = 1;
	//multiplicative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	float _iFrameDuration = 1;
};