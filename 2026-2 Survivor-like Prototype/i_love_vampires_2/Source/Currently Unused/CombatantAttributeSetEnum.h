#pragma once
#include <unordered_map>
namespace CombatantAttribute {
	enum class MyEnum {
		maxHP,
		currentHP,
		damageReduction_flat,
		damageReduction_percent,
		healthRegen_flat,
		healthRegen_percent,
		critChance,
		critMultiplier,
		attackSpeed,
		bonusBounces,
		bonusPierce,
		bonusProjectiles,
		projectileSpeed,
		projectileSize,
		movementSpeed,
		range,
		contactDamage,
		selfSize,
		iFrameDuration
	};
	using enum MyEnum;
	const std::unordered_map<MyEnum, float> DEFAULT = {
		{maxHP, 1},
		{currentHP, 1},
		{damageReduction_flat, 0},
		{damageReduction_percent, 0},
		{healthRegen_flat, 0},
		{healthRegen_percent, 0},
		{critChance, 0},
		{critMultiplier, 0},
		{attackSpeed, 0},
		{bonusBounces, 0},
		{bonusPierce, 0},
		{bonusProjectiles, 0},
		{projectileSpeed, 0},
		{projectileSize, 0},
		{movementSpeed, 0},
		{range, 0},
		{contactDamage, 0},
		{selfSize, 0},
		{iFrameDuration, 0}
	};
}
// Dependencies
// attackSpeed
//    - Active.h
// maxHP
//     - MyCombatantAttributeSet.h
//     - StatusEffect_Burn.h
// currentHP
//     - Combatant.h
//     - StatusEffect_Bleed.h
//     - StatusEffect_Burn.h
// contactDamage
//     - Combatant.h
// critChance
//     - Active.h
// critMultiplier
//     - Active.h