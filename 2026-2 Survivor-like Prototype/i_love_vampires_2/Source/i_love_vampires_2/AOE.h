#pragma once
#include "CoreMinimal.h"
// AAOE
#include "AttackActor.h"
// UAOEConfig
#include "AOEEnum.h"
// UAOEAttributeData
#include "BaseAttributeData.h"
// AOEAttributes
#include "BaseAttributes.h"
#include "Combatant.h"
//
#include "AOE.generated.h"

class UShapeComponent;
class UAOEConfig;
class UAOEAttributeData;
class AOEAttributes;
struct AOEInitStruct;

UCLASS()
class AAOE : public AAttackActor {
	GENERATED_BODY()

	const static inline EAOEShape _CIRCLE = EAOEShape::circle;
	const static inline EAOEShape _ARC = EAOEShape::arc;
	const static inline EAOEShape _LINE = EAOEShape::line;
	const static inline EEffectApplication _ONCE = EEffectApplication::once;
	const static inline EEffectApplication _TICK = EEffectApplication::tick;
	const static inline EAOETargeting _FOLLOW_INSTIGATOR = EAOETargeting::followInstigator;
	//const static inline bool _FOLLOW_AIM = false;

	float _consumedDuration = 0;
	// set during construction
	float _nextTick = 999;
	bool _isAfterimage = false;
	bool _initialisedWithDelay = false;
	
protected:
	UPROPERTY()
	UShapeComponent* _collider = nullptr;
	TObjectPtr<const UAOEConfig> _AOEConfig = nullptr;
	std::unique_ptr<const AOEAttributes> _AOEAttributes = nullptr;

	virtual void updateActorScale() override;
	virtual bool shouldApplyEffect(AActor* other) const;
	virtual bool createCollider();
	UShapeComponent* makeDefaultSphereCollider();

private:
	void initShape();
	void reorientSlash();
	void collideWithAll();

public:
	AAOE() : AAttackActor() {
		PrimaryActorTick.bStartWithTickEnabled = false;
	}
	void initialise_AAOE(const AOEInitStruct& temp);
	void completeDelayedConstruction();
	//virtual void factoryInitQuery(AAttackFactory* factory) override;
	float getRemainingDuration() const;
	float getTotalDuration() const;

	virtual void Tick(float delta) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};

///////////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UAOEConfig : public UBaseConfig
{
	GENERATED_BODY()

	struct defaults {
		EAOEShape _shape = static_cast<EAOEShape>(0);
		EAOETargeting _targeting = static_cast<EAOETargeting>(0);
		EEffectApplication _effectApplication = static_cast<EEffectApplication>(0);
		float _tickInterval = 1.f;
	};
	const static inline defaults _defaults;

protected:
	//void setSword() {

	//}

public:
	virtual void replaceOverrides() override;
	virtual void dynamicDeepCopy(UObject* context) override {}
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EAOEShape _shape = static_cast<EAOEShape>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EAOETargeting _targeting = static_cast<EAOETargeting>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EEffectApplication _effectApplication = static_cast<EEffectApplication>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _tickInterval = SENTINEL_FLOAT;
	UAOEConfig(const FObjectInitializer& init) : Super(init) {}

};
///////////////////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UAOEAttributeData : public UBaseAttributeData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _duration = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _arcShape_angle = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _duration_spawner = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _count_spawner = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _range_spawner = SENTINEL_FLOAT;

	virtual void replaceOverrides() override;
};

template<>
class DefaultProxy<UAOEAttributeData> {
public:
	using self = UAOEAttributeData;
	const static std::unordered_map<float(self::*), float, helpers::MemberPtrHash>& get() {
		const static std::unordered_map<float(self::*), float, helpers::MemberPtrHash> temp = {
			{&self::_duration, 0.f},
			{&self::_arcShape_angle, 90.0f},
			{&self::_duration_spawner, 0.f},
			{&self::_count_spawner, 0.f},
			{&self::_range_spawner, 0.f}
		};
		return temp;
	}
};
///////////////////////////////////////////////////////////////////////////////
#define MYSTAT(X) \
	X(_duration) \
	X(_arcShape_angle) \
	X(_duration_spawner) \
	X(_count_spawner) \
	X(_range)

class AOEAttributes : public BaseAttributes {
	std::weak_ptr<const CombatantAttributes> _attrRef;

	void modifyAttributes(const std::shared_ptr<const CombatantAttributes>&);

public:
	MYSTAT(BASEATTRIBUTES_DECLARE);

	AOEAttributes() = delete;
	AOEAttributes(const AOEAttributes& other);
	AOEAttributes(AOEAttributes&& other);
	AOEAttributes& operator=(AOEAttributes&& other) = delete;
	AOEAttributes(const UAOEAttributeData* attr, std::shared_ptr<const CombatantAttributes> attrRef);
	virtual ~AOEAttributes() = default;

	virtual void tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) override;
	virtual void discretizeFull() override {}
	virtual void applyStatus(UObject* context, const FEffectStruct& status, float delta) override {}
	virtual void applyToAllStats(const std::function<void(Stat&)>& func) override {
		MYSTAT(BASEATTRIBUTES_APPLY);
	}
	virtual void applyToAllStats(const std::function<void(const Stat&)>& func) const override {
		MYSTAT(BASEATTRIBUTES_APPLY);
	}
	virtual bool isCompatibleWith(const UBaseAttributeData* data) const override { return dynamic_cast<const UAOEAttributeData*>(data) != nullptr; }
};
#undef MYSTAT
///////////////////////////////////////////////////////////////////////////////

struct AOEInitStruct {
	AttackInitStruct _attack;
	const UAOEConfig* _AOEConfig;
	const AOEAttributes _AOEAttributes;
	const bool _delayFullConstruction = false;
	const FVector _arcShape_forwardVector = FVector(1, 0, 0);
	AOEInitStruct() = delete;
	AOEInitStruct(const AttackInitStruct& attk, const UAOEConfig* config, const AOEAttributes& attr, bool delay, const FVector& direction) :
		_attack(attk), _AOEConfig(config), _AOEAttributes(attr), _delayFullConstruction(delay), _arcShape_forwardVector(direction){}
};
///////////////////////////////////////////////////////////////////////////////

class AOEFactory : public AttackFactory
{
	const static inline EAOETargeting _RANDOM = EAOETargeting::random;
	const static inline EAOETargeting _BACKSTAB = EAOETargeting::backstab;
	const static inline EAOEShape _ARC = EAOEShape::arc;

	TObjectPtr<const UAOEConfig> _AOEConfig = nullptr;
	std::unique_ptr<BaseAttributeWrapper<AOEAttributes>> _AOEAttributes = nullptr;
	TArray<AAOE*> launchAttack_Impl(const FVector& forward, const AOEInitStruct* init);
protected:
	virtual bool isCompatible(const UAttackLevel* level) const override { return dynamic_cast<const UAOELevel*>(level) != nullptr; }
	virtual void finishUpgrade(const UAttackLevel* newLevel) override;
	void launchAttack(const FVector& forward, const AOEInitStruct* init);

public:
	AOEInitStruct getAOEInit() const;

	AOEFactory() = delete;
	AOEFactory(const AOEFactory& other) = delete;
	AOEFactory& operator=(const AOEFactory& other) = delete;
	AOEFactory(AOEFactory&& other);
	AOEFactory& operator=(AOEFactory&& other) = delete;
	AOEFactory(ACombatant* owner, const UAOETemplate* temp);
	AOEFactory(ACombatant* owner, const UAOEConfig* config, const AOEAttributes& attr);
	virtual TArray<AAOE*> launchAttack_Impl(const FVector& forward);
	virtual void launchAttack(const FVector& forward) override;
	
	virtual void tick(float delta) override;
	bool shouldFollowAim() const;
};
///////////////////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UAOELevel : public UAttackLevel {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAOEAttributeData> _AOEOffsets;

	virtual void replaceOverrides() override {
		Super::replaceOverrides();
		_AOEOffsets->replaceOverrides();
	}
	UAOELevel(const FObjectInitializer& init) : Super(init) {
		_AOEOffsets = init.CreateDefaultSubobject<UAOEAttributeData>(this, "_AOEOffsets");
	}
	virtual void dynamicDeepCopy(UObject* context) override { Super::dynamicDeepCopy(context); }
};

///////////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UAOETemplate : public UAttackTemplate {
	GENERATED_BODY()

	const static inline EAOETargeting _TRAIL = EAOETargeting::trail;

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAOEConfig> _AOEConfig;
	/*UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UAOEAttributeData> _AOEAttributes;*/

	UAOETemplate(const FObjectInitializer& init) : Super(init) {
		_AOEConfig = init.CreateDefaultSubobject<UAOEConfig>(this, "_AOEConfig");
		//_AOEAttributes = init.CreateDefaultSubobject<UAOEAttributeData>(this, "_AOEAttributes");
	}
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override;
	virtual void replaceOverrides() override {
		Super::replaceOverrides();
		_AOEConfig->replaceOverrides();
		//_AOEAttributes->replaceOverrides();
	}
	virtual void dynamicDeepCopy(UObject* context) override {
		_AOEConfig->dynamicDeepCopy(context);
	}
};
