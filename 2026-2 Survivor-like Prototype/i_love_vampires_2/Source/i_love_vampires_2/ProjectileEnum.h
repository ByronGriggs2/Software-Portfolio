#pragma once
#include "CoreMinimal.h"
#include "ProjectileEnum.generated.h"

UENUM(BlueprintType)
enum class EProjectileShape : uint8
{
	circle UMETA(DisplayName = "circle"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
UENUM(BlueprintType)
enum class EAttackShape : uint8
{
	fan UMETA(DisplayName = "fan"),
	random UMETA(DisplayName = "random"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies
// all - ProjectileFactory

UENUM(BlueprintType)
enum class EProjectileTargeting : uint8
{
	skillshot      UMETA(DisplayName = "skillshot"),
	closest		   UMETA(DisplayName = "closest"),
	random		   UMETA(DisplayName = "random"),
	randomEnemy	   UMETA(DisplayName = "randomEnemy"),
	sniperRifle  UMETA(DisplayName = "sniperRifle"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies
// skillshot - Projectile.h
// random - Projectile.h
// sniperRifle - ProjectileFactory