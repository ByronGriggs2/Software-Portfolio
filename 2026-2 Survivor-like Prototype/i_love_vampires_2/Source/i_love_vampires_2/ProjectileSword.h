#pragma once
#include "CoreMinimal.h"
#include "AOE.h"
#include "Projectile.h"
#include "ProjectileSword.generated.h"
// Instead of making a new spawner actor I decided to just spawn the projectile and sword slash simultaneously.
// Initialise functions are called by base class(es)

class ProjectileSwordFactory : public AOEFactory
{
	ProjectileFactory _projectileFactory;

public:
	ProjectileSwordFactory() = delete;
	ProjectileSwordFactory(const ProjectileSwordFactory& other) = delete;
	ProjectileSwordFactory& operator=(const ProjectileSwordFactory& other) = delete;
	ProjectileSwordFactory(ProjectileSwordFactory&& other);
	ProjectileSwordFactory& operator=(ProjectileSwordFactory&& other) = delete;
	ProjectileSwordFactory(ACombatant* owner, const UAOETemplate* aoeTemplate, const UProjectileTemplate* projectileTemplate);
	virtual void tick(float delta) override {
		AOEFactory::tick(delta);
		_projectileFactory.tick(delta);
	}
	virtual TArray<AAOE*> launchAttack_Impl(const FVector& forward) override;
	virtual void inflictStatus(const FEffectStruct& status) override {
		AOEFactory::inflictStatus(status);
		_projectileFactory.inflictStatus(status);
	}
};


// Although ProjectileSword inherits from AOE, the template is on the same tier in the hierarchy as AttackTemplate (because it's weird)
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UProjectileSwordTemplate : public UBaseAttackTemplate {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAOETemplate> _aoeTemplate;
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UProjectileTemplate> _projectileTemplate;

	UProjectileSwordTemplate(const FObjectInitializer& init) : Super(init) {
		_aoeTemplate = init.CreateDefaultSubobject<UAOETemplate>(this, "_aoeTemplate");
		_projectileTemplate = init.CreateDefaultSubobject<UProjectileTemplate>(this, "_projectileTemplate");
	}
	virtual void dynamicDeepCopy(UObject* context) override {
		if (IsValid(_aoeTemplate))
			_aoeTemplate->dynamicDeepCopy(context);
		if (IsValid(_projectileTemplate))
			_projectileTemplate->dynamicDeepCopy(context);
	};
	virtual void replaceOverrides() override {
		if (IsValid(_aoeTemplate))
			_aoeTemplate->replaceOverrides();
		if (IsValid(_projectileTemplate))
			_projectileTemplate->replaceOverrides();
	}
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override {
		return std::make_unique<ProjectileSwordFactory>(owner, _aoeTemplate.Get(), _projectileTemplate.Get());
	}
};