#include "MyWeaponAttributeSet.h"
#include "MyCombatantAttributeSet.h"
#include "CombatantAttributes.h"

void UMyWeaponAttributeSet::initialise_UWeaponAttributeSet(const FWeaponAttributes* rowRef, TSharedObjectPtr<UMyCombatantAttributeSet>& modifiers) {
	_modifiers = TWeakObjectPtr<UMyAttributeSet>(modifiers);
	Initialise_UMyAttributeSet(rowRef);
}

void UMyWeaponAttributeSet::updateFromModifiers() {
	// The parent object is in the middle of destruction or creation
	if (!_modifiers.IsValid()) {
		return;
	}
	const FCombatantAttributes& combatantAttributes = _modifiers->getAttributes();
	// damage currently does not have a corresponding attribute
	_finalAttributes._warmup = _baseAttributes._warmup * (1.0f/combatantAttributes.attackSpeed);
	_finalAttributes._critChance = _baseAttributes._critChance + combatantAttributes.critChance;
	_finalAttributes._critMultiplier = _baseAttributes._critMultiplier + combatantAttributes.critMultiplier;
}