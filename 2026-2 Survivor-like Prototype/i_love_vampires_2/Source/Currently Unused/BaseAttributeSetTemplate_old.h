#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseAttributeSet.h"
#include "BaseAttributeSetTemplate.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UBaseAttributeSetTemplate : public UObject
{
	GENERATED_BODY()

public:
	UBaseAttributeSetTemplate(const FObjectInitializer& init) : Super(init) {}
};