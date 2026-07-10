//couldn't think of a better name. This is the portion of ActiveTemplate used only by UActive.

#pragma once

#include "CoreMinimal.h"
#include "AttackActor.h"
#include "UObject/Object.h"
#include "WeaponConfig.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UWeaponConfig : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponConfig")
	FString _name = "Active";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponConfig")
	TArray<TSubclassOf<AAttackActor>> _attackActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponConfig")
	bool _startOnCooldown = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponConfig")
	float warmup = 1.f;
};