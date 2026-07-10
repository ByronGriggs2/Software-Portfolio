#include "ForcedMovement.h"
#include "Combatant.h"

ForcedMovement::ForcedMovement(ACombatant* target, const FVector& endPosition, float duration) : _target(target), _endPosition(endPosition), _duration(duration), _elapsedTime(0) {
	if (!_target.IsValid())
		return;
	const auto startPos = _target->GetActorLocation();
	_cachedDelta = (_endPosition - startPos) / _duration;
	_rootUID = _target->inflictPermanentStatus(FEffectStruct(_ROOT, 1, 999, 1));
	_contactImmuneUID = _target->inflictPermanentStatus(FEffectStruct(_CONTACT_IMMUNE, 1, 999, 1));
}

bool ForcedMovement::tick(float delta) {
	if (!_target.IsValid())
		return true;
	if (_elapsedTime >= _duration) {
		if (_done)
			return true;
		finish();
	}
	_target->AddActorWorldOffset(_cachedDelta * delta, false, nullptr, ETeleportType::TeleportPhysics);
	_elapsedTime += delta;
	return false;
}

void ForcedMovement::finish() {
	_target->removePermanentStatus(_rootUID);
	_target->removePermanentStatus(_contactImmuneUID);
	_done = true;
}