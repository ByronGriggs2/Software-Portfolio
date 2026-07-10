// Originally created for greataxe, I probably could have just used the AOE spawner, but I think sticking to factories is much cleaner.
// This simply has a factory that contains an aribtrary number of other factories of arbitrary attack types. You could for example have 
// both a projectile and a summon. They do need to share Active.h data though, which includes cooldown.
#pragma once
#include "CoreMinimal.h"
#include "AttackActor.h"
#include <vector>
#include "Definitions.h"
#include "MultiAttack.generated.h"

class UMultiAttackTemplate;
class MultiAttackFactory : public BaseAttackFactory {
	std::vector<std::unique_ptr<BaseAttackFactory>> _factories;
	float _delayBetweenAttacks;
	bool _isFiring = false;
	float _timeSinceLastAttack = 0;
	int _currentAttackIndex = 0;
	FVector _lastForward = FVector(1, 0, 0);

public:
	MultiAttackFactory() = delete;
	MultiAttackFactory(const MultiAttackFactory& other) = delete;
	MultiAttackFactory(MultiAttackFactory&& other);
	MultiAttackFactory& operator=(const MultiAttackFactory& other) = delete;
	MultiAttackFactory& operator=(MultiAttackFactory&& other) = delete;
	MultiAttackFactory(ACombatant* owner, const UMultiAttackTemplate* temp);
	virtual void launchAttack(const FVector& forward) override;
	virtual int getMaxLevel() const override;
	virtual void setLevel(int newLevel) override;
	virtual bool canFire() const override;
	virtual void tick(float delta) override;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UMultiAttackTemplate : public UBaseAttackTemplate {
	GENERATED_BODY()
	struct defaults {
		float _delayBetweenAttacks = 0;
	};
	const static inline defaults _defaults;

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UBaseAttackTemplate>> _attacks;
	UPROPERTY(EditAnywhere)
	float _delayBetweenAttacks = SENTINEL_FLOAT;
	virtual void replaceOverrides() override;
	virtual void dynamicDeepCopy(UObject* context) override;
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override{ return std::make_unique<MultiAttackFactory>(owner, this); }
};