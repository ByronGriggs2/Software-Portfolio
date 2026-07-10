#include "MyCombatantAttributeSet.h"
#include "StatusEffect_Burn.h"
#include "Combatant.h"

void UMyCombatantAttributeSet::burnTick() {
	const TArray<UStatusEffect*>& statusEffects = getStatusEffects();
	for (const auto& statusEffect : statusEffects) {
		UStatusEffect_Burn* burnEffect = Cast<UStatusEffect_Burn>(statusEffect);
		if (burnEffect != nullptr) {
			burnEffect->burnTick();
		}
	}
}
