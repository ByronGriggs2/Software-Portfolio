#pragma once
#include "CoreMinimal.h"
#include "ForcedMovement.h"
#include "CollisionShape.h"
#include <functional>
#include "CustomChannelEnums.h"

class CollidingForcedMovement : public ForcedMovement {
	TWeakObjectPtr<AActor> _guarantor;
	FCollisionShape _collider;
	collisionType _collisionChannel;
	std::function<void(ACombatant*, ACombatant*)> _onCollision;
	TArray<AActor*> _ignoredActors;
	
public:
	CollidingForcedMovement(
		ACombatant* target,
		const FVector& endPosition,
		float duration,
		TWeakObjectPtr<AActor> guarantor,
		const FCollisionShape& collider,
		collisionType collisionChannel,
		std::function<void(ACombatant*, ACombatant*)> callback)
		:
		ForcedMovement(target, endPosition, duration),
		_guarantor(guarantor),
		_collider(collider),
		_collisionChannel(collisionChannel),
		_onCollision(callback),
		_ignoredActors({target})
	{}
	virtual bool tick(float delta) override;
};