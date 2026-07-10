#pragma once
#include "CoreMinimal.h"
#include "SummonAttack.h"
#include "SummonEnum.h"
#include "ChasingSummon.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API AChasingSummon : public ASummon
{
	GENERATED_BODY()

	const static inline ESummonTargeting _NEAREST = ESummonTargeting::nearestEnemy;
	const static inline EActivationTrigger _AUTOCHARGE = EActivationTrigger::autocharge;

	bool getNearestEnemyLocation(FVector& ret) const;
	int isInRange(ESummonTargeting targetingType, const FVector& targetLocationIn, const FVector& myLocationIn) const;
	
protected:
	virtual void Tick_Impl(float delta) override;
public:
	virtual void giveWeapon_safe(const UWeaponTemplate* data, int socket = -1) override;
};

