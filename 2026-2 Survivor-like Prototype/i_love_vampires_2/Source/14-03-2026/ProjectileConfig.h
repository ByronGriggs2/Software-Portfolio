#pragma once
#include "CoreMinimal.h"
#include "BaseConfig.h"
#include "Projectile.h"
#include "ProjectileEnum.h"
#include "ProjectileConfig.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UProjectileConfig : public UBaseConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ProjectileConfig")
	EProjectileShape _shape = static_cast<EProjectileShape>(0);
	UPROPERTY(EditAnywhere, Category = "ProjectileConfig")
	EAttackShape _attackShape = static_cast<EAttackShape>(0);
	UPROPERTY(EditAnywhere, Category = "ProjectileConfig")
	EProjectileTargeting _targeting = static_cast<EProjectileTargeting>(0);
	UPROPERTY(EditAnywhere, Category = "ProjectileConfig")
	bool _isHoming = false;
};