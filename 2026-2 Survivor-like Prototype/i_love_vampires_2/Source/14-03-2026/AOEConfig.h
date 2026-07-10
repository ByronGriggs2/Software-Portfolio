#pragma once
#include "CoreMinimal.h"
#include "BaseConfig.h"
#include "AOE.h"
#include "AOEEnum.h"
#include "AOEConfig.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UAOEConfig : public UBaseConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "AOEConfig")
	EAOEShape _shape = static_cast<EAOEShape>(0);
	UAOEConfig() { _attackClass = AAOE::StaticClass(); }
};
