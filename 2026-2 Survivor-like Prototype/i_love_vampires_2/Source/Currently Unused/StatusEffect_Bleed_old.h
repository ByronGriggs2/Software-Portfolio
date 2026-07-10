#pragma once
#include "StatusEffect.h"
#include "StatusEffect_Bleed.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API UStatusEffect_Bleed : public UStatusEffect {
	GENERATED_BODY()
public:
	void initialise_UStatusEffect_Bleed(float duration, float magnitude, float chance) {
		initialise_UStatusEffect(duration, magnitude, chance);
	}
	void postbonusStep(float delta, ABaseAttributeSet*) override;
};