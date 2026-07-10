#pragma once
#include "CoreMinimal.h"
#include "AOE.h"
#include "Donut.generated.h"

struct DonutInitStruct;
UCLASS()
class I_LOVE_VAMPIRES_2_API ADonut : public AAOE {
	GENERATED_BODY()
	float _innerOuterRatio = 0.5f;

public:
	virtual bool shouldApplyEffect(AActor* other) const override;
	virtual bool createCollider() override;
	bool initialise_ADonut(const DonutInitStruct& temp);
};

struct DonutInitStruct {
	AOEInitStruct _base;
	float _innerOuterRatio;

	DonutInitStruct() = delete;
	DonutInitStruct(const AOEInitStruct& base, float innerOuterRatio) : _base(base), _innerOuterRatio(innerOuterRatio) {}
};

class DonutFactory : public AOEFactory {
	float _innerOuterRatio = 0.5f;

protected:
	DonutInitStruct getDonutInit() const;
public:
	DonutFactory() = delete;
	DonutFactory(const DonutFactory& other) = delete;
	DonutFactory(DonutFactory&& other);
	DonutFactory& operator=(const DonutFactory& other) = delete;
	DonutFactory& operator=(DonutFactory&& other) = delete;
	DonutFactory(ACombatant* owner, const UDonutTemplate* temp);
	virtual TArray<AAOE*> launchAttack_Impl(const FVector& forward) override;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UDonutTemplate : public UAOETemplate {
	GENERATED_BODY()

	struct defaults {
		float _innerOuterRatio = 0.5f;
	};
	const static inline defaults _defaults;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float _innerOuterRatio = SENTINEL_FLOAT;
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override { return std::make_unique<DonutFactory>(owner, this); }
	virtual void replaceOverrides() override;
};