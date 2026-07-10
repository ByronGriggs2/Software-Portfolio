#pragma once
#include "CoreMinimal.h"
#include "AttackActor.h"
#include "EnemyBase.h"
#include "BaseAttributes.h"
#include "Definitions.h"
#include "BaseAttributeWrapper.h"
#include "SingleTarget.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API USingleTargetAttributeData : public UBaseAttributeData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float _range = SENTINEL_FLOAT;
	virtual void replaceOverrides() override;
};

template<>
class DefaultProxy<USingleTargetAttributeData> {
public:
	using self = USingleTargetAttributeData;
	const static std::unordered_map<float(self::*), float, helpers::MemberPtrHash>& get() {
		const static std::unordered_map<float(self::*), float, helpers::MemberPtrHash> temp = {
			{&self::_range, 1.f}
		};
		return temp;
	}
};

#define MYSTAT(X) \
	X(_range)
class SingleTargetAttributes : public BaseAttributes {
	const static inline float _BASE_RANGE = 70.0f;

	std::weak_ptr<const CombatantAttributes> _attrRef;

	void modifyAttributes(const std::shared_ptr<const CombatantAttributes>&);

public:
	MYSTAT(BASEATTRIBUTES_DECLARE);

	SingleTargetAttributes(const USingleTargetAttributeData* attr, std::shared_ptr<const CombatantAttributes> attrRef);
	virtual void tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) override;
	virtual void applyStatus(UObject* context, const FEffectStruct& status, float delta) override {}
	virtual void applyToAllStats(const std::function<void(Stat&)>& func) override {
		MYSTAT(BASEATTRIBUTES_APPLY);
	}
	virtual void applyToAllStats(const std::function<void(const Stat&)>& func) const override {
		MYSTAT(BASEATTRIBUTES_APPLY);
	}
	virtual bool isCompatibleWith(const UBaseAttributeData* data) const override {
		return dynamic_cast<const USingleTargetAttributeData*>(data) != nullptr;
	}
	virtual void discretizeFull() override {}
};
#undef MYSTAT

class USingleTargetTemplate;
class SingleTargetFactory : public AttackFactory {
	const static inline float _AFTERIMAGE_DURATION = 0.25f;

	std::unique_ptr<BaseAttributeWrapper<SingleTargetAttributes>> _singleTargetAttributes = nullptr;
	TMap<TWeakObjectPtr<AAttackActor>, float> _destroyQueue;

	float getRange() const;
	bool getTarget(AEnemyBase*& ret) const;

protected:
	AEnemyBase* launchAttack_internal(const FVector& forward);
	void launchAttack_internal(AEnemyBase* target, const FVector& forward);
	virtual void addToDestroyQueue(AEnemyBase* target);

public:
	virtual bool canFire() const override;
	virtual void launchAttack(const FVector& forward) override;
	void launchAttack(AEnemyBase* target, const FVector& forward);
	virtual void tick(float delta) override;
	SingleTargetFactory(ACombatant* owner, const UAttackTemplate* temp);
};

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API USingleTargetLevel : public UAttackLevel {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<USingleTargetAttributeData> _singleTargetOffsets;
	virtual void replaceOverrides() override {
		Super::replaceOverrides();
		_singleTargetOffsets->replaceOverrides();
	}
	USingleTargetLevel(const FObjectInitializer& init) : Super(init) {
		_singleTargetOffsets = init.CreateDefaultSubobject<USingleTargetAttributeData>(this, "_singleTargetOffsets");
	}
	virtual void dynamicDeepCopy(UObject* context) override { Super::dynamicDeepCopy(context); }
};