#pragma once
#include "CoreMinimal.h"
#include "MyAttributeSet.h"
#include "MyAttackAttributeSet.generated.h"
class UAttackAttributes;
class UCombatantAttributes;

UCLASS()
class UMyAttackAttributeSet : public UMyAttributeSet {
	GENERATED_BODY()

protected:
	virtual bool updateFromModifiers() override;
public:
	bool initialise_UMyAttackAttributeSet(UCombatantAttributes* combatantAttributes, UAttackAttributes* rowReference, const TSubclassOf<UBaseAttributes>& attributeType);
};