#include "AutomaticAssetRefs.h"
#include "Active.h"
#include "Combatant.h"
#include "Technology.h"

UAutomaticAssetRefs::UAutomaticAssetRefs() {
	loadAssets<UWeaponTemplate_Base>(_weapons);
	performWeaponDataValidityCheck();
	loadAssets<UPassiveData>(_passives);
	loadAssets<UTechnology>(_technologies);
	initGunsAndSwords();
}

const UWeaponTemplate* UAutomaticAssetRefs::getRandomWeapon(const TArray<UWeaponTemplate*>& ignored) {
	TArray<UWeaponTemplate_Base*> ignoredBase;
	for (UWeaponTemplate* weapon : ignored)
		ignoredBase.Add(Cast<UWeaponTemplate_Base>(weapon));
	return getRandomAsset<UWeaponTemplate_Base>(_weapons, ignoredBase);
}
const UWeaponTemplate* UAutomaticAssetRefs::getRandomSword(const TArray<UWeaponTemplate*>& ignored) {
	TArray<UWeaponTemplate_Base*> ignoredBase;
	for (UWeaponTemplate* weapon : ignored)
		ignoredBase.Add(Cast<UWeaponTemplate_Base>(weapon));
	return getRandomAsset<UWeaponTemplate_Base>(_swords, ignoredBase);
}
const UWeaponTemplate* UAutomaticAssetRefs::getRandomGun(const TArray<UWeaponTemplate*>& ignored) {
	TArray<UWeaponTemplate_Base*> ignoredBase;
	for (UWeaponTemplate* weapon : ignored)
		ignoredBase.Add(Cast<UWeaponTemplate_Base>(weapon));
	return getRandomAsset<UWeaponTemplate_Base>(_guns, ignoredBase);
}
const UPassiveData* UAutomaticAssetRefs::getRandomPassive(const TArray<UPassiveData*>& ignored) {
	return getRandomAsset<UPassiveData>(_passives, ignored);
}
void UAutomaticAssetRefs::initGunsAndSwords() {
	for (const auto& weapon : _weapons) {
		if (!IsValid(weapon)) {
			LOGERROR("UAutomaticAssetRefs::UAutomaticAssetRefs - invalid weapon in _weapons");
			continue;
		}
		switch (weapon->_weaponType) {
		case EWeaponType::sword:
			_swords.Add(weapon);
			break;
		case EWeaponType::gun:
			_guns.Add(weapon);
			break;
		default:
			LOGWARNING("UAutomaticAssetRefs::UAutomaticAssetRefs - weapon with invalid type in _weapons");
			break;
		}
	}
}

void UAutomaticAssetRefs::performWeaponDataValidityCheck() {
	for (const auto& weapon : _weapons) {
		// Note that the lambdas after the first assume valid ptrs
		auto checkType = [](const UWeaponTemplate* weapon) {
			if (weapon->_weaponType != _SWORD && weapon->_weaponType != _GUN) {
				LOGERROR("UAutomaticAssetRefs::performWeaponDataValidityCheck - weapon with invalid type");
				return false;
			}
			return true;
			};
		auto checkSoftPtr = [](const UWeaponTemplate_Base* weapon) {
			if (weapon == nullptr) {
				LOGERROR("UAutomaticAssetRefs::performWeaponDataValidityCheck - soft pointer not set or not loaded");
				return false;
			}
			return true;
			};
		auto checkSoftPtrE = [](TSoftObjectPtr<UWeaponTemplate_Evolution> evo) {
			if (evo.Get() == nullptr)
				evo.LoadSynchronous();
			if (evo.Get() == nullptr) {
				LOGERROR("UAutomaticAssetRefs::performWeaponDataValidityCheck - soft pointer not set");
				return false;
			}
			return true;
			};
		auto checkSymmetry = [](const UWeaponTemplate_Base* weapon, const UWeaponTemplate_Base* sister) {
			bool ret = true;
			if ((weapon->_weaponType == _SWORD && sister->_weaponType != _GUN) || (weapon->_weaponType == _GUN && sister->_weaponType != _SWORD)) {
				LOGERROR("UAutomaticAssetRefs::performWeaponDataValidityCheck - sister weapon has different type");
				ret = false;
			};
			if (!IsValid(sister->_sister.Get()) || sister->_sister.Get()->GetName() != weapon->GetName()) {
				LOGERROR("UAutomaticAssetRefs::performWeaponDataValidityCheck - sister weapon pairing is not symmetric");
				ret = false;
			}
			return ret;
			};
		auto checkSymmetryE = [](const UWeaponTemplate_Base* weapon, const UWeaponTemplate_Evolution* evolution) {
			bool ret = true;
			if (weapon->_weaponType != evolution->_weaponType) {
				LOGERROR("UAutomaticAssetRefs::performWeaponDataValidityCheck - evolution weapon has different type");
				ret = false;
			};
			if (!IsValid(evolution->_baseWeapon.Get()) || evolution->_baseWeapon.Get()->GetName() != weapon->GetName()) {
				LOGERROR("UAutomaticAssetRefs::performWeaponDataValidityCheck - evolution weapon pairing is not symmetric");
				ret = false;
			}
			return ret;
			};

		auto sister = weapon->_sister.Get();
		auto evolution = weapon->_evolution;

		bool success = true;
		success &= checkSoftPtr(sister);
		success &= checkSoftPtrE(evolution);
		if (!success)
			continue;

		success &= checkType(weapon);
		success &= checkType(sister);
		success &= checkType(evolution.Get());
		success &= checkSymmetry(weapon, sister);
		success &= checkSymmetryE(weapon, evolution.Get());
		if (!success) {
			LOGERROR("Data validation failed for this weapon");
			continue;
		}
	}
}