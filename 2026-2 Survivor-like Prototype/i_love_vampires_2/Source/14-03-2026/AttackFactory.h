#pragma once
#include "CoreMinimal.h"
#include "BaseAttributeSet.h"
#include "AttackFactory.generated.h"
class AAttackActor;
class AProjectile;
class AAOE;
class AExplosiveProjectile;

class UAttackComponent;
class UProjectileComponent;
class UAOEComponent;
class UExplosiveProjectileComponent;

class UAttackConfig;
class UProjectileConfig;
class UAOEConfig;
class UExplosiveProjectileConfig;

class APawn;

UCLASS()
class I_LOVE_VAMPIRES_2_API UAttackFactory : public UBaseAttributeSet
{
	GENERATED_BODY()

	UPROPERTY()
	UAttackConfig* _attackConfig;
	UPROPERTY()
	UAttackComponent* _attackComponent = nullptr;

	void shouldNotRunError() const;
	
protected:
	TWeakObjectPtr<APawn> _pawnRef;

	virtual void initAttack(AAttackActor*) const;
	virtual void initProjectile(AProjectile*) const { shouldNotRunError(); }
	virtual void initAOE(AAOE*) const { shouldNotRunError(); }
	virtual void initExplosiveProjectile(AExplosiveProjectile*) const { shouldNotRunError(); }

public:
	virtual void launchAttack();
};

UCLASS()
class I_LOVE_VAMPIRES_2_API UProjectileFactory : public UAttackFactory
{
	GENERATED_BODY()

	UPROPERTY()
	UProjectileConfig* _projectileConfig;
	UPROPERTY()
	UProjectileComponent* _projectileComponent = nullptr;

	float _directionX = 0;
	float _directionZ = 1;

protected:
	virtual void initProjectile(AProjectile*) const override;

	float getDirectionX() const { return _directionX; }
	float getDirectionZ() const { return _directionZ; }

public:
};

UCLASS()
class I_LOVE_VAMPIRES_2_API UAOEFactory : public UAttackFactory
{
	GENERATED_BODY()

	UPROPERTY()
	UAOEConfig* _aoeConfig;
	UPROPERTY()
	UAOEComponent* _aoeComponent = nullptr;

protected:
	virtual void initAOE(AAOE*) const override;

public:
};

UCLASS()
class I_LOVE_VAMPIRES_2_API UExplosiveProjectileFactory : public UProjectileFactory
{
	GENERATED_BODY()

	UPROPERTY()
	UExplosiveProjectileConfig* _explosiveProjectileConfig = nullptr;
	UPROPERTY()
	UExplosiveProjectileComponent* _explosiveProjectileComponent = nullptr;
	UPROPERTY()
	UAOEConfig* _aoeConfig = nullptr;
	UPROPERTY()
	UAOEComponent* _aoeComponent = nullptr;

protected:
	virtual void initExplosiveProjectile(AExplosiveProjectile*) const override;
	virtual void initAOE(AAOE*) const override;

public:
};