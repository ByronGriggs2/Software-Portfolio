#pragma once

#include "CoreMinimal.h"
#include "ProjectileEnum.h"
#include "WeaponConfig.h"
#include "ProjectileConfig.generated.h"

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FProjectileConfig : public FWeaponConfig
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileConfig")
	EProjectileShape _shape = static_cast<EProjectileShape>(0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileConfig")
	EAttackShape _attackShape = static_cast<EAttackShape>(0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileConfig")
	EProjectileTargeting _targeting = static_cast<EProjectileTargeting>(0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileConfig")
	bool _isHoming = false;
};
