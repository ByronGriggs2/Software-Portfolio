#pragma once
#include "CoreMinimal.h"
// AProjectile
#include "AttackActor.h"
#include "ProjectileEnum.h"
// UProjectileConfig
#include "BaseConfig.h"
// UProjectileAttributeData
#include "BaseAttributeData.h"
// ProjectileAttributes
#include "BaseAttributes.h"
#include "Combatant.h"
//
#include "Projectile.generated.h"
class UProjectileConfig;
class UProjectileAttributeData;
class ProjectileAttributes;
struct ProjectileInitStruct;
class AEnemyBase;

UCLASS()
class AProjectile : public AAttackActor {
	GENERATED_BODY()

	const static inline EProjectileShape _CIRCLE = EProjectileShape::circle;

	TWeakObjectPtr<const ACombatant> _targetEnemy = nullptr;
	bool _boomerangActive = false;

protected:
	float _directionX = 0;
	float _directionZ = 1;
	float _pierce = 0;
	float _bounce = 0;
	float _distanceTravelled = 0;

	TObjectPtr<const UProjectileConfig> _projectileConfig = nullptr;
	std::unique_ptr<ProjectileAttributes> _projectileAttributes = nullptr;
	void setTarget(const ACombatant* target) { _targetEnemy = target; }
	const ACombatant* getTarget() const { return _targetEnemy.Get(); }

private:
	bool performSweep(const FVector&, const FVector&, TArray<struct FHitResult>&);
	void executeBounce(AEnemyBase* ineligibleTarget);
	void setNewDirection();

protected:
	virtual void bulletDeath() { 
		_effectedPawns.Empty();
		Destroy();
	}
	// returns true iff bulletDeath was called
	virtual bool handleSweepResults(const TArray<struct FHitResult>& hits);
	// returns true iff bulletDeath was called
	bool handleBouncePierce(ACombatant* ineligibleTarget);

public:
	AProjectile() : AAttackActor() {}
	virtual void BeginPlay() override;
	
	void initialise_AProjectile(const ProjectileInitStruct&);
	// currently unused, since I realized making all random projectiles homing was stupid
	void homeToTarget(ACombatant* homingTarget) {
		_targetEnemy = homingTarget;
		setNewDirection();
	}
	virtual void Tick(float delta) override;
	FVector getDirection() const { return FVector(_directionX, 0, _directionZ).GetSafeNormal(); }
};

///////////////////////////////////////////////////////////////////////////////


UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UProjectileConfig : public UBaseConfig
{
	GENERATED_BODY()

	struct defaults {
		EProjectileShape _shape = static_cast<EProjectileShape>(0);
		EAttackShape _attackShape = static_cast<EAttackShape>(0);
		EProjectileTargeting _targeting = static_cast<EProjectileTargeting>(0);
	};
	const static inline defaults _defaults;

public:
	virtual void replaceOverrides() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EProjectileShape _shape = static_cast<EProjectileShape>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackShape _attackShape = static_cast<EAttackShape>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EProjectileTargeting _targeting = static_cast<EProjectileTargeting>(static_cast<uint8>(255));
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool _isHoming = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool _isBoomerang = false;
	UProjectileConfig(const FObjectInitializer& init) : Super(init) {}
	virtual void dynamicDeepCopy(UObject*) override {}
};
///////////////////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UProjectileAttributeData : public UBaseAttributeData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _spread = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _speed = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _range = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _pierce = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _bounce = SENTINEL_FLOAT;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _projectileCount = SENTINEL_FLOAT;

	UProjectileAttributeData(const FObjectInitializer& init) : Super(init) {}
	virtual void replaceOverrides() override;
};
template<>
class DefaultProxy<UProjectileAttributeData> {
public:
	using self = UProjectileAttributeData;
	const static std::unordered_map<float(self::*), float, helpers::MemberPtrHash>& get() {
		const static std::unordered_map<float(self::*), float, helpers::MemberPtrHash> temp = {
			{&self::_spread, -1.f},
			{&self::_speed, 1.f},
			{&self::_range, 1.f},
			{&self::_pierce, 0.f},
			{&self::_bounce, 0.f},
			{&self::_projectileCount, 1.f}
		};
		return temp;
	}
};
///////////////////////////////////////////////////////////////////////////////
#define MYSTAT(X) \
	X(_spread) \
	X(_speed) \
	X(_range) \
	X(_pierce) \
	X(_bounce) \
	X(_projectileCount)

class ProjectileAttributes : public BaseAttributes {
	const static inline float _PROJECTILE_SPEED = 200.f;
	const static inline float _PROJECTILE_RANGE = 200.f;

	std::weak_ptr<const CombatantAttributes> _attrRef;

	void modifyAttributes(const std::shared_ptr<const CombatantAttributes>&);

public:
	MYSTAT(BASEATTRIBUTES_DECLARE);

	ProjectileAttributes() = delete;
	ProjectileAttributes(const ProjectileAttributes& other);
	ProjectileAttributes(ProjectileAttributes&& other);
	ProjectileAttributes& operator=(const ProjectileAttributes& other) = delete;
	ProjectileAttributes& operator=(ProjectileAttributes&& other) = delete;
	ProjectileAttributes(const UProjectileAttributeData* attr, std::shared_ptr<const CombatantAttributes> attrRef);
	virtual ~ProjectileAttributes() = default;

	virtual void tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) override;
	virtual void discretizeFull() override;
	virtual void applyToAllStats(const std::function<void(Stat&)>& func) override {
		MYSTAT(BASEATTRIBUTES_APPLY);
	}
	virtual void applyToAllStats(const std::function<void(const Stat&)>& func) const override {
		MYSTAT(BASEATTRIBUTES_APPLY);
	}
	virtual void applyStatus(UObject* context, const FEffectStruct& status, float delta) override {}
	virtual bool isCompatibleWith(const UBaseAttributeData* data) const override {
		return dynamic_cast<const UProjectileAttributeData*>(data) != nullptr;
	}
};
#undef MYSTAT
///////////////////////////////////////////////////////////////////////////////

struct ProjectileInitStruct {
	AttackInitStruct _attack;
	const UProjectileConfig* _projectileConfig;
	const ProjectileAttributes _projectileAttributes;
	const float _directionX;
	const float _directionZ;
	ProjectileInitStruct() = delete;
	ProjectileInitStruct(const AttackInitStruct& attack, const UProjectileConfig* config, const ProjectileAttributes& attr, float x, float z) :
		_attack(attack), _projectileConfig(config), _projectileAttributes(attr), _directionX(x), _directionZ(z){}
};
///////////////////////////////////////////////////////////////////////////////

class ProjectileFactory : public AttackFactory
{
	const static inline EProjectileTargeting _SKILLSHOT = EProjectileTargeting::skillshot;
	const static inline EProjectileTargeting _RANDOM_DIRECTION = EProjectileTargeting::random;
	const static inline EProjectileTargeting _RANDOM_ENEMY = EProjectileTargeting::randomEnemy;
	const static inline EProjectileTargeting _SNIPER = EProjectileTargeting::sniperRifle;
	const static inline EAttackShape _FAN = EAttackShape::fan;
	const static inline EAttackShape _RANDOM_SHAPE = EAttackShape::random;
	const static inline EStatus _BLIND = EStatus::blind;
	friend class ProjectileSwordFactory;

	float _directionX = 0;
	float _directionZ = 1;

	static FVector getDirection_random();
	TArray<AProjectile*> launchAttack_internal_projectile(const FVector& forward);

protected:
	TObjectPtr<const UProjectileConfig> _projectileConfig = nullptr;
	std::unique_ptr<BaseAttributeWrapper<ProjectileAttributes>> _projectileAttributes = nullptr;
	//Gives a vector depending on the projectile's targeting type
	FVector getTempForward(const FVector& forward) const;
	//Uses the vector from the targeting type to create the final direction for all fired projectiles
	TArray<FVector> getProjectileDirections(const FVector& tempForward);
	virtual AProjectile* launchSingleProjectile(const FVector& direction);

	virtual void launchAttack(const FVector& forward) override;

	float getDirectionX() const { return _directionX; }
	void setDirectionX(float x) { _directionX = x; }
	float getDirectionZ() const { return _directionZ; }
	void setDirectionZ(float z) { _directionZ = z; }
	ProjectileInitStruct getProjectileInit() const {
		ProjectileAttributes temp(*(_projectileAttributes->getCore()));
		temp.discretizeFull();
		ProjectileInitStruct ret(AttackFactory::getAttackInit(), _projectileConfig.Get(), temp, _directionX, _directionZ);
		return ret;
	}
	virtual bool isCompatible(const UAttackLevel* level) const override { return Cast<UProjectileLevel>(level) != nullptr; }
	virtual void finishUpgrade(const UAttackLevel* newLevel) override;
	

public:
	ProjectileFactory() = delete;
	ProjectileFactory(const ProjectileFactory& other) = delete;
	ProjectileFactory& operator=(const ProjectileFactory& other) = delete;
	ProjectileFactory(ProjectileFactory&& other);
	ProjectileFactory& operator=(ProjectileFactory&& other) = delete;
	ProjectileFactory(ACombatant*, const UProjectileTemplate*);
	virtual void tick(float delta) override;
};
///////////////////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UProjectileLevel : public UAttackLevel {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UProjectileAttributeData> _projectileOffsets;
	virtual void replaceOverrides() override {
		Super::replaceOverrides();
		_projectileOffsets->replaceOverrides();
	}
	UProjectileLevel(const FObjectInitializer& init) : Super(init) {
		_projectileOffsets = init.CreateDefaultSubobject<UProjectileAttributeData>(this, "_projectileOffsets");
	}
	virtual void dynamicDeepCopy(UObject* context) override { Super::dynamicDeepCopy(context); }
};

///////////////////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UProjectileTemplate : public UAttackTemplate {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UProjectileConfig> _projectileConfig;
	//UPROPERTY(EditAnywhere, Instanced)
	//TObjectPtr<UProjectileAttributeData> _projectileAttributes;

public:
	UProjectileTemplate(const FObjectInitializer& init) : Super(init) {
		_projectileConfig = init.CreateDefaultSubobject<UProjectileConfig>(this, "_projectileConfig");
		//_projectileAttributes = init.CreateDefaultSubobject<UProjectileAttributeData>(this, "_projectileAttributes");
	}
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override;
	virtual void replaceOverrides() override {
		Super::replaceOverrides();
		_projectileConfig->replaceOverrides();
		//_projectileAttributes->replaceOverrides();
	}
	virtual void dynamicDeepCopy(UObject* context) override {
		if (!IsValid(_projectileConfig)) {
			LOGERROR("Invalid projectile config in dynamicDeepCopy");
			return;
		}
		Super::dynamicDeepCopy(context);
		_projectileConfig->dynamicDeepCopy(context);
	}
};

