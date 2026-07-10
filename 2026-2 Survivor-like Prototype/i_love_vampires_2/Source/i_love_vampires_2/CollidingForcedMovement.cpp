#include "ForcedMovement.h"
#include "unrealHelpers.h"
#include "Definitions.h"

bool CollidingForcedMovement::tick(float delta) {
	const auto actor = getTarget();
	if (!IsValid(actor))
		return ForcedMovement::tick(delta);
	if (!_guarantor.IsValid()) {
		ForcedMovement::tick(delta);
		finish();
		return true;
	}
	const auto startPos = actor->GetActorLocation();
	const auto endPos = startPos + getDelta(delta);
	TArray<FHitResult> hits;
	if (!unrealHelpers::performSweep(actor, startPos, endPos, _collider, hits, _ignoredActors)) {
		LOGERROR("CollidingForcedMovement::tick - failed to perform sweep");
		return ForcedMovement::tick(delta);
	}
	for (const auto& hit : hits) {
		auto otherActor = Cast<ACombatant>(hit.GetActor());
		if (otherActor.IsValid()) {
			_onCollision(getTargetRef(), otherActor.Get());
			_ignoredActors.Add(otherActor.Get());
		}
	}
	return ForcedMovement::tick(delta);
}