#include "StatusEffect_Damage.h"
#include "Combatant.h"

void UStatusEffect_Damage::postbonusStep(float delta, ABaseAttributeSet* caller) {
	if (!IsValid(caller)) {
		LOGERROR("UStatusEffect_Damage::postbonusStep - caller is not valid");
		return;
	}
	UCombatantComponent* component = caller->getComponent<UCombatantComponent>();
	if (!IsValid(component))
		return;
	UCombatantAttributes* offsets = component->getOffsets<UCombatantAttributes>();
	if (!IsValid(offsets))
		return;
	offsets->_currentHP -= _magnitude;
	UStatusEffect::postbonusStep(delta, caller);
}