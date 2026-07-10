#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "BurnEffect.generated.h"

UCLASS()
class  I_LOVE_VAMPIRES_2_API UBurnEffect : public UGameplayEffect {
	GENERATED_BODY()

public:
	virtual void PostLoad() override;
};