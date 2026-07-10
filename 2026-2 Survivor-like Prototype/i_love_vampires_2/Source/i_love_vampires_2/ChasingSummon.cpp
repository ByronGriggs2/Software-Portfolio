#include "ChasingSummon.h"
#include "MyGameplayStatics.h"
#include "CombatantManager.h"
#include "unrealHelpers.h"
#include "Active.h"
#include "EnemyBase.h"

void AChasingSummon::giveWeapon_safe(const UWeaponTemplate* data, int socket) {
	Super::giveWeapon_safe(data, socket);
	getAllActives().back()->setActivationTrigger(_AUTOCHARGE);
}

void AChasingSummon::Tick_Impl(float delta) {
	TRACE_CPUPROFILER_EVENT_SCOPE(AChasingSummon_Tick_Impl);
	auto myTemplate = getTemplate();
	if (!IsValid(myTemplate) || !IsValid(myTemplate->_summonConfig)) {
		LOGERROR("AChasingSummon::Tick - invalid template");
		return;
	}

	

	// TODO: ignores sprite size when calculating if enemy is in range
	if (myTemplate->_summonConfig->_targeting == _NEAREST) {
		const FVector myLocation = GetActorLocation();
		FVector targetLocation;
		// possible if there are no enemies
		if (!getNearestEnemyLocation(targetLocation)) {
			return;
		}
		const FVector direction = (targetLocation - myLocation).GetSafeNormal();
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(AChasingSummon_rotate);
			lookAtDirection(direction.X, direction.Z);
		}

		const int rangeType = isInRange(myTemplate->_summonConfig->_targeting, targetLocation, myLocation);
		if (rangeType != 1){
			for (auto& active : getAllActives()) {
				if (active->isReady()) {
					active->activate(myGetForwardVector());
				}
			}
		}
		
		if (rangeType == -1) {
			TRACE_CPUPROFILER_EVENT_SCOPE(AChasingSummon_move_0);
			moveTowardsDirection(-direction, delta);
		}
		else
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(AChasingSummon_move_1);
			moveTowardsDirection(direction, delta);
		}
	}
	else {
		LOGERROR("AChasingSummon::Tick - targeting type not implemented");
		return;
	}
}

// -1 means you're too close
// 0 means you're in range but far
// 1 means you're out of range
int AChasingSummon::isInRange(ESummonTargeting targetingType, const FVector& targetLocation, const FVector& myLocation) const {
	const FVector direction = (targetLocation - myLocation).GetSafeNormal();
	const float distance = unrealHelpers::getDistance(getSpriteBoundary(direction), targetLocation);
	float minimumAttackRange = 9999;
	for (const auto& attack : getAllActives()) {
		if (attack->isAOE()) {
			float tempRange;
			if (!attack->getAOERange(tempRange)) {
				LOGERROR("AChasingSummon::Tick - failed to get AOE range");
				continue;
			}
			if (tempRange < minimumAttackRange)
				minimumAttackRange = tempRange;
		}
	}
	const float myRange = getAttributeMember(&CombatantAttributes::_range);
	const float actualRange = minimumAttackRange * myRange * SPRITE_RADIUS;
	if (distance < 0.5 * actualRange)
		return -1;
	else if (distance < actualRange)
		return 0;
	else
		return 1;
}

bool AChasingSummon::getNearestEnemyLocation(FVector& ret) const {
	UCombatantManager* manager = nullptr;
	if (!MyGameplayStatics::getCombatantManager(this, manager)) {
		LOGERROR("AChasingSummon::Tick - failed to get combatant manager");
		return false;
	}
	auto enemy = manager->getNearestEnemyPtr(this);
	if (!IsValid(enemy)) {
		return false;
	}
	ret = enemy->GetActorLocation();
	return true;
}