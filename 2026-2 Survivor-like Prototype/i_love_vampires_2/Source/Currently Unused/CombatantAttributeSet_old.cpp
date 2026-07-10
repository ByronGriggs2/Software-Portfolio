// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatantAttributeSet.h"

UCombatantAttributeSet::UCombatantAttributeSet()
{
	maxHP.SetBaseValue(1.f);
	damageReduction_flat.SetBaseValue(0.f);
	damageReduction_percent.SetBaseValue(0.f);
	healthRegen_flat.SetBaseValue(0.f);
	healthRegen_percent.SetBaseValue(0.f);
	critChance.SetBaseValue(0.f);
	critMultiplier.SetBaseValue(2.f);
	attackSpeed.SetBaseValue(1.f);
	bonusBounces.SetBaseValue(0.f);
	bonusPierce.SetBaseValue(0.f);
	bonusProjectiles.SetBaseValue(0.f);
	projectileSpeed.SetBaseValue(1.f);
	projectileSize.SetBaseValue(1.f);
	AOESize.SetBaseValue(1.f);
	movementSpeed.SetBaseValue(1.f);
	range.SetBaseValue(1.f);
	contactDamage.SetBaseValue(0.f);
	currentHP.SetBaseValue(1.f);
	selfSize.SetBaseValue(1.f);
	iFrameDuration.SetBaseValue(1.f);
}