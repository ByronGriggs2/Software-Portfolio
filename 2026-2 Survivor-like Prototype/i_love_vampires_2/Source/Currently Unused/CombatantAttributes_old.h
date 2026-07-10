#pragma once
#include "BaseAttributes.h"
// I may put all of this in the same file. Probably.
#include "Combatant.h"
#include "StatusEnum.h"

class CombatantAttributes : public BaseAttributes {
	const static EStatus _DAMAGE = EStatusEffect::Damage;
	const static EStatus _BLEED = EStatusEffect::Bleed;
	const static EStatus _BURN = EStatusEffect::Burn;

	Stat _maxHP;
	Stat _currentHP;
	Stat _damageReduction_flat;
	Stat _damageReduction_percent;
	Stat _healthRegen_flat;
	Stat _healthRegen_percent;
	Stat _critChance;
	Stat _critMultiplier;
	Stat _attackSpeed;
	Stat _bonusBounces;
	Stat _bonusPierce;
	Stat _bonusProjectiles;
	Stat _projectileSpeed;
	Stat _projectileSize;
	Stat _movementSpeed;
	Stat _range;
	Stat _contactDamage;
	Stat _selfSize;
	Stat _iFrameDuration;

public:
	CombatantAttributes() = delete;
	// later chang eto UCombatantAttributeData
	CombatantAttributes(const UCombatantAttributes* attr);
	virtual void applyStatus(const FStatusEffect* status, float delta) override;
	virtual void applyToAllStats(const std::function<void(Stat&)>& func) override;
};