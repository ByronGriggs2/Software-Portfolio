#include "MyAOEAttributeSet.h"
#include "CombatantAttributes.h"
#include "MyCombatantAttributeSet.h"

void UMyAOEAttributeSet::initialise_UMyAOEAttributeSet(const FAOEAttributes* rowReference, TSharedPtr<UMyCombatantAttributeSet>& modifiers) {
	_modifiers = TWeakPtr<UMyAttributeSet>(modifiers);
	initialise_UMyAttributeSet(rowReference);
}

void UMyAOEAttributeSet::updateFromModifiers() {
	// The parent object is in the middle of destruction or creation (or there's a bug)
	if (!_modifiers.IsValid()) {
		return;
	}
	const FCombatantAttributes& combatantAttributes = _modifiers->getAttributes();
	_finalAttributes._radius = _baseAttributes._radius * combatantAttributes.projectileSize;
}