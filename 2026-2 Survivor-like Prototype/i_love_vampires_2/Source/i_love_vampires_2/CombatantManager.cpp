#include "CombatantManager.h"
#include "Combatant.h"
#include "Math/UnrealMathUtility.h"
#include "Definitions.h"
#include "Engine/World.h"
#include "CombatGameModeBase.h"
#include "EnemyBase.h"
#include "helpers.h"

void UCombatantManager::tick(float delta) {
	if (!_gameReady)
		return;
	_poisonTimer -= delta;
	if (_poisonTimer <= 0.0f) {
		_poisonThisFrame = true;
		_poisonTimer = _POISON_PERIOD;
		if (_ignoreNextBurn)
			_ignoreNextBurn = false;
		else {
			_ignoreNextBurn = true;
			_burnThisFrame = true;
		}
	}
	else {
		_burnThisFrame = false;
		_poisonThisFrame = false;
	}
}

int UCombatantManager::registerEnemy(AEnemyBase* enemy) {
	int key = _nextKey;
	_nextKey += 1;
	_enemyReferences.Add(key, TWeakObjectPtr<AEnemyBase>(enemy));
	return key;
}

void UCombatantManager::removeFromRegister(int key) {
	_enemyReferences.Remove(key);
}

AEnemyBase* UCombatantManager::getRandomEnemyPtr(const AEnemyBase* excluded) const {
	if (_enemyReferences.Num() <= 1) {
		return nullptr;
	}
	int offset = excluded == nullptr ? 0 : 1;
	int roll = FMath::RandRange(static_cast<int>(0), static_cast<int>(_enemyReferences.Num() - 1 - offset));
	// pair is TPair
	int count = 0;
	for (const auto& pair : _enemyReferences) {
		if (count == roll) {
			if (pair.Value.IsValid() && pair.Value.Get() != excluded) {
				return pair.Value.Get();
			}
			roll++;
		}
		count++;
	}
	// If all combatants are in the middle of construction or destruction, it could be valid for this function to fail without error
	return nullptr;
}

AEnemyBase* UCombatantManager::getNearestEnemyPtr(const AActor* caller) const {
	AEnemyBase* nearest = nullptr;
	float nearestDist = -1;
	for (const auto& pair : _enemyReferences) {
		if (pair.Value.IsValid()) {
			AEnemyBase* enemy = pair.Value.Get();
			float dist = FVector::Dist(caller->GetActorLocation(), enemy->GetActorLocation());
			if (helpers::nearEq(nearestDist, -1) || dist < nearestDist) {
				nearestDist = dist;
				nearest = enemy;
			}
		}
	}
	return nearest;
}

void UCombatantManager::setPlayerRef(ACombatant* playerRef) {
	_playerRef = TWeakObjectPtr<ACombatant>(playerRef);
}

AEnemyBase* UCombatantManager::getHighestHPEnemy() const {
	float highest = -1;
	AEnemyBase* current = nullptr;
	for (const auto& pair : _enemyReferences) {
		if (pair.Value.IsValid()) {
			AEnemyBase* enemy = pair.Value.Get();
			const float HP = enemy->getHP();
			if (HP > highest) {
				highest = HP;
				current = enemy;
			}
		}
	}
	return current;
}