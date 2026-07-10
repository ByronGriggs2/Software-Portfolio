#include "Active.h"
#include "Engine/AssetManager.h"
#include "Definitions.h"
#include "Combatant.h"
#include "GameFramework/Pawn.h"
#include "unrealHelpers.h"
#include "helpers.h"
#include "Misc/DataValidation.h"
#include "Engine/Texture2D.h"
#include "AssetRefs.h"
#include "AOE.h"
#include "TestingDefinitions.h"

void Active::tick(float delta, const FVector& forwardVector) {
	if (!_owner.IsValid()) {
		return;
	}
	//If this is performance intensive I can change the trigger to onAttributeChanged
	_factory->tick(delta);
	
	if (_weaponTemplate->_activationTrigger == _PERIODIC || _weaponTemplate->_activationTrigger == _AUTOCHARGE) {
		updateWarmup(delta);
		if (_weaponTemplate->_activationTrigger == _PERIODIC && _chargeRatio >= 1)
			activate(forwardVector);
		else if (_weaponTemplate->_activationTrigger == _AUTOCHARGE && _chargeRatio >= 1 && _factory->canFire())
			activate(forwardVector);
	}
	else if (_weaponTemplate->_activationTrigger == _MANUAL) {
		// Do nothing, the weapon will be activated by the player calling activate()
	}
	else {
		LOGERROR("Active::tick - activation trigger not implemented");
	}

	for (int i = 0; i < _queuedAttacks.size(); i+=0) {
		_queuedAttacks[i] -= delta;
		if (_queuedAttacks[i] <= 0) {
			_factory->launchAttack(forwardVector);
			_queuedAttacks.erase(_queuedAttacks.begin() + i);
		}
		else {
			i++;
		}
	}

	// This class does not have a dedicated attribute set because the overhead is too intense, so it's handled here
	for (int i = 0; i < _statusEffects.Num(); i += 0) {
		_statusEffects[i]._duration -= delta;
		if (_statusEffects[i]._duration <= -EPSILON) {
			_statusEffects.RemoveAt(i);
		}
		else {
			i++;
		}
	}
}

UWeaponTemplate* Active::getDiskTemplate() const {
	UDynamicAssetManager* manager = nullptr;
	if (!MyGameplayStatics::getDynamicAssetManager(_owner.Get(), manager)) {
		LOGERROR("Active::getDiskTemplate - failed to get dynamic asset manager");
		return nullptr;
	}
	return manager->getKey(_weaponTemplate.Get());
}

bool Active::hasStatus(EStatus status) const {
	for (const auto& effect : _statusEffects) {
		if (effect._type == status)
			return true;
	}
	return false;
}

bool Active::operator==(UWeaponTemplate* other) const {
	return getDiskTemplate() == other;
}
bool Active::operator==(const Active& other) const {
	return getDiskTemplate() == other.getDiskTemplate();
}

void Active::updateWarmup(float delta) {
	if (!_owner.IsValid())
		return;
	if (QUICK_ATTACK) {
		_chargeRatio += delta;
	}
	else {
		float newWarmup = getPeriod();
		const float timeSinceLastActivation = _chargeRatio * newWarmup + delta;
		_chargeRatio = timeSinceLastActivation / newWarmup;
	}
}

void Active::activate(const FVector& forward) {
	if (_chargeRatio < 1 - EPSILON)
		return;
	if (_weaponTemplate->_activationType == _SINGLE)
		_factory->launchAttack(forward);
	else if (_weaponTemplate->_activationType == _BURST) {
		auto casted = dynamic_cast<AttackFactory*>(_factory.get());
		if (casted == nullptr) {
			LOGERROR("Active::activate - burst is only supported for subclasses of AttackFactory. Cancelling attack.");
			return;
		}
		const int attackNum = casted->getBurstCount();
		if (attackNum > 0)
			_factory->launchAttack(forward);
		for (int i = 1; i < attackNum; i++) {
			_queuedAttacks.push_back(i*_weaponTemplate->_burstInterval);
		}
	}
	else {
		LOGERROR("Active::activate - invalid activation type");
		return;
	}
	if (_weaponTemplate->_activationTrigger == _PERIODIC || _weaponTemplate->_activationTrigger == _AUTOCHARGE)
		_chargeRatio = 0;
	else if (_weaponTemplate->_activationTrigger == _MANUAL) {}
	else {
		LOGERROR("Active::activate - invalid activation trigger");
		return;
	}
}

Active::Active(ACombatant* owner, const UWeaponTemplate* rawData) : _owner(owner), _weaponTemplate(rawData)
{
	if (!IsValid(owner) || !IsValid(rawData) || !IsValid(rawData->_attackData)) {
		LOGERROR("Active::Active - invalid parameters");
		return;
	}
	{
		auto casted = Cast<UAttackTemplate>(rawData->_attackData);
		if (IsValid(casted)) {
			if (!IsValid(casted->_attackConfig)) {
				LOGERROR("Active::Active - invalid attack config");
				return;
			}
			if (casted->_attackConfig->_isSingleTarget && rawData->_activationTrigger != _AUTOCHARGE) {
				LOGERROR("Active::Active - single target attacks must be autocharge.");
				return;
			}
		}
	}
	_factory = std::move(rawData->_attackData->createFactory(_owner.Get()));
	//warmup
	if constexpr (FORCE_START_OFF_COOLDOWN) {
		_chargeRatio = 1;
	}
	else {
		_chargeRatio = QUICK_ATTACK || _weaponTemplate->_startOnCooldown ? 0 : 1;
	}
}

//template stuff
Active::Active(Active&& other) :
	_chargeRatio(other._chargeRatio),
	_factory(std::move(other._factory)),
	_owner(other._owner),
	_weaponTemplate(other._weaponTemplate)
{
	other._owner = nullptr;
	other._weaponTemplate = nullptr;
}

Active& Active::operator=(Active&& other) {
	if (this != &other) {
		_chargeRatio = other._chargeRatio;
		_factory = std::move(other._factory);
		_owner = other._owner;
		_weaponTemplate = other._weaponTemplate;
		other._owner = nullptr;
		other._weaponTemplate = nullptr;
	}
	return *this;
}
Active_WeaponBase& Active_WeaponBase::operator=(Active_WeaponBase&& other) {
	if (this != &other) {
		Active::operator=(std::move(other));
		_evolutionTurnedDown = other._evolutionTurnedDown;
	}
	return *this;
}

void UWeaponTemplate::replaceOverrides() {
	if (unrealHelpers::isInvalidData(_name))
		_name = _defaults._name;
	if (helpers::isInvalidData(_warmup))
		_warmup = _defaults._warmup;
	if (unrealHelpers::isInvalidData(_activationType))
		_activationType = _defaults._activationType;
	if (unrealHelpers::isInvalidData(_activationTrigger))
		_activationTrigger = _defaults._activationTrigger;
	if (helpers::isInvalidData(_burstInterval))
		_burstInterval = _defaults._burstInterval;
	if (unrealHelpers::isInvalidData(_icon)) {
		UAssetRefs* refs = nullptr;
		if (!MyGameplayStatics::getAssetRefs(refs)) {
			LOGERROR("UWeaponTemplate::replaceOverrides - failed to get asset refs");
			return;
		}
		_icon = refs->getDefaultIcon();
	}
	_attackData->replaceOverrides();
}

void Active::inflictStatus(const FEffectStruct& status) {
	if (status._type == _CHILL) {
		_statusEffects.Add(status);
	}
	_factory->inflictStatus(status);
}

EWeaponType Active::getWeaponType() const {
	if (!IsValid(_weaponTemplate)) {
		LOGERROR("Active::getWeaponType - invalid weapon template");
		return static_cast<EWeaponType>(0);
	}
	return _weaponTemplate->_weaponType;
}

void UWeaponTemplate::dynamicDeepCopy(UObject* context) {
	if (!IsValid(_attackData)) {
		LOGERROR("UWeaponTemplate::dynamicDeepCopy - invalid parameter");
		return;
	}
	_attackData->dynamicDeepCopy(context);
	// This sentinel value (nullptr) happens to be incompatible with the dynamic asset manager, so we skip it if sentinel is detected.
	// This is fine only because the sentinel does not reference a disk asset
	if (_icon.Get() == nullptr) {
		_icon.LoadSynchronous();
	}
	if (_icon.Get() != nullptr)
		_icon = unrealHelpers::getDynamicWildcard(_icon, context);
}

bool Active::isUpgradable() const {
	if (_factory.get() == nullptr) {
		LOGERROR("Active::isUpgradable - uninitialized factory");
		return false;
	}
	return _factory->getLevel() < _factory->getMaxLevel();
}

UWeaponTemplate_Evolution* Active_WeaponBase::getEvolution() const {
	auto casted = Cast<UWeaponTemplate_Base>(_weaponTemplate);
	if (!IsValid(casted)) {
		LOGERROR("Active::getEvolution - weapon template is not a valid");
		return nullptr;
	}
	if (!IsValid(casted->_evolution.Get())) {
		casted->_evolution.LoadSynchronous();
	}
	return casted->_evolution.Get();
}

bool Active_WeaponBase::isEvolutionEligible() const { 
	if (!IsValid(_weaponTemplate)) {
		LOGERROR("Active::isEvolutionEligible - invalid weapon template");
		return false;
	}
	if (unrealHelpers::hasTag(_weaponTemplate->_assetTags, "Placeholder")) {
		return false;
	}
	auto casted = Cast<UWeaponTemplate_Base>(_weaponTemplate);
	if (!IsValid(casted))
		return false;
	if (!IsValid(casted->_evolution.Get())) {
		casted->_evolution.LoadSynchronous();
		if (!IsValid(casted->_evolution.Get())) {
			LOGERROR("Active::isEvolutionEligible - invalid evolution template");
			return false;
		}
	}
	if (unrealHelpers::hasTag(casted->_evolution->_assetTags, "Placeholder")) {
		return false;
	}
	return !isUpgradable(); 
}

bool Active::setLevel(int newLevel) {
	if (_factory.get() == nullptr) {
		LOGERROR("Active::setLevel - uninitialized factory");
		return false;
	}
	_factory->setLevel(newLevel);
	return true;
}

bool Active::isAOE() const {
	if (_factory.get() == nullptr) {
		LOGERROR("Active::isAOE - uninitialized factory");
		return false;
	}
	return dynamic_cast<AOEFactory*>(_factory.get()) != nullptr;
}

bool Active::getAOERange(float& ret) const {
	if (_factory.get() == nullptr) {
		LOGERROR("Active::getRange - uninitialized factory");
		return false;
	}
	auto casted = dynamic_cast<AOEFactory*>(_factory.get());
	if (casted == nullptr) {
		LOGERROR("Active::getRange - factory is not an AOEFactort");
		return false;
	}
	ret = casted->getAttackAttributeWrapper().getMember(&AttackAttributes::_radius);
	return true;
}

float Active::getPeriod() const {
	const float newAttackSpeed = _owner->getAttributeMember(&CombatantAttributes::_attackSpeed);
	const float baseWarmup = _weaponTemplate->_warmup;
	float ret = baseWarmup * (1.0f / newAttackSpeed);
	if (hasStatus(_CHILL)) {
		ret *= 1 + _statusEffects[0]._magnitude / 100;
	}
	return ret;
}
