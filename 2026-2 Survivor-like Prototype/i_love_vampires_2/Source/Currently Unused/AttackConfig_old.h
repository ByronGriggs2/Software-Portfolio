#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EffectStruct.h"
#include "WeaponEnum.h"
#include "WeaponConfig.generated.h"

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FWeaponConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponConfig")
	TArray<FEffectStruct> _statusEffects;
};