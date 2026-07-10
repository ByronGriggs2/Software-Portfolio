#pragma once

// Afaik the only attribute will be attack speed/warmup. To simplify things I'm just going to use the template as the config and copy the base warmup to a member float.
#include "CoreMinimal.h"
// Active
#include <memory>
#include <vector>
#include "AttackActor.h"
#include "StatusEnum.h"
// WeaponTemplate
#include "ActiveEnum.h"
#include "BaseTemplate.h"
// UWeaponConfig
#include "BaseConfig.h"
//
#include "Active.generated.h"

class UBaseAttackTemplate;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UWeaponTemplate : public UBaseTemplate
{
	GENERATED_BODY()

	struct defaults {
		FString _name = "Active";
		float _warmup = 1.f;
		EActivationType _activationType = static_cast<EActivationType>(0);
		EActivationTrigger _activationTrigger = static_cast<EActivationTrigger>(0);
		float _burstInterval = 0.05f;
	};
	const static inline defaults _defaults;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString _name = "_invalid_";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EWeaponType _weaponType = static_cast<EWeaponType>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EActivationType _activationType = static_cast<EActivationType>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _burstInterval = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EActivationTrigger _activationTrigger = static_cast<EActivationTrigger>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool _startOnCooldown = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _warmup = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TSoftObjectPtr<UTexture2D> _icon = nullptr;
	UPROPERTY(EditAnywhere, Instanced) TObjectPtr<UBaseAttackTemplate> _attackData;
	
	UWeaponTemplate(const FObjectInitializer& init) : Super(init) {
	}
	virtual void replaceOverrides() override;
	virtual void dynamicDeepCopy(UObject* context) override;
};

class UWeaponTemplate_Base;
class UWeaponTemplate_Evolution;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UWeaponTemplate_Base : public UWeaponTemplate {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWeaponTemplate_Base> _sister;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWeaponTemplate_Evolution> _evolution;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UWeaponTemplate_Evolution : public UWeaponTemplate {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWeaponTemplate_Base> _baseWeapon;
};
///////////////////////////////////////////////////////////////////////////////
class ACombatant;
class CombatantAttributes;
class UTexture2D;

class BaseAttackFactory;
class Active {
	const static inline EStatus _CHILL = EStatus::chill;
	const static inline EActivationType _SINGLE = EActivationType::single;
	const static inline EActivationType _BURST = EActivationType::burst;
	const static inline EActivationTrigger _PERIODIC = EActivationTrigger::periodic;
	const static inline EActivationTrigger _MANUAL = EActivationTrigger::manual;
	const static inline EActivationTrigger _AUTOCHARGE = EActivationTrigger::autocharge;

	float _chargeRatio = 0;
	std::unique_ptr<BaseAttackFactory> _factory;
	TWeakObjectPtr<ACombatant> _owner = nullptr;
	TArray<FEffectStruct> _statusEffects;
	std::vector<float> _queuedAttacks;
	EActivationTrigger _activationTrigger = static_cast<EActivationTrigger>(0);

protected:
	TObjectPtr<const UWeaponTemplate> _weaponTemplate = nullptr;

private:
	void updateWarmup(float delta);
	bool hasStatus(EStatus status) const;

public:
	Active() = delete;
	virtual ~Active() = default;
	Active(const Active& other) = delete;
	Active(Active&& other);
	Active& operator=(const Active& other) = delete;
	Active& operator=(Active&& other);
	Active(ACombatant* owner, const UWeaponTemplate* data);

	void tick(float delta, const FVector& forward);

	int getLevel() const { return _factory->getLevel(); }
	bool operator==(UWeaponTemplate* other) const;
	bool operator==(const Active& other) const;
	bool isUpgradable() const;
	UWeaponTemplate* getDiskTemplate() const;
	EWeaponType getWeaponType() const;
	const TObjectPtr<const UWeaponTemplate>& getRuntimeTemplate() const { return _weaponTemplate; }
	bool isAOE() const;
	bool getAOERange(float& ret) const;

	void inflictStatus(const FEffectStruct& status);
	void upgrade() { _factory->upgrade(); }
	bool setLevel(int newLevel);
	void setActivationTrigger(EActivationTrigger trigger) { _activationTrigger = trigger; }
	void activate(const FVector&);
	bool isReady() const { return _chargeRatio >= 1 - EPSILON; }
	float getPeriod() const;
};

class Active_WeaponBase : public Active {
	bool _evolutionTurnedDown = false;

public:
	Active_WeaponBase() = delete;
	Active_WeaponBase(const Active_WeaponBase& other) = delete;
	Active_WeaponBase(Active_WeaponBase&& other) : Active(MoveTemp(other)), _evolutionTurnedDown(other._evolutionTurnedDown) {}
	Active_WeaponBase& operator=(const Active_WeaponBase& other) = delete;
	Active_WeaponBase& operator=(Active_WeaponBase&& other);
	Active_WeaponBase(ACombatant* owner, const UWeaponTemplate_Base* data) : Active(owner, data) {}

	bool isEvolutionEligible() const;
	bool wasTurnedDown() const { return _evolutionTurnedDown; }
	void turnDownEvolution() { _evolutionTurnedDown = true; }
	UWeaponTemplate_Evolution* getEvolution() const;
};

