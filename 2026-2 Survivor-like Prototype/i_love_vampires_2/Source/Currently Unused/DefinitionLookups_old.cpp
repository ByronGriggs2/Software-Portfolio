#include "DefinitionLookups.h"
#include "AttributeSet.h"
#include "CombatantAttributeSet.h"
#include "Definitions.h"

UDefinitionLookups::UDefinitionLookups() {}

namespace {
	const TArray<UClass*> implementedClasses = {
		UCombatantAttributeSet::StaticClass()
	};
	const TMap<FName, FGameplayAttribute> combatantAttributeMap() {
		static TMap<FName, FGameplayAttribute> combatantAttributeMap_internal = {
		{ "maxHP", UCombatantAttributeSet::GetmaxHPAttribute() },
		{ "damageReduction_flat", UCombatantAttributeSet::GetdamageReduction_flatAttribute() },
		{ "damageReduction_percent", UCombatantAttributeSet::GetdamageReduction_percentAttribute() },
		{ "healthRegen_flat", UCombatantAttributeSet::GethealthRegen_flatAttribute() },
		{ "healthRegen_percent", UCombatantAttributeSet::GethealthRegen_percentAttribute() },
		{ "critChance", UCombatantAttributeSet::GetcritChanceAttribute() },
		{ "critMultiplier", UCombatantAttributeSet::GetcritMultiplierAttribute() },
		{ "attackSpeed", UCombatantAttributeSet::GetattackSpeedAttribute() },
		{ "bonusBounces", UCombatantAttributeSet::GetbonusBouncesAttribute() },
		{ "bonusPierce", UCombatantAttributeSet::GetbonusPierceAttribute() },
		{ "bonusProjectiles", UCombatantAttributeSet::GetbonusProjectilesAttribute() },
		{ "projectileSpeed", UCombatantAttributeSet::GetprojectileSpeedAttribute() },
		{ "projectileSize", UCombatantAttributeSet::GetprojectileSizeAttribute() },
		{ "AOESize", UCombatantAttributeSet::GetAOESizeAttribute() },
		{ "movementSpeed", UCombatantAttributeSet::GetmovementSpeedAttribute() },
		{ "range", UCombatantAttributeSet::GetrangeAttribute() },
		{ "contactDamage", UCombatantAttributeSet::GetcontactDamageAttribute() },
		{ "currentHP", UCombatantAttributeSet::GetcurrentHPAttribute() },
		{ "selfSize", UCombatantAttributeSet::GetselfSizeAttribute() },
		{ "iFrameDuration", UCombatantAttributeSet::GetiFrameDurationAttribute() }
		};
		return combatantAttributeMap_internal;
	}
}

TArray<FName> UDefinitionLookups::combatantAttributeKeys() const
{
	TArray<FName> ret;
	combatantAttributeMap().GenerateKeyArray(ret);
	return ret;
}
TArray<FGameplayAttribute> UDefinitionLookups::combatantAttributeValues() const
{
	TArray<FGameplayAttribute> ret;
	combatantAttributeMap().GenerateValueArray(ret);
	return ret;
}

FGameplayAttribute UDefinitionLookups::getCombatantAttributeByName(const FName& name) const
{
	if (const FGameplayAttribute* attribute = combatantAttributeMap().Find(name))
		return *attribute;
	LOGERROR("UDefinitionLookups::getCombatantAttributeByName - no attribute found for name ");
	return FGameplayAttribute();
}

FName UDefinitionLookups::getNameByCombatantAttribute(const FGameplayAttribute& attribute) const
{
	for (const auto& pair : combatantAttributeMap()) {
		if (pair.Value == attribute)
			return pair.Key;
	}
	LOGERROR("UDefinitionLookups::getNameByCombatantAttribute - no name found for attribute ");
	return FName("ERROR");
}

TArray<float> UDefinitionLookups::getFloatPropertiesAsArray(const UObject* object, bool& containsOnlyFloats)
{
	// This function should work properly even if the passed object has non-float properties. However, it will specify in 
	// containsOnlyFloats if there were any non-parsed properties.
	containsOnlyFloats = true;
	TArray<float> ret;
	UClass* objectUClass = object->StaticClass();
	bool foundClass = false;
	for (const UClass* cls : implementedClasses) {
		if (objectUClass->IsChildOf(cls)) {
			foundClass = true;
			break;
		}
	}
	if (!foundClass) {
		LOGERROR("UDefinitionLookups::getFloatPropertiesAsArray - no implemented class found for object of class ");
		return ret;
	}
	for (TFieldIterator<FProperty> PropIt(objectUClass); PropIt; ++PropIt)
	{
		FProperty* tempProperty = *PropIt;
		FStructProperty* structProperty = CastField<FStructProperty>(tempProperty);
		// All non-float properties are skipped, so this function will return only float properties.
		// The exception is struct properties of type FGameplayAttributeData, which are treated as floats by returning their base value.
		if (structProperty){
			if (structProperty->Struct->IsChildOf(FGameplayAttributeData::StaticStruct())) {
				const FGameplayAttributeData* attributeData = structProperty->ContainerPtrToValuePtr<FGameplayAttributeData>(object);
				ret.Add(attributeData->GetBaseValue());
			}
			else
				containsOnlyFloats = false;
			continue;
		}
		FFloatProperty* floatProperty = CastField<FFloatProperty>(tempProperty);
		if (floatProperty) {
			const float* floatValue = floatProperty->ContainerPtrToValuePtr<float>(object);
			ret.Add(*floatValue);
			continue;
		}
		containsOnlyFloats = false;
	}
	return ret;
}