#pragma once
#include "CoreMinimal.h"
#include "AoeShapeEnum.generated.h"

UENUM(BlueprintType)
enum class EAoeShape : uint8
{
	circle      UMETA(DisplayName = "circle")
};
// Dependencies
// circle
//     - AOE.h