#pragma once
#include "CoreMinimal.h"
#include "StatusEnum.h"
class ACombatant;

class ForcedMovement {
	const static inline EStatus _ROOT = EStatus::root;
	const static inline EStatus _CONTACT_IMMUNE = EStatus::contactDamageImmune;

	TWeakObjectPtr<ACombatant> _target;
	FVector _endPosition;
	float _duration;
	float _elapsedTime;
	int _rootUID;
	int _contactImmuneUID;
	FVector _cachedDelta;
	bool _done = false;

protected:
	const ACombatant* getTarget() const { return _target.IsValid() ? _target.Get() : nullptr; }
	ACombatant* getTargetRef() const { return _target.IsValid() ? _target.Get() : nullptr; }
	FVector getDelta(float time) const { return _cachedDelta * time; }
	void finish();

public:
	ForcedMovement() = delete;
	ForcedMovement(ACombatant* target, const FVector& endPosition, float duration);
	ACombatant* getTarget() const { return _target.Get(); }

	// must be called by the owner of this object. Returns true if this object wants to die.
	virtual bool tick(float delta);
};