#include "Suplex.h"
#include "MyGameplayStatics.h"
#include "CombatantManager.h"
#include "EnemyBase.h"
#include "ForcedMovement.h"
#include "Math/UnrealMathUtility.h"
#include "unrealHelpers.h"
#include "BaseAttributes.h"
#include "AttackActor.h"

TArray<AAOE*> SuplexFactory::launchAttack_Impl(const FVector& forward) {
	AEnemyBase* target = nullptr;
	if (!myGetTarget(target))
		return {};
	const auto ownerPos = _owner->GetActorLocation();
	auto vect = target->GetActorLocation() - ownerPos;
	// opposite direction, same magnitude
	vect *= -1;
	const auto newPos = ownerPos + vect;
	_movements.emplace_back(target, newPos, _DURATION);
	return {};
}

bool SuplexFactory::myGetTarget(AEnemyBase*& ret) const {
	if (!_owner.IsValid())
		return false;
	UCombatantManager* manager = nullptr;
	if (!MyGameplayStatics::getCombatantManager(_owner.Get(), manager)) {
		LOGERROR("SuplexFactory::MyGetTarget - failed to get combatant manager");
		return false;
	}
	ret = manager->getNearestEnemyPtr(_owner.Get());
	if (!IsValid(ret))
		return false;
	return true;
}

void SuplexFactory::tick(float delta){
	AOEFactory::tick(delta);
	for (auto it = _movements.begin(); it != _movements.end();) {
		if (it->tick(delta)) {
			onSlam(Cast<AEnemyBase>(it->getTarget()));
			it = _movements.erase(it);
		}
		else {
			++it;
		}
	}
}

SuplexFactory::SuplexFactory(ACombatant* owner, const USuplexTemplate* temp) : AOEFactory(owner, temp) {}

void SuplexFactory::onSlam(AEnemyBase* target) {
	if (!IsValid(target) || !_owner.IsValid())
		return;
	const auto damage = getMember(&AttackAttributes::_damage);
	const auto critChance = getMember(&AttackAttributes::_critChance);
	const auto critMultiplier = 1 + getMember(&AttackAttributes::_critMultiplier);
	const auto maxHP = target->getMaxHP();
	const auto secondaryDamage = damage * _SPLASH_DAMAGE_MULTIPLIER + maxHP * _MAXHP_DAMAGE_MULTIPLIER;
	bool isCrit = FMath::FRandRange<float>(0, 1) < critChance;

	if (isCrit)
		target->inflictStatus(FEffectStruct{ EStatus::damage, damage * critMultiplier, 0, 1 });
	else
		target->inflictStatus(FEffectStruct{ EStatus::damage, damage, 0, 1 });

	// This block uses the convoluted interface provided by AOEFactory. It creates a temporary, alternative AOEInitStruct, then passes it to
	// AOEFactory's over*load* of BaseAttackFactory's virtual launchAttack function. Calling launchAttack here instead of launchAttack_Impl (which is 
	// also legal) allows AOEFactory to add its extra functionality on top.
	AOEInitStruct aoeInit = getAOEInit();
	StatAttourney::setFinal(AttackInitStructAttourney::getAttackAttributes(aoeInit._attack)._damage, secondaryDamage);
	if (isCrit) {
		StatAttourney::setFinal(AttackInitStructAttourney::getAttackAttributes(aoeInit._attack)._critChance, 1);
		StatAttourney::setFinal(AttackInitStructAttourney::getAttackAttributes(aoeInit._attack)._critMultiplier, critMultiplier-1);
	}
	else {
		StatAttourney::setFinal(AttackInitStructAttourney::getAttackAttributes(aoeInit._attack)._critChance, 0);
	}
	AOEFactory::launchAttack(FVector(1, 0, 0), &aoeInit);
}

bool SuplexFactory::canFire() const {
	AEnemyBase* target = nullptr;
	if (!myGetTarget(target))
		return false;
	const auto distance = unrealHelpers::getDistance(_owner->GetActorLocation(), target->GetActorLocation());
	const auto range = _owner->getAttributeMember(&CombatantAttributes::_range) * _RANGE;
	if (distance >= range)
		return false;
	return true;
}