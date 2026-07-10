#include "FriendlyCombatant.h"
#include "EnemyBase.h"
#include "unrealHelpers.h"
#include "PaperFlipbookComponent.h"

void AFriendlyCombatant::Tick(float delta) {
	TRACE_CPUPROFILER_EVENT_SCOPE(AFriendlyCombatant_Tick);
	Super::Tick(delta);
	if (!IsValid(this))
		return;
	TSet<AActor*> overlaps;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(AFriendlyCombatant_GetOverlaps);
		getOverlappingActors(overlaps);
	}
	for (auto& result : overlaps) {
		onOverlapBegin(this, result);
	}
}

void AFriendlyCombatant::onOverlapBegin(AActor* me, AActor* other) {
	if (!IsValid(me)) {
		LOGERROR("AMyPlayer::onOverlapBegin - self actor is invalid");
		return;
	}
	if (!IsValid(other))
		//other actor is being constructed or destructed
		return;
	onOverlapBegin_Impl(me, other);
}

bool AFriendlyCombatant::onOverlapBegin_Impl(AActor* me, AActor* other) {
	AEnemyBase* enemy = Cast<AEnemyBase>(other);
	if (IsValid(enemy)) {
		handleEnemyCollision(enemy);
		return true;
	}
	return false;
}

void AFriendlyCombatant::handleEnemyCollision(AEnemyBase* other) {
	exchangeContactDamage(this, other);
}