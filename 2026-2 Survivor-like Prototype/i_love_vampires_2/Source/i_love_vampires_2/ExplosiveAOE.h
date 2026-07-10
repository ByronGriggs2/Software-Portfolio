#pragma once
#include "CoreMinimal.h"
#include <memory>
#include "NestedAOE.h"
#include "ExplosiveAOE.generated.h"

// AExplosiveAOE is NOT an AConcentricAOE, but it can use the exact same template to initialise
struct ExplosiveAOEInitStruct {
	AOEInitStruct _AOE;
	TArray<AOEInitStruct> _childAOEs;
};

UCLASS()
class I_LOVE_VAMPIRES_2_API AExplosiveAOE : public AAOE {
	GENERATED_BODY()

	TArray<AOEInitStruct> _childAOEs;

	TArray<AAOE*> spawnChildren(ACombatant* target);

public:
	virtual void applyEffect(ACombatant* target) override;
	bool initialise_AExplosiveAOE(const ExplosiveAOEInitStruct& temp);
	virtual void BeginPlay() override;
};

class ExplosiveAOEFactory : public NestedAOEFactory {
protected:
	ExplosiveAOEInitStruct getExplosiveAOEInit() const;
public:
	ExplosiveAOEFactory() = delete;
	ExplosiveAOEFactory(const ExplosiveAOEFactory& other) = delete;
	ExplosiveAOEFactory& operator=(const ExplosiveAOEFactory& other) = delete;
	ExplosiveAOEFactory(ExplosiveAOEFactory&& other) : NestedAOEFactory(std::move(other)) {}
	ExplosiveAOEFactory& operator=(ExplosiveAOEFactory&& other) = delete;
	ExplosiveAOEFactory(ACombatant* owner, const UAOETemplate* mainTemplate, const TArray<const UAOETemplate*> _childTemplates) : NestedAOEFactory(owner, mainTemplate, _childTemplates) {}
	virtual TArray<AAOE*> launchAttack_Impl(const FVector& forward) override;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UExplosiveAOETemplate : public UNestedAOETemplate {
	GENERATED_BODY()

public:
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override {
		auto temp = preCreateFactory();
		return std::make_unique<ExplosiveAOEFactory>(owner, _primaryAOE.Get(), temp);
	}
};