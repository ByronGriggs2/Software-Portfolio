#include "CombatantAttributes.h"
#include "MyGameplayStatics.h"

CombatantAttributes::CombatantAttributes(const UCombatantAttributes* attr) :
	_maxHP(attr->_maxHP),
	_currentHP(attr->_currentHP),
	_damageReduction_flat(attr->_damageReduction_flat),
	_damageReduction_percent(attr->_damageReduction_percent),
	_healthRegen_flat(attr->_healthRegen_flat),
	_healthRegen_percent(attr->_healthRegen_percent),
	_critChance(attr->_critChance),
	_critMultiplier(attr->_critMultiplier),
	_attackSpeed(attr->_attackSpeed),
	_bonusBounces(attr->_bonusBounces),
	_bonusPierce(attr->_bonusPierce),
	_bonusProjectiles(attr->_bonusProjectiles),
	_projectileSpeed(attr->_projectileSpeed),
	_projectileSize(attr->_projectileSize),
	_movementSpeed(attr->_movementSpeed),
	_range(attr->_range),
	_contactDamage(attr->_contactDamage),
	_selfSize(attr->_selfSize),
	_iFrameDuration(attr->_iFrameDuration)
{
}

void CombatantAttributes::applyToAllStats(const std::function<void(Stat&)>& func) {
	func(_maxHP);
	func(_currentHP);
	func(_damageReduction_flat);
	func(_damageReduction_percent);
	func(_healthRegen_flat);
	func(_healthRegen_percent);
	func(_critChance);
	func(_critMultiplier);
	func(_attackSpeed);
	func(_bonusBounces);
	func(_bonusPierce);
	func(_bonusProjectiles);
	func(_projectileSpeed);
	func(_projectileSize);
	func(_movementSpeed);
	func(_range);
	func(_contactDamage);
	func(_selfSize);
	func(_iFrameDuration);
}

void CombatantAttributes::applyStatus(const FStatusEffect* status, float delta) {
	// Require nothing
	if (stats->_type == _DAMAGE)
		_currentHP._offset -= status->_magnitude;
	else if (stats->_type == _BLEED)
		_currentHP._offset -= status->_magnitude * delta;
	// Require combatant manager
	else if (stats->_type == _BURN || false) {
		UCombatantManager* manager = nullptr;
		if (!MyGameplayStatics::GetCombatantManager(manager)) {
			LOGERROR("CombatantAttributes::applyStatus - could not get combatant manager for burn damage");
			return;
		}
		if (stats->_type == _BURN)
			if (manager->getBurnThisFrame())
				_currentHP._offset -= (status->_magnitude / 100.0f) * _currentHP.getFinal();
	}
	else
		return;
}