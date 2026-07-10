#pragma once
#include "CoreMinimal.h"
#include "BaseConfig.h"
#include "AttackActor.h"
#include "AttackConfig.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UAttackConfig : public UBaseConfig {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "AttackConfig")
	TSubclassOf<AAttackActor> _attackClass;
	UPROPERTY(EditAnywhere, Category = "AttackConfig")
	TArray<FEffectStruct> _statusEffects;
	UAttackConfig() { _attackClass = AAttackActor::StaticClass(); }
};