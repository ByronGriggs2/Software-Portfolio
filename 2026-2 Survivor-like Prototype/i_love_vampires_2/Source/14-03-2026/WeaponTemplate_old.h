#pragma once
#include "CoreMinimal.h"
#include "BaseTemplate.h"
#include "BaseConfig.h"
#include "AttackActor.h"
#include "WeaponTemplate.generated.h"
class UWeaponConfig;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UWeaponTemplate : public UBaseTemplate {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Instanced, Category = "WeaponTemplate")
	UWeaponConfig* _config;
	UPROPERTY(EditAnywhere, Instanced, Category = "WeaponTemplate")
	TArray<TObjectPtr<UAttackFactoryTemplate>> _attackData;
};
