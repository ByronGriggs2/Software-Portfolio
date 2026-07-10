#include "StatusEffect_Burn.h"
#include "Combatant.h"

void UStatusEffect_Burn::postbonusStep(float delta, ABaseAttributeSet* caller) {
	if (!IsValid(caller)) {
		LOGERROR("UStatusEffect_Burn::postbonusStep - caller is not valid");
		return;
	}
	if (_burnThisFrame) {
		UCombatantComponent* component = caller->getComponent<UCombatantComponent>();
		if (!IsValid(component))
			return;
		const UCombatantAttributes* myFinal = component->getFinal<UCombatantAttributes>();
		if (!IsValid(myFinal))
			return;
		UCombatantAttributes* offsets = component->getOffsets<UCombatantAttributes>();
		if (!IsValid(offsets))
			return;
		offsets->_currentHP -= (_magnitude / 100.0f) * myFinal->_maxHP;
		_burnThisFrame = false;
	}
	UStatusEffect::postbonusStep(delta, caller);
}