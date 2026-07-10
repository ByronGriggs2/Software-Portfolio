#include "StatusEffect_Bleed.h"
#include "Combatant.h"

void UStatusEffect_Bleed::postbonusStep(float delta, ABaseAttributeSet* caller) {
	if (!IsValid(caller)) {
		LOGERROR("UStatusEffect_Bleed::postbonusStep - caller is not valid");
		return;
	}
	UCombatantComponent* component = caller->getComponent<UCombatantComponent>();
	if (!IsValid(component))
		return;
	UCombatantAttributes* offsets = component->getOffsets<UCombatantAttributes>();
	if (!IsValid(offsets))
		return;
	offsets->_currentHP -= _magnitude * delta;
	UStatusEffect::postbonusStep(delta, caller);
}