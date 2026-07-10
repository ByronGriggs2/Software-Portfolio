#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatusEffect.generated.h"
class ABaseAttributeSet;

UCLASS()
class I_LOVE_VAMPIRES_2_API UStatusEffect : public UObject {
	GENERATED_BODY()

protected:
	float _duration = 0;
	float _magnitude = 0;
	float _chance = 1;

public:
	void initialise_UStatusEffect(float duration, float magnitude, float chance) {
		_duration = duration;
		_magnitude = magnitude;
		_chance = chance;
	}
	virtual void prebonusStep(float delta, ABaseAttributeSet*) {}
	virtual void multiplierStep(float delta, ABaseAttributeSet*) {}
	virtual void postbonusStep(float delta, ABaseAttributeSet*) {
		_duration -= delta;
	}

	float getDuration() const { return _duration; }
};