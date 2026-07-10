#include "MyAttackAttributeSet.h"
#include "AttackActor.h"
#include "CoreMinimal.h"
#include "Definitions.h"
#include "Combatant.h"

bool UMyAttackAttributeSet::initialise_UMyAttackAttributeSet(UCombatantAttributes* combatantAttributes, UAttackAttributes* rowReference, const TSubclassOf<UBaseAttributes>& attributeType) {
	if (!IsValid(combatantAttributes) {
		LOGERROR("UMyAttackAttributeSet::initialise_UMyAttackAttributeSet - combatantAttributes is not valid");
		return false;
	}
	if (!initialise_UMyAttributeSet(rowReference, attributeType)) {
		return false;
	}
	_modifiers = TWeakObjectPtr<UCombatantAttributes>(combatantAttributes);
	return true;
}

bool UMyAttackAttributeSet::updateFromModifiers() {
	if (!_modifiers.IsValid() || !IsValid(_baseAttributes) || !_finalAttributes.IsValid()) {
		return false;
	}
	UAttackAttributes* CDO = _attributeClass.GetDefaultObject<UAttackAttributes>();
	if (CDO == nullptr) {
		LOGERROR("MyAttackAttributeSet::updateFromModifiers - CDO is null");
		return false;
	}
	CDO->modifyAttributes(_modifiers.Get(), _baseAttributes, _finalAttributes.Get());
	return true;
}