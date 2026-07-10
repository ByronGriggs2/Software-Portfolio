#pragma once
#include "AttackActor.h"
#include "Trail.h"
#include "AOE.h"
#include "MyGameplayTag.h"
#include "AttackTrail.generated.h"

class ACombatant;

struct TrailTag : public MyGameplayTag {
	TWeakObjectPtr<ATrailSegmentFactory> _attack;

	TrailTag() = delete;
	TrailTag(ATrailSegmentFactory* attack, float duration = 999) : MyGameplayTag(duration), _attack(attack) {}
};

UCLASS()
class I_LOVE_VAMPIRES_2_API AAttackTrail : public AAOE {
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<ATrailSegmentFactory> _trailOwner = nullptr;

	std::unique_ptr<TrailTag> getTrailTag() const;
	bool canEffectTarget(ACombatant* target) const;
public:
	bool initialise_AAttackTrail(ATrailSegmentFactory* owner, const AOEInitStruct& init);
	virtual void applyEffect(ACombatant* target) override;
};

class AttackTrailFactory : public AOEFactory {
protected:
	TrailSegmentAttackInitStruct getTrailSegmentInit(bool randomizeRotation = true, const FVector& forward = FVector(0,0,0)) const;
public:
	virtual TArray<AAOE*> launchAttack_Impl(const FVector& forward) override;
	AttackTrailFactory() = delete;
	AttackTrailFactory(const AttackTrailFactory& other) = delete;
	AttackTrailFactory& operator=(const AttackTrailFactory& other) = delete;
	AttackTrailFactory(AttackTrailFactory&& other) : AOEFactory(std::move(other)) {}
	AttackTrailFactory& operator=(AttackTrailFactory&& other) = delete;
	AttackTrailFactory(ACombatant* owner, const UAOETemplate* temp);
};