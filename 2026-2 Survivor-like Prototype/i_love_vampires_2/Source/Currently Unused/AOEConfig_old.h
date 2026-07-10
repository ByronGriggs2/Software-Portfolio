#pragma once

#include "CoreMinimal.h"
#include "AOEEnum.h"
#include "WeaponConfig.h"
#include "AOEConfig.generated.h"

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FAOEConfig : public FWeaponConfig
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOEConfig")
	EAOEShape _shape = static_cast<EAOEShape>(0);
};
