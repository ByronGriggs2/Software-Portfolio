#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "DamageEffect.generated.h"

UCLASS()
class  I_LOVE_VAMPIRES_2_API UDamageEffect : public UGameplayEffect {
	GENERATED_BODY()

public:
	virtual void PostLoad() override;
};