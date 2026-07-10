#pragma once
// The Combatant subclass is in Combtatant/Friendlies/SummonActor.h. This breaks the naming convention but oh well.
#include "CoreMinimal.h"
#include "AttackActor.h"
#include <memory>
#include "SummonEnum.h"
#include "Summon.h"
#include "Passive.h"
#include "SummonAttack.generated.h"

// when a unit is summoned, the unit's master's Passives and Status Effects are expicitly queried for everything that could be relevant for the summon (like summon damage).
// these passives/status effects are then converted into passives/status effects which are applied to the summon. Additionally, once per frame, all summons that have been created
// by the factory have this process repeated (dead summons are removed from the list). If this is intensive it could be made less frequent, but for now I would like split second
// buffs to actually work.
// So for example if the player has a passive that grants +30% summon damage, the summons will be created with/updated to a passive that gives +30% damage (the conversion is done explicitly
// per effect type).
// If the player has a status effect that boosts global damage by 15%, the summons will be given a passive that increases global damage by 15%. This will be removed once the master's
// buff wears off (and the summons are updated)

// Correction - to improve maintainability, CombatantAttributes has a "convert to summon" function which converts the attributes
// of the master into passives for the summon. 
class ACombatant;

class SummonFactory : public BaseAttackFactory {
	const static inline ESummonType _CHASING = ESummonType::chasing;

	TObjectPtr<const USummonTemplate> _template = nullptr;
	TArray<TWeakObjectPtr<ASummon>> _pastSummons;
	std::vector<Passive> _passiveCache;

	void updateSummon(ASummon* summon);

public:
	SummonFactory() = delete;
	SummonFactory(const SummonFactory& other) = delete;
	SummonFactory& operator=(const SummonFactory& other) = delete;
	SummonFactory(SummonFactory&& other);
	SummonFactory& operator=(SummonFactory&& other) = delete;
	// template is assumed to by dynamically copied by master.GiveWeapon() or master.initialise
	SummonFactory(ACombatant* owner, const USummonTemplate* temp);
	~SummonFactory();

	virtual void tick(float delta) override;
	virtual void launchAttack(const FVector& forward) override;
	virtual int getMaxLevel() const override;
	virtual void setLevel(int newLevel) override;
	SummonInitStruct getSummonInit() const;
};

UCLASS(EditInlineNew)
class I_LOVE_VAMPIRES_2_API USummonConfig : public UBaseConfig
{
	GENERATED_BODY()

	struct defaults {
		float _duration = 999.0f;
	};
	const static inline defaults _defaults;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESummonTargeting _targeting = static_cast<ESummonTargeting>(255);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESummonType _type = static_cast<ESummonType>(255);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float _duration = SENTINEL_FLOAT;
	virtual void dynamicDeepCopy(UObject* context) override {}
	virtual void replaceOverrides() override;
};

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API USummonTemplate : public UBaseAttackTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UCombatantConfig> _config;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<USummonConfig> _summonConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UCombatantAttributeData>> _combatantLevels;

	virtual void replaceOverrides() override {
		_config->replaceOverrides();
		_summonConfig->replaceOverrides();
		for (auto& level : _combatantLevels) {
			level->replaceOverrides();
		}
	}
	virtual void dynamicDeepCopy(UObject* context) override {
		_config->dynamicDeepCopy(context);
		//UCombatantAttributeData does not have a deep copy
	}
	USummonTemplate(const FObjectInitializer& init) : Super(init) {
		_config = init.CreateDefaultSubobject<UCombatantConfig>(this, "_config");
		_config->_combatantClass = ASummon::StaticClass();
		_summonConfig = init.CreateDefaultSubobject<USummonConfig>(this, "_summonConfig");
	}
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override {
		return std::make_unique<SummonFactory>(owner, this);
	}
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

