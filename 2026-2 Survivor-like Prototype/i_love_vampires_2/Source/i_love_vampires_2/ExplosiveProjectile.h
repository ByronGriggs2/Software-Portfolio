#pragma once
#include "CoreMinimal.h"
#include "AOE.h"
// AExplosiveProjectile
#include "Projectile.h"
// UExplosiveProjectileConfig
#include "BaseConfig.h"
// UExplosiveProjectileAttributeData
#include "BaseAttributeData.h"
// ExplosiveProjectileAttributes
#include "BaseAttributes.h"
#include "Combatant.h"
//
#include <memory>
#include "ExplosiveProjectile.generated.h"
class UExplosiveProjectileConfig;
class UExplosiveProjectileAttributeData;
class ExplosiveProjectileAttributes;
struct ExplosiveProjectileInitStruct;
class UPaperFlipbook;

class AAOE;

UCLASS()
class I_LOVE_VAMPIRES_2_API AExplosiveProjectile : public AProjectile {
	GENERATED_BODY()

	UPROPERTY()
	AAOE* _AOE = nullptr;

protected:
	TObjectPtr<const UExplosiveProjectileConfig> _explosiveProjectileConfig = nullptr;
	std::unique_ptr<ExplosiveProjectileAttributes> _explosiveProjectileAttributes = nullptr;

	virtual void bulletDeath() override;
	//virtual void handleSweepResults(const TArray<struct FHitResult>& hits) override;

public:
	AExplosiveProjectile() : AProjectile() {}

	void initialise_AExplosiveProjectile(const ExplosiveProjectileInitStruct& temp);
	virtual void BeginPlay() override;

	const AAOE* getAOE() const { return _AOE; }
};

///////////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UExplosiveProjectileConfig : public UBaseConfig
{
	GENERATED_BODY()

public:
	UExplosiveProjectileConfig(const FObjectInitializer& init) : Super(init) {}
	virtual void dynamicDeepCopy(UObject* context) override {}
	virtual void replaceOverrides() override {}
};
///////////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UExplosiveProjectileAttributeData : public UBaseAttributeData
{
	GENERATED_BODY()

public:
	virtual void replaceOverrides() override {}
	UExplosiveProjectileAttributeData(const FObjectInitializer& init) : Super(init) {}
};
///////////////////////////////////////////////////////////////////////////////

class ExplosiveProjectileAttributes : public BaseAttributes {
	std::weak_ptr<const CombatantAttributes> _attrRef;

	void modifyAttributes(const std::shared_ptr<const CombatantAttributes>&);

public:
	ExplosiveProjectileAttributes() = delete;
	ExplosiveProjectileAttributes(const ExplosiveProjectileAttributes& other) : BaseAttributes(other), _attrRef(other._attrRef) { baseInit(other); }
	ExplosiveProjectileAttributes(ExplosiveProjectileAttributes&& other) : BaseAttributes(std::move(other)), _attrRef(other._attrRef) { baseInit(std::move(other)); other._attrRef.reset(); }
	ExplosiveProjectileAttributes& operator=(const ExplosiveProjectileAttributes& other) = delete;
	ExplosiveProjectileAttributes& operator=(ExplosiveProjectileAttributes&& other) = delete;
	ExplosiveProjectileAttributes(const UExplosiveProjectileAttributeData* data, const std::shared_ptr<const CombatantAttributes>& attr) : BaseAttributes(data), _attrRef(attr) { baseInit(data); }
	virtual ~ExplosiveProjectileAttributes() = default;
	virtual void tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) override;
	
	virtual void discretizeFull() override {}
	virtual void applyStatus(UObject* context, const FEffectStruct& status, float delta) override {}
	virtual void applyToAllStats(const std::function<void(Stat&)>& func) override {}
	virtual void applyToAllStats(const std::function<void(const Stat&)>& func) const override {}
	virtual bool isCompatibleWith(const UBaseAttributeData* data) const override { return dynamic_cast<const UExplosiveProjectileAttributeData*>(data) != nullptr; }
};
///////////////////////////////////////////////////////////////////////////////

struct ExplosiveProjectileInitStruct {
	ProjectileInitStruct _projectile;
	AAOE* _AOE;
	const UExplosiveProjectileConfig* _explosiveProjectileConfig;
	const ExplosiveProjectileAttributes _explosiveProjectileAttributes;
	ExplosiveProjectileInitStruct() = delete;
	ExplosiveProjectileInitStruct(const ProjectileInitStruct& projectile, AAOE* aoe, const UExplosiveProjectileConfig* config, const ExplosiveProjectileAttributes& attr) :
		_projectile(projectile), _AOE(aoe), _explosiveProjectileConfig(config), _explosiveProjectileAttributes(attr){}
};
///////////////////////////////////////////////////////////////////////////////

class ExplosiveProjectileFactory : public ProjectileFactory
{
	TWeakObjectPtr<AAOE> _tempAOE = nullptr;

protected:
	TObjectPtr<const UExplosiveProjectileConfig> _explosiveProjectileConfig = nullptr;
	std::unique_ptr<BaseAttributeWrapper<ExplosiveProjectileAttributes>> _explosiveProjectileAttributes = nullptr;
	TObjectPtr<const UAOEConfig> _AOEConfig = nullptr;
	std::unique_ptr<BaseAttributeWrapper<AOEAttributes>> _AOEAttributes = nullptr;
	TObjectPtr<const UAttackConfig> _AOEConfig_attack = nullptr;
	std::unique_ptr<BaseAttributeWrapper<AttackAttributes>> _AOEAttributes_attack = nullptr;

	ExplosiveProjectileInitStruct getExplosiveProjectileInit() const;
	AOEInitStruct getAOEInit() const;
	virtual AProjectile* launchSingleProjectile(const FVector& direction) override;
	virtual bool isCompatible(const UAttackLevel* level) const override { return Cast<UExplosiveProjectileLevel>(level) != nullptr; }
	virtual void finishUpgrade(const UAttackLevel* newLevel) override;

public:
	ExplosiveProjectileFactory() = delete;
	ExplosiveProjectileFactory(const ExplosiveProjectileFactory& other) = delete;
	ExplosiveProjectileFactory& operator=(const ExplosiveProjectileFactory& other) = delete;
	ExplosiveProjectileFactory(ExplosiveProjectileFactory&& other);
	ExplosiveProjectileFactory& operator=(ExplosiveProjectileFactory&& other) = delete;
	ExplosiveProjectileFactory(ACombatant*, const UExplosiveProjectileTemplate*);
	virtual void tick(float delta) override;
	// Uses the same version as ProjectileFactory. LaunchSingleProjectile is overridden.
	//virtual void launchAttack(const FVector& forward) override;
};
///////////////////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UExplosiveProjectileLevel : public UProjectileLevel {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UExplosiveProjectileAttributeData> _explosiveProjectileAttributeOffsets;
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAOEAttributeData> _AOEAttributeOffsets;
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAttackAttributeData> _AOEAttributeOffsets_attack;
	UPROPERTY(EditAnywhere)
	TArray<FEffectStruct> _AOEStatusEffects;
	virtual void replaceOverrides() override {
		Super::replaceOverrides();
		_explosiveProjectileAttributeOffsets->replaceOverrides();
		_AOEAttributeOffsets->replaceOverrides();
		_AOEAttributeOffsets_attack->replaceOverrides();
	}
	virtual void dynamicDeepCopy(UObject* context) override { Super::dynamicDeepCopy(context); }
	UExplosiveProjectileLevel(const FObjectInitializer& init) : Super(init) {
		_explosiveProjectileAttributeOffsets = init.CreateDefaultSubobject<UExplosiveProjectileAttributeData>(this, "_explosiveProjectileAttributeOffsets");
		_AOEAttributeOffsets = init.CreateDefaultSubobject<UAOEAttributeData>(this, "_AOEAttributeOffsets");
		_AOEAttributeOffsets_attack = init.CreateDefaultSubobject<UAttackAttributeData>(this, "_AOEAttributeOffsets_attack");
	}
};

///////////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UExplosiveProjectileTemplate : public UProjectileTemplate {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UExplosiveProjectileConfig> _explosiveProjectileConfig;
	//UPROPERTY(EditAnywhere, Instanced)
	//TObjectPtr<UExplosiveProjectileAttributeData> _explosiveProjectileAttributes;
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAOEConfig> _AOEConfig;
	//UPROPERTY(EditAnywhere, Instanced)
	//TObjectPtr<UAOEAttributeData> _AOEAttributes;
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAttackConfig> _AOEConfig_attack;
	//UPROPERTY(EditAnywhere, Instanced)
	//TObjectPtr<UAttackAttributeData> _AOEAttributes_attack;

	UExplosiveProjectileTemplate(const FObjectInitializer& init) : Super(init) {
		_explosiveProjectileConfig = init.CreateDefaultSubobject<UExplosiveProjectileConfig>(this, "_explosiveProjectileConfig");
		//_explosiveProjectileAttributes = init.CreateDefaultSubobject<UExplosiveProjectileAttributeData>(this, "_explosiveProjectileAttributes");
		_AOEConfig = init.CreateDefaultSubobject<UAOEConfig>(this, "_AOEConfig");
		//_AOEAttributes = init.CreateDefaultSubobject<UAOEAttributeData>(this, "_AOEAttributes");
		_AOEConfig_attack = init.CreateDefaultSubobject<UAttackConfig>(this, "_AOEConfig_attack");
		//_AOEAttributes_attack = init.CreateDefaultSubobject<UAttackAttributeData>(this, "_AOEAttributes_attack");
	}
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override;
	virtual void replaceOverrides() override {
		Super::replaceOverrides();
		_explosiveProjectileConfig->replaceOverrides();
		//_explosiveProjectileAttributes->replaceOverrides();
		_AOEConfig->replaceOverrides();
		//_AOEAttributes->replaceOverrides();
		_AOEConfig_attack->replaceOverrides();
		//_AOEAttributes_attack->replaceOverrides();
	}
	virtual void dynamicDeepCopy(UObject* context) override {
		if (!IsValid(_explosiveProjectileConfig) || !IsValid(_AOEConfig) || !IsValid(_AOEConfig_attack)) {
			LOGERROR("Invalid config in dynamicDeepCopy of UExplosiveProjectileTemplate");
			return;
		}
		Super::dynamicDeepCopy(context);
		_explosiveProjectileConfig->dynamicDeepCopy(context);
		_AOEConfig->dynamicDeepCopy(context);
		_AOEConfig_attack->dynamicDeepCopy(context);
	}
};

