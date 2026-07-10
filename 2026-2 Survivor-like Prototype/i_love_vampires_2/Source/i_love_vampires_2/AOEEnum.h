#pragma once
#include "CoreMinimal.h"
#include "AOEEnum.generated.h"

UENUM(BlueprintType)
enum class EAOEShape : uint8
{
	circle UMETA(DisplayName = "circle"),
	arc UMETA(DisplayName = "arc"),
	line UMETA(DisplayName = "line"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};

// Dependencies
// arc - ABackstab, AOEFactory
// all - AAOE

UENUM(BlueprintType)
enum class EAOETargeting : uint8
{
	instigator UMETA(DisplayName = "instigator"),
	followInstigator UMETA(DisplayName = "follow instigator"),
	random UMETA(DisplayName = "random"),
	spawner_line UMETA(DisplayName = "spawner line"),
	trail UMETA(DisplayName = "trail"),
	backstab UMETA(DisplayName = "backstab"),
	//arc_homing_stationary UMETA(DisplayName = "homing stationary"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
namespace AOEEnumSpace {
	inline bool instigatorSpawn(EAOETargeting target) {
		return target == EAOETargeting::instigator || target == EAOETargeting::followInstigator;
	}
	inline bool followsAim(EAOEShape shape, EAOETargeting target) {
		return target == EAOETargeting::backstab || (target == EAOETargeting::instigator && (shape == EAOEShape::arc || shape == EAOEShape::line));
	}
}
// Dependencies
// random - AOEFactory
// all spawner - AAOESpawner
// trail - AOETemplate
// backstab - ABackstab, AOEFactory
// followInstigator - AAOE
namespace AOEEnumHelpers {
	inline bool isSpawner(EAOETargeting in) {
		return in == EAOETargeting::spawner_line;
	}
}

UENUM()
enum class EEffectApplication : uint8
{
	once UMETA(DisplayName = "once"),
	tick UMETA(DisplayName = "tick"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies
// tick, once - AAOE