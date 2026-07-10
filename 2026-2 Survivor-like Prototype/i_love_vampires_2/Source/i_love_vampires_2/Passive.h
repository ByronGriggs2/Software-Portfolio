#pragma once
#include "CoreMinimal.h"
#include "BaseLevelContainer.h"
#include "BaseTemplate.h"
#include "Passive.generated.h"
class UCombatantAttributeData;

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UPassiveLevelData : public UBaseLevelContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TObjectPtr<UCombatantAttributeData> _prebonus;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TObjectPtr<UCombatantAttributeData> _postbonus;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TObjectPtr<UCombatantAttributeData> _multiplier;

	UPassiveLevelData(const FObjectInitializer& init);
	virtual void replaceOverrides() override;
	//std::vector<Stat> getStatVector() const;
	virtual void dynamicDeepCopy(UObject*) override {}
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UPassiveData : public UBaseTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName _name = "Passive";
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<TObjectPtr<UPassiveLevelData>> _levels;
	virtual void replaceOverrides() override;
	virtual void dynamicDeepCopy(UObject* context) override;
};
class Passive {
	int _level = 0;
	TArray<const TObjectPtr<const UPassiveLevelData>> _levels = {};
	TObjectPtr<const UPassiveData> _data = nullptr;
public:
	Passive() = delete;
	Passive(const UPassiveData* data);
	UPassiveData* getDiskData() const;
	bool isUpgradable() const { return (_level + 1) <= _levels.Num() - 1; }
	void upgrade();
	const UCombatantAttributeData* getPrebonus() const;
	const UCombatantAttributeData* getPostbonus() const;
	const UCombatantAttributeData* getMultiplier() const;
	int getLevel() const { return _level; }
	bool operator==(UPassiveData* other) const { return getDiskData() == other; }
};