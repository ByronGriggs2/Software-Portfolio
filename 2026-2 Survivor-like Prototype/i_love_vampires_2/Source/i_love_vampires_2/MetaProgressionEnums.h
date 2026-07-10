#pragma once
#include "CoreMinimal.h"
#include "MetaProgressionEnums.generated.h"

UENUM(BlueprintType)
enum class ETechnologyState : uint8 {
	hidden UMETA(DisplayName = "hidden"),
	locked UMETA(DisplayName = "locked"),
	unlockable UMETA(DisplayName = "unlockable"),
	unlocked UMETA(DisplayName = "unlocked"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};