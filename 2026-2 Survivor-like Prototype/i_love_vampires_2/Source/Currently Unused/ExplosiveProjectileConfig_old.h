#pragma once

#include "CoreMinimal.h"
#include "ProjectileConfig.h"
#include "AOEAttributes.h"
#include "AOEConfig.h"
#include "ExplosiveProjectileConfig.generated.h"

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FExplosiveProjectileConfig : public FProjectileConfig
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosiveProjectileConfig")
	FAOEConfig _explosionConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosiveProjectileConfig")
	FAOEAttributes _explosionAttributes;
};
