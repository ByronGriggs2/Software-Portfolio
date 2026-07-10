#pragma once
#include "CoreMinimal.h"
#include "BaseConfig.h"
#include "WeaponConfig.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UWeaponConfig : public UBaseConfig {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "WeaponConfig")
	FString _name = "Active";
	UPROPERTY(EditAnywhere, Category = "WeaponConfig")
	bool _startOnCooldown = true;
	UPROPERTY(EditAnywhere, Category = "WeaponConfig")
	float warmup = 1.f;
	UPROPERTY(EditAnywhere, Category = "WeaponConfig")
	TA
};