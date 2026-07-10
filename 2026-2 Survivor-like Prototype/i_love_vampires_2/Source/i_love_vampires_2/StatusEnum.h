#pragma once
#include "CoreMinimal.h"
#include "StatusEnum.generated.h"
struct FEffectStruct;

UENUM(BlueprintType)
enum class EStatus : uint8
{
	bleed UMETA(DisplayName = "bleed"),
	// Unable to fire projectile attacks
	blind UMETA(DisplayName = "blind"),
	// percent maxHP damage twice per second
	burn UMETA(DisplayName = "burn"),
	// Slows and reduces attack speed by magnitude%
	chill UMETA(DisplayName = "chill"),
	damage UMETA(DisplayName = "damage"),
	// Percent missing HP applied with burn
	decay UMETA(DisplayName = "decay"),
	// Instantaneous %missing HP damage
	decay_instant UMETA(DisplayName = "decay_instant"),
	// execute below percentage
	execute UMETA(DisplayName = "execute"),
	// flat damage
	friendlyFire UMETA(DisplayName = "friendlyFire"),
	// flat healing when enemy is hit
	healInstigator UMETA(DisplayName = "healInstigator"),
	instantDeath UMETA(DisplayName = "instantDeath"),
	// flat damage once per second, increases exponentially at 10% per tick
	poison UMETA(DisplayName = "poison"),
	// rooted disables a single function- ACombatant::moveTowardsDirection. Ideally all non-initialisation movement ordered by the actor itself
	// should be routed through this function.
	root UMETA(DisplayName = "rooted"),
	slow UMETA(DisplayName = "slowed"),
	contactDamageImmune UMETA(DisplayName = "contactDamageImmune"),
	randomNegativePersistent UMETA(DisplayName = "randomNegativePersistent"),

	INVALID = 255 UMETA(DisplayName = "INVALID")
};

namespace EStatusHelpers {
	inline bool canBeMultiplied(EStatus status) {
		return
			status == EStatus::bleed ||
			status == EStatus::burn ||
			status == EStatus::damage ||
			status == EStatus::decay ||
			status == EStatus::decay_instant ||
			status == EStatus::friendlyFire ||
			status == EStatus::poison;
	}
}

// Dependencies
// bleed, burn, damage, decay, decay_instant, execute, instantDeath -> Combatant.h->CombatantAttributes
// damage, burn -> CombatantAttributeSet
// damage, bleed, rooted, contactDamageImmune -> ACombatant
// burn, poison -> BaseAttributeSet.h
// healInstigator, randomNegativePersistent, friendlyFire -> AttackActor.h
// blind -> ProjectileFactory.h
// chill -> Active.h, CombatantAttributes
// rooted, contactDamageImmune -> ForcedMovement
// bleed, burn, poison, blind, chill, decay, slow, root -> see below

class StatusStatics {
public:
	StatusStatics() = delete;
	StatusStatics(const StatusStatics& other) = delete;
	StatusStatics(StatusStatics&& other) = delete;
	StatusStatics& operator=(const StatusStatics& other) = delete;
	StatusStatics& operator=(StatusStatics&& other) = delete;

	static FEffectStruct getRandomNegativePersistentEffect(float magnitude, float duration);
};