#include "MyCombatantAttributeSet.h"
#include "Definitions.h"
#include "StatusEffect_Burn.h"

MyCombatantAttributeSet::MyCombatantAttributeSet() {
	// pair is std::pair
	const auto& myDefault = CombatantAttribute::DEFAULT;
	for (const auto& pair : myDefault) {
		_attributes.emplace(pair.first, MyAttribute(pair.second, 0.0f, 0.0f, 0.0f));
	}
}
bool MyCombatantAttributeSet::getAttributeValue(CombatantAttribute::MyEnum attributeName, float& ret) const {
	auto it = _attributes.find(attributeName);
	if (it == _attributes.end()) {
		return false;
	}
	const MyAttribute& attr = it->second;
	ret = (attr._baseValue + attr._prebonus) * (1.0f + attr._premultiplier) + attr._postbonus;
	return true;
}

void MyCombatantAttributeSet::inflictStatus(std::unique_ptr<StatusEffect> statusEffect) {
	_activeStatusEffects.push_back(std::move(statusEffect));
	std::unordered_map<CombatantAttribute::MyEnum, float> currentAttributes;
	if (!getCurrentAttributes(currentAttributes)) {
		LOGERROR("Failed to get current attributes when inflicting status effect.");
		return;
	}
	auto modifiers = statusEffect->applyEffect(currentAttributes);
	addModifiers(modifiers);

	StatusEffect_Burn* burnEffect = dynamic_cast<StatusEffect_Burn*>(_activeStatusEffects.back().get());
	if (burnEffect != nullptr) {
		burnEffect->setBurnBoolean(_burnThisFrame.get());
	}
}

bool MyCombatantAttributeSet::getCurrentAttributes(std::unordered_map<CombatantAttribute::MyEnum, float>& ret) const {
	bool success = true;
	for (const auto& attr : _attributes) {
		if (!getAttributeValue(attr.first, ret[attr.first])) {
			LOGERROR("Failed to get attribute value for attribute");
			success = false;
			continue;
		}
	}
	return success;
}

void MyCombatantAttributeSet::tick(float deltaTime) {
	std::unordered_map<CombatantAttribute::MyEnum, float> currentAttributes;
	if (!getCurrentAttributes(currentAttributes)) {
		LOGERROR("Failed to get current attributes when inflicting status effect.");
		return;
	}
	for (auto it = _activeStatusEffects.begin(); it != _activeStatusEffects.end();) {
		std::unique_ptr<StatusEffect>& effect = *it;
		if (effect->getDuration() <= 0) {
			auto modifiers = effect->removeEffect(currentAttributes);
			addModifiers(modifiers);
			it = _activeStatusEffects.erase(it);
		}
		else {
			auto modifiers = (*it)->tick(currentAttributes, deltaTime);
			addModifiers(modifiers);
			it++;
		}
	}
	if (*_burnThisFrame) {
		*_burnThisFrame = false;
	}
}

void MyCombatantAttributeSet::addModifiers(const std::unordered_map<CombatantAttribute::MyEnum, AttributeModifier>& modifiers) {
	for (const auto& mod : modifiers) {
		auto it = _attributes.find(mod.first);
		if (it == _attributes.end()) {
			LOGERROR("Invalid attribute modifier for attribute");
			continue;
		}
		float oldVal;
		if (!getAttributeValue(mod.first, oldVal)) {
			LOGERROR("Failed to get attribute value for attribute");
			continue;
		}
		MyAttribute& attr = it->second;
		attr._prebonus += mod.second._prebonus;
		attr._premultiplier += mod.second._premultiplier;
		attr._postbonus += mod.second._postbonus;
		float newVal;
		if (!getAttributeValue(mod.first, newVal)) {
			LOGERROR("Failed to get attribute value for attribute");
			continue;
		}
		if (_onAttributeChangedCallback) {
			_onAttributeChangedCallback(mod.first, oldVal, newVal);
		}
	}
}

void MyCombatantAttributeSet::dealDamage(float damage) {
	auto it = _attributes.find(_CURRENT_HP);
	if (it == _attributes.end()) {
		LOGERROR("Current HP attribute not found when applying damage.");
		return;
	}
	float oldVal;
	if (!getAttributeValue(_CURRENT_HP, oldVal)) {
		LOGERROR("Failed to get current HP value when applying damage.");
		return;
	}
	it->second._postbonus -= damage;
	float newVal;
	if (!getAttributeValue(_CURRENT_HP, newVal)) {
		LOGERROR("Failed to get current HP value after applying damage.");
		return;
	}
	if (_onAttributeChangedCallback) {
		_onAttributeChangedCallback(_CURRENT_HP, oldVal, newVal);
	}
}

// This function constructs an alternative _attributes so that if an error is encountered halfway through, the original _attributes is left unchanged.
// There's not currently any error checking (because I couldn't think of an error that could occur) but there you go.
bool MyCombatantAttributeSet::setBaseAttributes(const std::unordered_map<CombatantAttribute::MyEnum, float>& newAttributes) {
	std::unordered_map<CombatantAttribute::MyEnum, MyAttribute> newAttrMap;
	// pair is std::pair
	const auto& myDefault = CombatantAttribute::DEFAULT;
	for (const auto& pair : myDefault) {
		auto it = newAttributes.find(pair.first);
		if (it == newAttributes.end()) {
			LOGERROR("Missing attribute in setBaseAttributes");
			return false;
		}
	}
	// pair is std::pair
	for (const auto& pair : newAttributes) {
		auto it = _attributes.find(pair.first);
		
		float prebonus;
		float premultiplier;
		float postbonus;
		if (it == _attributes.end()) {
			const MyAttribute& oldAttr = it->second;
			prebonus = oldAttr._prebonus;
			premultiplier = oldAttr._premultiplier;
			postbonus = oldAttr._postbonus;
		}
		else {
			prebonus = 0.0f;
			premultiplier = 0.0f;
			postbonus = 0.0f;
		}
		newAttrMap.emplace(pair.first, MyAttribute(pair.second, prebonus, premultiplier, postbonus));
	}
	_attributes = std::move(newAttrMap);
	return true;
}

void MyCombatantAttributeSet::burnTick() {
	if (_burnThisFrame)
		*_burnThisFrame = true;
}