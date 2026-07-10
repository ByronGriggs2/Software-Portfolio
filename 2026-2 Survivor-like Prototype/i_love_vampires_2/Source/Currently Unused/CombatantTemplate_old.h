#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combatant.h"
#include "CombatantTemplate.generated.h"
UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UCombatantTemplate : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "UCombatantTemplate")
	UCombatantConfig* _config;
	UPROPERTY(EditAnywhere, Instanced, Category = "UCombatantTemplate")
	UCombatantAttributes* _attributes;
};