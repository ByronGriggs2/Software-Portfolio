#include "MyProjectileAttributeSet.h"
#include "MyCombatantAttributeSet.h"
#include "CombatantAttributes.h"

void UMyProjectileAttributeSet::initialise_UProjectileAttributeSet(const FProjectileAttributes* rowRef, TSharedPtr<UMyCombatantAttributeSet>& modifiers) {
	_modifiers = TWeakObjectPtr<UMyAttributeSet>(modifiers);
	Initialise_UMyAttributeSet(rowRef);
}

void UMyProjectileAttributeSet::updateFromModifiers() {
	// The parent object is in the middle of destruction or creation (or there's a bug)
	if (!_modifiers.IsValid()) {
		return;
	}
	const FCombatantAttributes& combatantAttributes = _modifiers->getAttributes();
	_finalAttributes._radius = _baseAttributes._radius * combatantAttributes.projectileSize;
	_finalAttributes._speed = _baseAttributes._speed * combatantAttributes.projectileSpeed;
	_finalAttributes._pierce = _baseAttributes._pierce + combatantAttributes.bonusPierce;
	_finalAttributes._bounce = _baseAttributes._bounce + combatantAttributes.bonusBounces;
	_finalAttributes._projectileCount = _baseAttributes._projectileCount + combatantAttributes.bonusProjectiles;
}