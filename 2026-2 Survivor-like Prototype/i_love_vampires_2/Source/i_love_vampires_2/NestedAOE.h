#pragma once
#include "CoreMinimal.h"
#include "AOE.h"
#include <memory>
#include "Definitions.h"
#include "NestedAOE.generated.h"

class UNestedAOETemplate;

class NestedAOEFactory : public AOEFactory
{
protected:
	std::vector<std::unique_ptr<AOEFactory>> _childFactories;

public:
	NestedAOEFactory() = delete;
	NestedAOEFactory(const NestedAOEFactory& other) = delete;
	NestedAOEFactory& operator=(const NestedAOEFactory& other) = delete;
	NestedAOEFactory(NestedAOEFactory&& other);
	NestedAOEFactory& operator=(NestedAOEFactory&& other) = delete;
	NestedAOEFactory(ACombatant* owner, const UAOETemplate* mainTemplate, const TArray<const UAOETemplate*> _childTemplates);
	virtual void tick(float delta) override {
		AOEFactory::tick(delta);
		for (auto& factory : _childFactories)
			factory->tick(delta);
	}
	virtual TArray<AAOE*> launchAttack_Impl(const FVector& forward) override = 0;
	virtual void inflictStatus(const FEffectStruct& status) override {
		AOEFactory::inflictStatus(status);
		for (auto& factory : _childFactories)
			factory->inflictStatus(status);
	}
	virtual void setLevel(int newLevel) override {
		AOEFactory::setLevel(newLevel);
		for (auto& factory : _childFactories)
			factory->setLevel(newLevel);
	}
};

// Although NestedAOE inherits from AOE, the template is on the same tier in the hierarchy as AttackTemplate (because it's weird)
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UNestedAOETemplate : public UBaseAttackTemplate {
	GENERATED_BODY()

protected:
	TArray<const UAOETemplate*> preCreateFactory() const {
		TArray<const UAOETemplate*> temp;
		for (const auto& child : _childAOEs) {
			temp.Add(child.Get());
		}
		return temp;
	}

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAOETemplate> _primaryAOE;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UAOETemplate>> _childAOEs = {};

	UNestedAOETemplate(const FObjectInitializer& init) : Super(init) {
		_primaryAOE = init.CreateDefaultSubobject<UAOETemplate>(this, "_primaryAOE");
	}
	virtual void dynamicDeepCopy(UObject* context) override {
		if (IsValid(_primaryAOE))
			_primaryAOE->dynamicDeepCopy(context);
		for (auto& child : _childAOEs) {
			if (IsValid(child))
				child->dynamicDeepCopy(context);
		}
	};
	virtual void replaceOverrides() override {
		if (IsValid(_primaryAOE))
			_primaryAOE->replaceOverrides();
		for (auto& child : _childAOEs) {
			if (IsValid(child))
				child->replaceOverrides();
		}
	}
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override
		PURE_VIRTUAL(UNestedAOETemplate::createFactory,
			LOGERROR("createFactory not implemented in this class"); return nullptr;);
};