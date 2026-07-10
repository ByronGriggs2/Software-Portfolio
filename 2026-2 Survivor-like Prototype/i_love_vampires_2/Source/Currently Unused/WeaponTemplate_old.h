#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponTemplate.generated.h"
class UWeaponConfig;
class UAttackData;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UWeaponTemplate : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "WeaponTemplate")
	UWeaponConfig* _config;
	UPROPERTY(EditAnywhere, Instanced, Category = "WeaponTemplate")
	TArray<UAttackData*> _attackData;
};