#pragma once
#include "CoreMinimal.h"
#include "MyAttributeSet.h"
#include "AOEAttributes.h"
class UMyCombatantAttributeSet;

class UMyAOEAttributeSet : public UMyAttributeSet<FAOEAttributes>
{
protected:
	virtual void updateFromModifiers() override;

public:
	UMyAOEAttributeSet() = default;
	void initialise_UMyAOEAttributeSet(const FAOEAttributes*, TSharedPtr<UMyCombatantAttributeSet>&);
};