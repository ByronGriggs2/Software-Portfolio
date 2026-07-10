#pragma once
#include "CoreMinimal.h"
#include "ProjectileEnums.generated.h"

UENUM(BlueprintType)
enum class EProjectileShape : uint8
{
	circle      UMETA(DisplayName = "circle")
};
// Dependencies
// Circle
//     - Bullet.h

UENUM(BlueprintType)
enum class EAttackShape : uint8
{
	fan      UMETA(DisplayName = "fan")
};

UENUM(BlueprintType)
enum class EProjectileTargeting : uint8
{
	skillshot      UMETA(DisplayName = "skillshot"),
	closest		   UMETA(DisplayName = "closest"),
	random		   UMETA(DisplayName = "random")
};