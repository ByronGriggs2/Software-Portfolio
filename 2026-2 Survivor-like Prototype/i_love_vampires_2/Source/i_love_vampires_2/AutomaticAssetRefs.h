#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <type_traits>
#include "Engine/DataAsset.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/AssetData.h"
#include "GameplayTagContainer.h"
#include "Definitions.h"
#include "ActiveEnum.h"
#include "Passive.h"
#include "AutomaticAssetRefs.generated.h"

class UWeaponTemplate_Base;
class UBaseTemplate;
class UCombatantPassive;
class UPassiveData;
class UTechnology;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UAutomaticAssetRefs : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	const static inline EWeaponType _SWORD = EWeaponType::sword;
	const static inline EWeaponType _GUN = EWeaponType::gun;

	UPROPERTY() TArray<const TObjectPtr<const UWeaponTemplate_Base>> _weapons;
	UPROPERTY() TArray<const TObjectPtr<const UWeaponTemplate_Base>> _swords;
	UPROPERTY() TArray<const TObjectPtr<const UWeaponTemplate_Base>> _guns;
	UPROPERTY() TArray<const TObjectPtr<const UPassiveData>> _passives;
	UPROPERTY() TArray<const TObjectPtr<const UTechnology>> _technologies;

	template <typename T>
	const T* getRandomAsset(const TArray<const TObjectPtr<const T>>& assetArray, const TArray<T*>& ignored);
	void initGunsAndSwords();
	void performWeaponDataValidityCheck();

public:
	UAutomaticAssetRefs();
	UFUNCTION(BlueprintCallable) const UWeaponTemplate* getRandomWeapon(const TArray<UWeaponTemplate*>& ignored);
	UFUNCTION(BlueprintCallable) const UWeaponTemplate* getRandomSword(const TArray<UWeaponTemplate*>& ignored);
	UFUNCTION(BlueprintCallable) const UWeaponTemplate* getRandomGun(const TArray<UWeaponTemplate*>& ignored);
	UFUNCTION(BlueprintCallable) const UPassiveData* getRandomPassive(const TArray<UPassiveData*>& ignored);
	const TArray<const TObjectPtr<const UTechnology>>& getAllTechnologies() const { return _technologies; }
	template <typename T>
	static void loadAssets(TArray<const TObjectPtr<const T>>& out);
};

template <typename T>
void UAutomaticAssetRefs::loadAssets(TArray<const TObjectPtr<const T>>& out) {
	static_assert(std::is_base_of<UDataAsset, T>::value, "T must be a UDataAsset or derived from UDataAsset");

	out.Empty();
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FARFilter Filter;
	Filter.ClassPaths.Add(FTopLevelAssetPath(T::StaticClass()));
	Filter.bRecursiveClasses = true;
	//Filter.bIncludeOnlyOnDiskAssets = true;
	TArray<FAssetData> AssetList;
	AssetRegistryModule.Get().GetAssets(Filter, AssetList);
	for (const FAssetData& Asset : AssetList) {
		const FString PackageShortName = FPackageName::GetShortName(Asset.PackageName.ToString());
		const FString AssetName = Asset.AssetName.ToString();
		if (AssetName != PackageShortName)
			continue;
		if (Asset.FindTag("myTest") || Asset.FindTag("Placeholder"))
			continue;
		TSoftObjectPtr<T> SoftPtr(Asset.ToSoftObjectPath());
		T* AssetPtr = SoftPtr.LoadSynchronous();
		if (IsValid(AssetPtr)) {
			out.Add(AssetPtr);
		}
	}
}

template<typename T>
const T* UAutomaticAssetRefs::getRandomAsset(const TArray<const TObjectPtr<const T>>& assetArray, const TArray<T*>& ignored) {
	static_assert(std::is_base_of<UDataAsset, T>::value, "T must be a UDataAsset or derived from UDataAsset");

	if (assetArray.Num() == 0) {
		LOGERROR("UAutomaticAssetRefs::getRandomAsset - no assets found");
		return nullptr;
	}
	if (ignored.Num() == 0) {
		return assetArray[FMath::RandRange(0, assetArray.Num() - 1)].Get();
	}
	else {
		TArray<const TObjectPtr<const T>> temp;
		for (const auto& asset : assetArray) {
			if (!ignored.Contains(asset.Get())) {
				temp.Add(asset);
			}
		}
		if (temp.Num() == 0) {
			LOGWARNING("UAutomaticAssetRefs::getRandomAsset - no valid assets");
			return nullptr;
		}
		else
			return temp[FMath::RandRange(0, temp.Num() - 1)].Get();
	}
}