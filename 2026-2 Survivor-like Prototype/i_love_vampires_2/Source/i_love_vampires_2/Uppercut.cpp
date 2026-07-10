#include "Uppercut.h"
#include "SingleTarget.h"
#include "unrealHelpers.h"

void UppercutFactory::launchAttack(const FVector& forward) {
	AEnemyBase* target = launchAttack_internal(forward);
	if (!IsValid(target))
		return;
	const auto startPos = _owner->GetActorLocation();
	const auto endPos = startPos + forward * _BASE_MOVEMENT_DISTANCE;
	const auto radius = _owner->GetActorScale3D().X * SPRITE_RADIUS;
	const auto collider = FCollisionShape::MakeSphere(radius);
	const auto collisionType = _owner->isFriendly() ? _FRIENDLY_ATTACK : _ENEMY_ATTACK;
	
	auto onCollision = [this, endPos, startPos](ACombatant* self, ACombatant* other) {
		auto moveRef = move.lock();
		if (!moveRef)
			return;
		_collateralFactory.launchAttack(other, forward);
		const auto mainDirection = moveRef->getDelta(1.0f).GetSafeNormal();
		const auto mainAngle = unrealHelpers::getYaw(endPos-startPos);
		const auto mod = mainAngle < 0 ? -(2 * PI) / 3 : (2 * PI) / 3;
		const auto otherDirection = mainAngle < 0 ? unrealHelpers::makeFromYaw(mainAngle + mod);
		_collateralMovements.emplace_back(other, otherDirection, _COLLATERAL_MULTIPLIER * _MOVEMENT_DURATION);
		};
	_movements.emplace_back(target, endPos, MOVEMENT_DURATION, _owner, collider, collisionType, onCollision);
}