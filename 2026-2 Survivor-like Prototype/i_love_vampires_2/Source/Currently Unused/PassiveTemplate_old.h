#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MyCombatantAttributeSet.h"
#include "AttributeModifier.h"
#include "PassiveTemplate.generated.h"

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FPassiveTemplate : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	FPassiveTemplate() = default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveTemplate")
	FString _name = "Passive";
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveTemplate")
	//TMap<FName, AttributeModifier> combatantAttributeModifiers;
};