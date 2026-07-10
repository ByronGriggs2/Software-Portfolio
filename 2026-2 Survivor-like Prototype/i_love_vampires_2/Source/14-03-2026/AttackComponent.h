#pragma once
#include "CoreMinimal.h"
#include "BaseAttributeComponent.h"
#include "AttackAttributeComponent.generated.h"
class UAttackAttributes;
class UProjectileAttributes;
class UAOEAttributes;
class UExplosiveProjectileAttributes;

UCLASS()
class I_LOVE_VAMPIRES_2_API UAttackAttributeComponent : public UBaseAttributeComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UAttackAttributes* _base;
	UPROPERTY()
	UAttackAttributes* _final;
	UPROPERTY()
	UAttackAttributes* _offsets;
};

UCLASS()
class I_LOVE_VAMPIRES_2_API UProjectileAttributeComponent : public UAttackAttributeComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UProjectileAttributes* _base;
	UPROPERTY()
	UProjectileAttributes* _final;
	UPROPERTY()
	UProjectileAttributes* _offsets;
};

UCLASS()
class I_LOVE_VAMPIRES_2_API UExplosiveProjectileAttributeComponent : public UProjectileAttributeComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UExplosiveProjectileAttributes* _base;
	UPROPERTY()
	UExplosiveProjectileAttributes* _final;
	UPROPERTY()
	UExplosiveProjectileAttributes* _offsets;
};

UCLASS()
class I_LOVE_VAMPIRES_2_API UAOEAttributeComponent : public UAttackAttributeComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UAOEAttributes* _base;
	UPROPERTY()
	UAOEAttributes* _final;
	UPROPERTY()
	UAOEAttributes* _offsets;
};