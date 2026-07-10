#include "Backstab.h"
#include "Combatant.h"
#include "Definitions.h"
#include "unrealHelpers.h"
#include <cmath>
// for PI
#include "Math/UnrealMathUtility.h"

void ABackstab::applyEffect(ACombatant* target) {
	if (isBehindTarget(target))
		getNextHitModifiersRef()._powerMultiplier *= 2;
	Super::applyEffect(target);
}

bool ABackstab::isBehindTarget(const ACombatant* target) const {
	if (!IsValid(target)) {
		LOGERROR("ABackstab::isBehindTarget - target is not valid");
		return false;
	}
	const float myAngle = unrealHelpers::getYaw(GetActorForwardVector());
	const float targetAngle = unrealHelpers::getYaw(target->GetActorForwardVector());
	return std::abs(myAngle - targetAngle) <= PI / 2;
}

void ABackstab::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(_AOEConfig.Get())) {
		LOGERROR("ABackstab::BeginPlay - invalid AOEConfig");
		return;
	}
	if (_AOEConfig->_targeting != _BACKSTAB || _AOEConfig->_shape != _ARC) {
		LOGERROR("ABackstab::BeginPlay - invalid AOEConfig for backstab");
		return;
	}
}