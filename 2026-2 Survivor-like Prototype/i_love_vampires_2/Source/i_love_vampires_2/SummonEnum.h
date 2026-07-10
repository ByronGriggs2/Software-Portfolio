#pragma once
#include "CoreMinimal.h"
#include "SummonEnum.generated.h"

UENUM(BlueprintType)
enum class ESummonTargeting : uint8 {
	nearestEnemy UMETA(DisplayName = "Nearest Enemy"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies
// all - AChasingSummon.h
UENUM(BlueprintType)
enum class ESummonType : uint8 {
	chasing UMETA(DisplayName = "Chasing"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies
// all - SummonFactory