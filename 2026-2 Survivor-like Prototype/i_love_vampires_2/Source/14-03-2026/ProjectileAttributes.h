#pragma once
#include "CoreMinimal.h"
#include "BaseAttributes.h"
#include "ProjectileAttributes.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UProjectileAttributes : public UBaseAttributes
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "ProjectileAttributes")
	float _spread = 1.f;
	UPROPERTY(EditAnywhere, Category = "ProjectileAttributes")
	float _radius = 1.f;
	UPROPERTY(EditAnywhere, Category = "ProjectileAttributes")
	float _speed = 1.f;
	UPROPERTY(EditAnywhere, Category = "ProjectileAttributes")
	float _range = 1.f;
	UPROPERTY(EditAnywhere, Category = "ProjectileAttributes")
	float _pierce = 0.f;
	UPROPERTY(EditAnywhere, Category = "ProjectileAttributes")
	float _bounce = 0.f;
	UPROPERTY(EditAnywhere, Category = "ProjectileAttributes")
	float _projectileCount = 1.f;
};
