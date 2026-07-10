#pragma once

#include "CoreMinimal.h"
#include "CombatantAttributeSetUEnum.generated.h"

UENUM(BlueprintType)
enum class ECombatantAttribute : uint8 {
	maxHP UMETA(DisplayName = "maxHP"),
	currentHP UMETA(DisplayName = "currentHP"),
	damageReduction_flat UMETA(DisplayName = "damageReduction_flat"),
	damageReduction_percent UMETA(DisplayName = "damageReduction_percent"),
	healthRegen_flat UMETA(DisplayName = "healthRegen_flat"),
	healthRegen_percent UMETA(DisplayName = "healthRegen_percent"),
	critChance UMETA(DisplayName = "critChance"),
	critMultiplier UMETA(DisplayName = "critMultiplier"),
	attackSpeed UMETA(DisplayName = "attackSpeed"),
	bonusBounces UMETA(DisplayName = "bonusBounces"),
	bonusPierce UMETA(DisplayName = "bonusPierce"),
	bonusProjectiles UMETA(DisplayName = "bonusProjectiles"),
	projectileSpeed UMETA(DisplayName = "projectileSpeed"),
	projectileSize UMETA(DisplayName = "projectileSize"),
	movementSpeed UMETA(DisplayName = "movementSpeed"),
	range UMETA(DisplayName = "range"),
	contactDamage UMETA(DisplayName = "contactDamage"),
	selfSize UMETA(DisplayName = "selfSize"),
	iFrameDuration UMETA(DisplayName = "iFrameDuration")
};