#pragma once
#include "CoreMinimal.h"
#include "AOE.h"
#include <vector>
#include "ForcedMovement.h"
#include "Suplex.generated.h"

class AEnemyBase;
class USuplexTemplate;
class SuplexFactory : public AOEFactory {
	const static inline float _DURATION = 0.333f;
	const static inline float _SPLASH_DAMAGE_MULTIPLIER = 0.25f;
	const static inline float _MAXHP_DAMAGE_MULTIPLIER = 0.05f;
	const static inline float _RANGE = 40.0f;

	std::vector<ForcedMovement> _movements;

	void onSlam(AEnemyBase* target);
	bool myGetTarget(AEnemyBase*& ret) const;

public:
	SuplexFactory() = delete;
	SuplexFactory(ACombatant* owner, const USuplexTemplate* temp);
	virtual TArray<AAOE*> launchAttack_Impl(const FVector& forward) override;
	virtual void tick(float delta) override;
	virtual bool canFire() const override;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API USuplexTemplate : public UAOETemplate {
	GENERATED_BODY()

public:
	virtual std::unique_ptr<BaseAttackFactory> createFactory(ACombatant* owner) const override {
		return std::make_unique<SuplexFactory>(owner, this);
	}
};