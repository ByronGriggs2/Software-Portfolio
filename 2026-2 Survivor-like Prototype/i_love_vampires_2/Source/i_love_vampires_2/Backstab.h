#pragma once
#include "CoreMinimal.h"
#include "AOE.h"
#include "Backstab.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API ABackstab : public AAOE
{
	GENERATED_BODY()

	const static inline EAOETargeting _BACKSTAB = EAOETargeting::backstab;
	const static inline EAOEShape _ARC = EAOEShape::arc;
	bool isBehindTarget(const ACombatant* target) const;

public:
	virtual void applyEffect(ACombatant* target) override;
	virtual void BeginPlay() override;
};