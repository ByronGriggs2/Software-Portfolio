#pragma once
#include "CoreMinimal.h"
#include "CombatantUEnum.generated.h"

UENUM(BlueprintType)
enum class EStatus : uint8
{
	damage UMETA(DisplayName = "Damage"),
	bleed UMETA(DisplayName = "Bleed"),
	burn UMETA(DisplayName = "Burn")
};