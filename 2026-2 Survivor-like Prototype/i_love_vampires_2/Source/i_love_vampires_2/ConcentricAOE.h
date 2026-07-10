#pragma once
#include "CoreMinimal.h"
#include <vector>
#include <memory>
#include "NestedAOE.h"
#include "ConcentricAOE.generated.h"
// Instead of making a new spawner actor I decided to just spawn the projectile and sword slash simultaneously.
// Initialise functions are called by base class(es)

class ConcentricAOEFactory : public NestedAOEFactory
{
public:
	ConcentricAOEFactory() = delete;
	ConcentricAOEFactory(const ConcentricAOEFactory& other) = delete;
	ConcentricAOEFactory& operator=(const ConcentricAOEFactory& other) = delete;
	ConcentricAOEFactory(ConcentricAOEFactory&& other) : NestedAOEFactory(std::move(other)) {}
	ConcentricAOEFactory& operator=(ConcentricAOEFactory&& other) = delete;
	ConcentricAOEFactory(ACombatant* owner, const UAOETemplate* mainTemplate, const TArray<const UAOETemplate*> _childTemplates) : NestedAOEFactory(owner, mainTemplate, _childTemplates) {}

	virtual TArray<AAOE*> launchAttack_Impl(const FVector& forward) override;
};

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UConcentricAOETemplate : public UNestedAOETemplate {
	GENERATED_BODY()

public:
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override {
		auto temp = preCreateFactory();
		return std::make_unique<ConcentricAOEFactory>(owner, _primaryAOE.Get(), temp);
	}
};