#pragma once
#include "CoreMinimal.h"
#include "FriendlyCombatant.h"
#include "Summon.generated.h"

class USummonTemplate;
// Should use a dynamic copy
struct SummonInitStruct {
	TObjectPtr<const USummonTemplate> _template;
	int _level;
	float _duration;
};

UCLASS()
class I_LOVE_VAMPIRES_2_API ASummon : public AFriendlyCombatant {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<const USummonTemplate> _template = nullptr;
	float _duration = 0;

protected:
	const TObjectPtr<const USummonTemplate>& getTemplate() const { return _template; }
	virtual void setupFlipbook() override;
	virtual void getOverlappingActors(TSet<AActor*>& ret) override;

public:
	bool initialise_ASummon(const SummonInitStruct& init);
	bool setLevel(int newLevel);
	int getMaxLevel() const;
	virtual void myInitialise(UCombatantTemplate* temp) override;
	// deliberately non-virtual
	void Tick(float delta) override;
protected:
	// this is the virtual tick
	virtual void Tick_Impl(float delta) {}
};