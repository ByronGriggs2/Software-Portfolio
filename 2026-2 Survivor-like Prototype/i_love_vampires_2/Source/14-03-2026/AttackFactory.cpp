#include "AttackFactory.h"
#include "Definitions.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

#include "AttackActor.h"
#include "Projectile.h"
#include "AOE.h"
#include "ExplosiveProjectile.h"

#include "AttackAttributeComponent.h"
#include "ProjectileAttributeComponent.h"
#include "AOEAttributeComponent.h"
#include "ExplosiveProjectileAttributeComponent.h"

#include "AttackConfig.h"
#include "ProjectileConfig.h"
#include "AOEConfig.h"
#include "ExplosiveProjectileConfig.h"



void UAttackFactory::shouldNotRunError() const {
	LOGERROR("UAttackFactory::shouldNotRunError - this function should not run");
}

void UAttackFactory::initAttack(AAttackActor* cdo) const {
	if (!IsValid(cdo) || !IsValid(_attackComponent || !IsValid(_attackConfig)) {
		LOGERROR("UAttackFactory::initCDO - invalid");
		return;
	}
	cdo->initialise_AAttackActor(_pawnRef.Get(), _attackConfig, _attackComponent._final);
}

void UAttackFactory::launchAttack() {
	if (!IsValid(_attackConfig)) {
		LOGERROR("UAttackFactory::launchAttack - attackConfig is not valid");
		return;
	}
	const TSubclassOf<AAttackActor> attackClass = _attackConfig->_attackClass;
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AAttackActor* attack = GetWorld()->SpawnActor<AAttackActor>(attackClass, params);
	if (!IsValid(attack)) {
		LOGERROR("UAttackFactory::launchAttack - failed to spawn attack");
		return;
	}
	attack->factoryInitQuery(this);
}

void UProjectileFactory::initProjectile(AProjectile* cdo) const {
	if (!IsValid(cdo) || !IsValid(_projectileComponent) || !IsValid(_projectileConfig)) {
		LOGERROR("UProjectileFactory::initCDO - invalid");
		return;
	}
	cdo->initialise_AProjectile(
		_pawnRef.Get(), 
		_directionX, 
		_directionZ, 
		_attackConfig, 
		_attackComponent._final, 
		_projectileConfig, 
		_projectileComponent._final);
}

void UAOEFactory::initAOE(AAOE* cdo) const {
	if (!IsValid(cdo) || !IsValid(_aoeComponent) || !IsValid(_aoeConfig)) {
		LOGERROR("UAOEFactory::initCDO - invalid");
		return;
	}
	cdo->initialise_AAOE(
		_pawnRef.Get(), 
		_attackConfig, 
		_attackComponent._final, 
		_aoeConfig, 
		_aoeComponent._final);
}

void UExplosiveProjectileFactory::initExplosiveProjectile(AExplosiveProjectile* cdo) const {
	if (!IsValid(cdo) || !IsValid(_explosiveProjectileComponent) || !IsValid(_explosiveProjectileConfig)) {
		LOGERROR("UExplosiveProjectileFactory::initCDO - invalid");
		return;
	}
	cdo->initialise_AExplosiveProjectile(
		_pawnRef.Get(),
		_attackConfig,
		_attackComponent._final,
		_projectileConfig,
		_projectileComponent._final,
		_explosiveProjectileConfig, 
		_explosiveProjectileComponent._final);
}

void UExplosiveProjectileFactory::initAOE(AAOE* cdo) const {
	cdo->initialise_AAOE(
		_pawnRef.Get(), 
		_attackConfig, 
		_attackComponent._final, 
		_aoeConfig, 
		_aoeComponent._final);
}