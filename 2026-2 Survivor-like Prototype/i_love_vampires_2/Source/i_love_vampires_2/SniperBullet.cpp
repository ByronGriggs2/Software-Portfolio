#include "SniperBullet.h"
#include "MyGameplayStatics.h"
#include "EnemyBase.h"
#include "CombatantManager.h"

void ASniperBullet::BeginPlay() {
	Super::BeginPlay();
	UCombatantManager* manager = nullptr;
	if (!MyGameplayStatics::getCombatantManager(this, manager)) {
		LOGERROR("ASniperBullet::BeginPlay - failed to get combatant manager");
		return;
	}
	AEnemyBase* target = manager->getHighestHPEnemy();
	if (!IsValid(target)) {
		bulletDeath();
		return;
	}
	setTarget(target);
}

void ASniperBullet::Tick(float DeltaTime) {
	const APawn* casted = Cast<APawn>(getTarget());
	if (!IsValid(casted) && IsValid(getTarget())) {
		LOGERROR("ASniperBullet::Tick - target is not a pawn");
		bulletDeath();
		return;
	}
	if (!IsValid(casted) || _effectedPawns.Contains(casted)) {
		bulletDeath();
		return;
	}
	Super::Tick(DeltaTime);
}