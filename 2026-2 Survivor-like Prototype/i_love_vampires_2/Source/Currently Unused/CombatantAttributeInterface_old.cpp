#include "CombatantAttributeInterface.h"
#include <unordered_map>
#include "CombatantAttributeList.h"
#include "Definitions.h"


TArray<FName> CombatantAttributeInterface::getNames() const {
	if (!safetyCheck()) {
		return TArray<FName>();
	}
	TArray<FName> ret;
	// pair is std::pair
	for (const auto& pair : COMBATANT_NAMES) {
		ret.Add(FName(COMBATANT_NAMES.at(pair.first)));
	}
	return ret;
}

TArray<float> CombatantAttributeInterface::getValues() const {
	if (!safetyCheck()) {
		return TArray<float>();
	}
	std::unordered_map<CombatantAttributeEnum, float> temp;
	if (!_attributes->getCurrentAttributes(temp)) {
		LOGERROR("CombatantAttributeInterface::getValues - failed to get current attributes");
		return TArray<float>();
	}
	TArray<float> ret;
	// pair is std::pair
	for (const auto& pair : temp) {
		ret.Add(pair.second);
	}
	return ret;
}

TMap<FName, float> CombatantAttributeInterface::getMap() const {
	if (!safetyCheck()) {
		return TMap<FName, float>();
	}
	TMap<FName, float> ret;
	// pair is std::pair
	for (const auto& pair : COMBATANT_DEFINITIONS) {
		std::string strName = COMBATANT_NAMES.at(pair.first);
		if (strName.empty()) {
			LOGERROR("CombatantAttributeInterface::getMap - failed to get name for attribute");
			continue;
		}
		FName name = FName(strName.c_str());
		float val;
		if (!_attributes->getAttributeValue(pair.first, val)) {
			LOGERROR("CombatantAttributeInterface::getMap - failed to get attribute value for attribute");
			continue;
		}
		ret.Add(name, val);
	}
	return ret;
}

void CombatantAttributeInterface::setBaseAttributes(const TMap<FName, float>& newAttributes) {
	if (!safetyCheck()) {
		return;
	}
	std::unordered_map<CombatantAttributeEnum, float> newAttrMap;
	// pair is TPair
	for (const auto& pair : newAttributes) {
		FString fStrName = pair.Key.ToString();
		std::string strName(TCHAR_TO_UTF8(*fStrName));

		bool found = false;
		CombatantAttributeEnum attrEnum = static_cast<CombatantAttributeEnum>(0);
		// pair is std::pair
		for (const auto& namePair : COMBATANT_NAMES) {
			if (namePair.second == strName) {
				attrEnum = namePair.first;
				found = true;
				break;
			}
		}
		if (!found) {
			LOGERROR("CombatantAttributeInterface::setBaseAttributes - failed to find attribute enum for name");
			continue;
		}
		newAttrMap.emplace(attrEnum, pair.Value);
	}
	if (!_attributes->setBaseAttributes(newAttrMap)) {
		LOGERROR("CombatantAttributeInterface::setBaseAttributes - failed to set base attributes");
	}
}

bool CombatantAttributeInterface::safetyCheck() const {
	if (_attributes == nullptr) {
		LOGERROR("CombatantAttributeInterface::safetyCheck - _attributes is null");
		return false;
	}
	return true;
}