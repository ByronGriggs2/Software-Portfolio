#pragma once
#include "CoreMinimal.h"
// AAttackActor
#include "GameFramework/Actor.h"
#include <memory>
#include "StatusEnum.h"
#include "GameFramework/Pawn.h"
// UAttackConfig
//#include "SpriteEnum.h"
#include "BaseConfig.h"
#include "EffectStruct.h"
#include "PaperFlipbook.h"
// UAttackAttributeData
#include "BaseAttributeData.h"
#include <unordered_map>
// AttackAttributes
#include "BaseAttributes.h"
#include "Combatant.h"
// AttackFactory
#include "BaseAttributeSet.h"
#include "BaseAttributeWrapper.h"
// UAttackTemplate
#include "BaseTemplate.h"
// UAttackUpgrade
#include "BaseLevelContainer.h"
//
#include "AttackActor.generated.h"

struct effectModifiers {
	float _powerMultiplier = 1;
};

class UAttackConfig;
class AttackAttributes;
class AttackFactory;
class UAttackAttributeData;
class UAttackTemplate;
struct AttackInitStruct;

class ACombatant;
class UPaperFlipbookComponent;

class AAttackActorAttourney;
UCLASS()
class I_LOVE_VAMPIRES_2_API AAttackActor : public AActor {
	GENERATED_BODY()

	friend class AAttackActorAttourney;

	const static inline EStatus _DAMAGE = EStatus::damage;
	const static inline EStatus _HEAL_INSTIGATOR = EStatus::healInstigator;
	const static inline EStatus _RANDOM = EStatus::randomNegativePersistent;
	const static inline EStatus _FRIENDLY_FIRE = EStatus::friendlyFire;

	TObjectPtr<const UAttackConfig> _attackConfig = nullptr;
	TArray<FEffectStruct> _statusEffects;
	effectModifiers _nextHitModifiers;
	bool _friendly = false;
	
protected:
	TWeakObjectPtr<ACombatant> _pawnRef = nullptr;
	TArray<TWeakObjectPtr<APawn>> _effectedPawns;
	std::unique_ptr<const AttackAttributes> _attackAttributes = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	UPaperFlipbookComponent* _flipbook = nullptr;
	const TArray<FEffectStruct>& getStatus() const { return _statusEffects; }
	effectModifiers& getNextHitModifiersRef() { return _nextHitModifiers; }
	virtual void updateActorScale();
	bool isFriendly() const { return _friendly; }
	float getRadius() const;

private:
	void initialise_AAttackActor(ACombatant* pawnRef, const UAttackConfig* attackConfig, const AttackAttributes& attackAttributes, const TArray<FEffectStruct>& statusEffects);

protected:
	const UAttackConfig* getAttackConfig() const { return _attackConfig.Get(); }
	bool canHitInstigator() const;
	bool isHittable(const ACombatant* target);
	bool isFriendly(const ACombatant* target) const;

public:
	AAttackActor();
	void initialise_AAttackActor(const AttackInitStruct& temp);
	virtual void BeginPlay() override;
	virtual void Tick(float delta) override;
	virtual void applyEffect(ACombatant* target); 
	bool setFlipbook(UPaperFlipbook* flipbook);
};

class AAttackActorAttourney {
public:
	static UPaperFlipbookComponent* getFlipbookRef(const AAttackActor* actor) {
		return actor->_flipbook;
	}
};
///////////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UAttackConfig : public UBaseConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbook> _sprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool _isCosmetic = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool _isSingleTarget = false;

	UAttackConfig(const FObjectInitializer& init) : Super(init) {
		_sprite = init.CreateDefaultSubobject<UPaperFlipbook>(this, "_sprite");
	}
	virtual void replaceOverrides() override;
	virtual void dynamicDeepCopy(UObject* context) override {}
};
///////////////////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UAttackAttributeData : public UBaseAttributeData
{
	GENERATED_BODY()

	using self = UAttackAttributeData;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _damage = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _critChance = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _critMultiplier = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _radius = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _burstCount = SENTINEL_FLOAT;

	UAttackAttributeData(const FObjectInitializer& init) : Super(init) {}
	virtual void replaceOverrides() override;
};
template<>
class DefaultProxy<UAttackAttributeData> {
public:
	using self = UAttackAttributeData;
	const static std::unordered_map<float(self::*), float, helpers::MemberPtrHash>& get() {
		const static std::unordered_map<float(self::*), float, helpers::MemberPtrHash> temp = {
			{ &self::_damage, 0 },
			{ &self::_critChance, 0 },
			{ &self::_critMultiplier, 1 },
			{ &self::_radius, 1 },
			{ &self::_burstCount, 1 }
		};
		return temp;
	}
};
///////////////////////////////////////////////////////////////////////////////
#define MYSTAT(X) \
	X(_damage) \
	X(_critChance) \
	X(_critMultiplier) \
	X(_radius) \
	X(_burstCount) 

class AttackAttributes : public BaseAttributes {
	std::weak_ptr<const CombatantAttributes> _attrRef;

	void modifyAttributes(const std::shared_ptr<const CombatantAttributes>&);

public:
	MYSTAT(BASEATTRIBUTES_DECLARE);

	AttackAttributes() = delete;
	AttackAttributes(const AttackAttributes& other);
	AttackAttributes(AttackAttributes&& other);
	AttackAttributes& operator=(const AttackAttributes& other) = delete;
	AttackAttributes& operator=(AttackAttributes&& other) = delete;
	AttackAttributes(const UAttackAttributeData* attr, std::shared_ptr<const CombatantAttributes> attrRef);
	virtual ~AttackAttributes() = default;

	virtual void tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) override;
	virtual void discretizeFull() override {
		_burstCount.discretize();
	}
	virtual void applyStatus(UObject* context, const FEffectStruct& status, float delta) override {}
	virtual void applyToAllStats(const std::function<void(Stat&)>& func) override {
		MYSTAT(BASEATTRIBUTES_APPLY);
	}
	virtual void applyToAllStats(const std::function<void(const Stat&)>& func) const override {
		MYSTAT(BASEATTRIBUTES_APPLY);
	}
	virtual bool isCompatibleWith(const UBaseAttributeData* data) const override {
		return dynamic_cast<const UAttackAttributeData*>(data) != nullptr;
	}
};
#undef MYSTAT
///////////////////////////////////////////////////////////////////////////////
class AttackInitStructAttourney;
struct AttackInitStruct {
private:
	AttackAttributes _attackAttributes;
public:
	friend class AttackInitStructAttourney;
	ACombatant* _pawnRef;
	const UAttackConfig* _attackConfig;
	TArray<FEffectStruct> _statusEffects;
	AttackInitStruct() = delete;
	AttackInitStruct(ACombatant* pawn, const UAttackConfig* config, const AttackAttributes& attr, const TArray<FEffectStruct>& statusEffects) :
		_attackAttributes(attr), _pawnRef(pawn), _attackConfig(config), _statusEffects(statusEffects) {}
	const AttackAttributes& getAttackAttributes() const { return _attackAttributes; }
};
class SuplexFactory;
class AttackInitStructAttourney {
	static AttackAttributes& getAttackAttributes(AttackInitStruct& temp) {
		return temp._attackAttributes;
	}
public:
	friend class SuplexFactory;
};
///////////////////////////////////////////////////////////////////////////////

class AProjectile;
class AExplosiveProjectile;
class AAOE;
class ACombatant;

class BaseAttackFactory : public BaseAttributeSet {

protected:
	int _level = 0;
	const TWeakObjectPtr<ACombatant> _owner = nullptr;

public:
	BaseAttackFactory() = delete;
	BaseAttackFactory(const BaseAttackFactory& other) = delete;
	BaseAttackFactory(BaseAttackFactory&& other);
	BaseAttackFactory& operator=(const BaseAttackFactory& other) = delete;
	BaseAttackFactory& operator=(BaseAttackFactory&& other) = delete;
	BaseAttackFactory(ACombatant* owner);

	virtual void launchAttack(const FVector& forward) = 0;
	int getLevel() const { return _level; }
	virtual int getMaxLevel() const = 0;
	virtual void upgrade() { setLevel(getLevel() + 1); }
	virtual void tick(float delta) override { BaseAttributeSet::tick(delta); }
	virtual void setLevel(int newLevel) = 0;
	virtual bool canFire() const { return true; }
};

class AttackFactory : public BaseAttackFactory {
	
	std::unique_ptr<BaseAttributeWrapper<AttackAttributes>> _attackAttributes = nullptr;
protected:
	TObjectPtr<const UAttackConfig> _attackConfig = nullptr;
	TArray<const TObjectPtr<const UAttackLevel>> _levels = {};
	// Cannot be const as it is used to call CreateObject/SpawnActor
	
	AttackInitStruct getAttackInit() const;
	virtual bool isCompatible(const UAttackLevel* level) const { return true; }
	virtual void finishUpgrade(const UAttackLevel* newLevel);
public:
	AttackFactory() = delete;
	AttackFactory(const AttackFactory& other) = delete;
	AttackFactory& operator=(const AttackFactory& other) = delete;
	AttackFactory(AttackFactory&& other);
	AttackFactory& operator=(AttackFactory&& other) = delete;
	AttackFactory(ACombatant* owner, const UAttackTemplate* temp);
	virtual void tick(float delta) override;
	float getMember(Stat AttackAttributes::* member) const {
		if (!_attackAttributes) {
			LOGERROR("Attempting to access uninitialized AttackFactory attributes");
			return 0;
		}
		return _attackAttributes->getMember(member);
	}
	const BaseAttributeWrapper<AttackAttributes>& getAttackAttributeWrapper() const { return *_attackAttributes; }
	virtual void launchAttack(const FVector& forward) override;
	virtual int getMaxLevel() const override { return _levels.Num() - 1; }
	int getBurstCount() const {
		if (!_attackAttributes) {
			LOGERROR("Attempting to access uninitialized AttackFactory attributes");
			return 0;
		}
		Stat temp(_attackAttributes->getCore()->_burstCount);
		temp.discretize();
		return temp.getFinal();
	}
	virtual void setLevel(int newLevel) override;
};
///////////////////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class I_LOVE_VAMPIRES_2_API UAttackLevel : public UBaseLevelContainer {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAttackAttributeData> _attackOffsets;
	UPROPERTY(EditAnywhere)
	TArray<FEffectStruct> _statusEffects = {};
	virtual void replaceOverrides() override {
		_attackOffsets->replaceOverrides();
	}
	UAttackLevel(const FObjectInitializer& init) : Super(init) {
		_attackOffsets = init.CreateDefaultSubobject<UAttackAttributeData>(this, "_attackOffsets");
	}
	virtual void dynamicDeepCopy(UObject* context) override {}
};

///////////////////////////////////////////////////////////////////////////////

// The base template is blank to allow for really weird weapons
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UBaseAttackTemplate : public UBaseTemplate {
	GENERATED_BODY()

public:
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const { return nullptr; }
	virtual void replaceOverrides() override {}
	virtual void dynamicDeepCopy(UObject* context) override {}
};

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UAttackTemplate : public UBaseAttackTemplate {
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, Instanced)
	//TObjectPtr<UAttackAttributeData> _attackAttributes;
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAttackConfig> _attackConfig;
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UAttackLevel>> _levels = {};

	UAttackTemplate(const FObjectInitializer& init) : Super(init) {
		_attackConfig = init.CreateDefaultSubobject<UAttackConfig>(this, "_attackConfig");
		//_attackAttributes = init.CreateDefaultSubobject<UAttackAttributeData>(this, "_attackAttributes");
	}
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override;
	virtual void replaceOverrides() override;
	virtual void dynamicDeepCopy(UObject* context) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};