#pragma once

#include "CoreMinimal.h"
#include "WeaponAttributes.h"
#include "AOEAttributes.generated.h"

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FAOEAttributes : public FWeaponAttributes
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOEAttributes")
	float _radius = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOEAttributes")
	float _duration = 0.f;
};
