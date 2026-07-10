#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponAttributes.generated.h"

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FWeaponAttributes : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float damage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float critChance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponAttributes")
	float critMultiplier = 2.f;
};