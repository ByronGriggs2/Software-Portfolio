#pragma once

#include "CoreMinimal.h"
#include "WeaponAttributes.h"
#include "ProjectileAttributes.generated.h"

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FProjectileAttributes : public FWeaponAttributes
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttributes")
	float _spread = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttributes")
	float _radius = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttributes")
	float _speed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttributes")
	float _range = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttributes")
	float _pierce = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttributes")
	float _bounce = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttributes")
	float _projectileCount = 1.f;
};
