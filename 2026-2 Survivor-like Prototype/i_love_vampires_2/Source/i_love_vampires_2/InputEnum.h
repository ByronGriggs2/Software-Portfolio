#pragma once
#include "CoreMinimal.h"
#include "InputEnum.generated.h"

// mirrors the enhanced input context
UENUM(BlueprintType)
enum class EInput_UI : uint8 {
	up UMETA(DisplayName = "Up"),
	down UMETA(DisplayName = "Down"),
	left UMETA(DisplayName = "Left"),
	right UMETA(DisplayName = "Right"),
	confirm UMETA(DisplayName = "Confirm"),
	cancel UMETA(DisplayName = "Cancel"),
	num UMETA(Hidden),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};

UENUM(BlueprintType)
enum class EInput_Action : uint8 {
	move UMETA(DisplayName = "Move"),
	num UMETA(Hidden),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};