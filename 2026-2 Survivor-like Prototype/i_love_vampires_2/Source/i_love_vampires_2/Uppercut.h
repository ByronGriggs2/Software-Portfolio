#pragma once
#include "CoreMinimal.h"
#include "SingleTarget.h"
#include "CollidingForcedMovmement.h"
#include "ForcedMovement.h"
#include <vector>
#include "CustomChannelEnums.h"

class UppercutFactory : public SingleTargetFactory {
	const static inline float _MOVEMENT_DURATION = 1.0f;
	const static inline float _BASE_MOVEMENT_DISTANCE = 100.0f;
	const static inline float _COLLATERAL_MULTIPLIER = 0.333f;
	const static inline collisionType _FRIENDLY_ATTACK = collisionType::friendlyAttack;
	const static inline collisionType _ENEMY_ATTACK = collisionType::enemyAttack;

	std::vector<std::shared_ptr<CollidingForcedMovement>> _movements;
	std::vector<ForcedMovement> _collateralMovements;
	SingleTargetFactory _collateralFactory;

public:
	UppercutFactory(ACombatant* owner, const UAttackTemplate* temp) : SingleTargetFactory(owner, temp), _collateralFactory(owner, temp) {}
	virtual void launchAttack(const FVector& forward) override;
	virtual void tick(float delta) override;
};