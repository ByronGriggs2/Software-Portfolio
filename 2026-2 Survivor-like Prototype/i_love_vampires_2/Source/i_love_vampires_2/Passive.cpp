#include "Passive.h"
#include "Combatant.h"
#include "Definitions.h"
#include "MyGameplayStatics.h"
#include "DynamicAssetManager.h"

UPassiveLevelData::UPassiveLevelData(const FObjectInitializer& init) : Super(init) {
	_prebonus = init.CreateDefaultSubobject<UCombatantAttributeData>(this, "_prebonus");
	_postbonus = init.CreateDefaultSubobject<UCombatantAttributeData>(this, "_postbonus");
	_multiplier = init.CreateDefaultSubobject<UCombatantAttributeData>(this, "_multiplier");
}

void UPassiveLevelData::replaceOverrides() {
	_prebonus->zeroSentinelOverride();
	_postbonus->zeroSentinelOverride();
	_multiplier->zeroSentinelOverride();
}

void UPassiveData::replaceOverrides() {
	// The defaults are applied to the base values, these are offsets so the default is 0.
	for (auto& level : _levels) {
		level->replaceOverrides();
	}
}
void UPassiveData::dynamicDeepCopy(UObject* context) {
	for (auto& level : _levels) {
		if (!IsValid(level)) {
			LOGERROR("Attempting to copy UPassiveData with invalid level data");
			continue;
		}
		level->dynamicDeepCopy(context);
	}
}
Passive::Passive(const UPassiveData* data) : _data(data) {
	if (!IsValid(data)) {
		LOGERROR("Attempting to construct Passive with invalid data");
		return;
	}
	for (const auto& level : data->_levels) {
		if (!IsValid(level)) {
			LOGERROR("Attempting to construct Passive with invalid level data");
			return;
		}
		_levels.Add(level);
	}
}
UPassiveData* Passive::getDiskData() const {
	UDynamicAssetManager* manager = nullptr;
	if (!MyGameplayStatics::getDynamicAssetManager(GWorld, manager)) {
		LOGERROR("Passive::getDiskData - failed to get dynamic asset manager");
		return nullptr;
	}
	return manager->getKey(_data.Get());
}
void Passive::upgrade() {
	if (!isUpgradable()) {
		LOGERROR("Passive::upgrade - passive is not upgradable");
		return;
	}
	_level++;
}
const UCombatantAttributeData* Passive::getPrebonus() const {
	if (_level < 0 || _level > _levels.Num() - 1 || _levels.Num() == 0) {
		LOGERROR("Passive::getPrebonus - invalid levels");
		return nullptr;
	}
	return _levels[_level]->_prebonus;
}
const UCombatantAttributeData* Passive::getPostbonus() const {
	if (_level < 0 || _level > _levels.Num() - 1 || _levels.Num() == 0) {
		LOGERROR("Passive::getPostbonus - invalid levels");
		return nullptr;
	}
	return _levels[_level]->_postbonus;
}
const UCombatantAttributeData* Passive::getMultiplier() const {
	if (_level < 0 || _level > _levels.Num() - 1 || _levels.Num() == 0) {
		LOGERROR("Passive::getMultiplier - invalid levels");
		return nullptr;
	}
	return _levels[_level]->_multiplier;
}