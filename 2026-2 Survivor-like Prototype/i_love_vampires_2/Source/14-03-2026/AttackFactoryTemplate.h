#pragma once
#include "CoreMinimal.h"
#include "BaseAttributeActorTemplate.h"
#include "AttackFactoryTemplate.generated.h"

class APawn;

class UAttackConfig;
class UAttackAttributes;
class UAttackFactory;

class UProjectileConfig;
class UProjectileAttributes;

class UAOEConfig;
class UAOEAttributes;

class UExplosiveProjectileConfig;
class UExplosiveProjectileAttributes;


UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UAttackFactoryTemplate : public UBaseAttributeActorTemplate {
	GENERATED_BODY()

protected:
	virtual bool isValid() const;

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "UAttackFactoryTemplate")
	UAttackConfig* _attackConfig;
	UPROPERTY(EditAnywhere, Instanced, Category = "UAttackFactoryTemplate")
	UAttackAttributes* _attackAttributes;

	virtual UAttackFactory* createFactory(APawn*) const;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UProjectileFactoryTemplate : public UAttackFactoryTemplate {
	GENERATED_BODY()

protected:
	virtual bool isValid() const override;

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "UProjectileFactoryTemplate")
	UProjectleConfig* _projectileConfig;
	UPROPERTY(EditAnywhere, Instanced, Category = "UProjectileFactoryTemplate")
	UProjectileAttributes* _projectileAttributes;

	virtual UAttackFactory* createFactory(APawn*) const override;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UAOEFactoryTemplate : public UAttackFactoryTemplate {
	GENERATED_BODY()

protected:
	virtual bool isValid() const override;

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "UProjectileFactoryTemplate")
	UAOEConfig* _AOEConfig;
	UPROPERTY(EditAnywhere, Instanced, Category = "UProjectileFactoryTemplate")
	UProjectileAttributes* _AOEAttributes;

	virtual UAttackFactory* createFactory(APawn*) const override;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UExplosiveProjectileFactoryTemplate : public UProjectileFactoryTemplate {
	GENERATED_BODY()

protected:
	virtual bool isValid() const override;

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "UExplosiveProjectileFactoryTemplate")
	UExplosiveProjectileConfig* _explosiveProjectileConfig;
	UPROPERTY(EditAnywhere, Instanced, Category = "UExplosiveProjectileFactoryTemplate")
	UExplosiveProjectileAttributes* _explosiveProjectileAttributes;

	virtual UAttackFactory* createFactory(APawn*) const override;
};