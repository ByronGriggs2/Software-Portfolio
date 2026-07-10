#include "DamageEffect.h"
#include "CombatantAttributeSet.h"

void UDamageEffect::PostLoad() {
    Super::PostLoad();
    DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayModifierInfo mod;

    mod.Attribute = UCombatantAttributeSet::GetcurrentHPAttribute();
    mod.ModifierOp = EGameplayModOp::AddFinal;

    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(0.f));
    FSetByCallerFloat tempSetByCaller;
    tempSetByCaller.DataName = FName("UCombatantManager_damageEffect_magnitude");
    mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(tempSetByCaller);
    Modifiers.Add(mod);
}