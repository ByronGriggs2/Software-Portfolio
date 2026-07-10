#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BaseTemplate.h"
#include "BaseConfig.h"
#include "BaseAttributeData.h"
#include <type_traits>
#include "UObject/UObjectGlobals.h"
#include "BaseTemplate.h"
#include "DynamicAssetManager.generated.h"

class UInputMappingContext;
class UCombatantTemplate;
class UBaseLevelContainer;

UCLASS()
class I_LOVE_VAMPIRES_2_API UDynamicAssetManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<const TObjectPtr<UBaseTemplate>, UBaseTemplate*> _templateMap;
	UPROPERTY()
	TMap<const TObjectPtr<UObject>, UObject*> _wildcardMap;

	template<typename T, typename U>
	T* registerInternal(T* diskAsset, UObject* caller, TMap<const TObjectPtr<U>, U*>& container);
	template<typename T, typename U>
	T* getKeyInternal(const T* runtimeAsset, const TMap<const TObjectPtr<U>, U*>& container) const;

public:
	UDynamicAssetManager() = default;
	template <typename T>
	T* registerTemplate(T* diskTemplate, UObject*);
	template <typename T>
	T* getKey(const T* runtimeTemplate) const;
	template <typename T>
	T* registerWildcard(const TSoftObjectPtr<T>& diskAsset, UObject* caller);
	template <typename T>
	T* getWildcardKey(const T* runtimeAsset) const;
};

template <typename T, typename U>
T* UDynamicAssetManager::registerInternal(T* diskAsset, UObject* caller, TMap<const TObjectPtr<U>, U*>& container) {
	static_assert(std::is_base_of_v<UObject, U>, "U must be a subclass of UObject");
	static_assert(std::is_base_of_v<U, T>, "T must be a subclass of U");

	if (!IsValid(diskAsset)) {
		LOGERROR("UDynamicAssetManager::registerInternal: Invalid asset provided");
		return nullptr;
	}

	U* myAsset = Cast<U>(diskAsset);

	if (!container.Contains(diskAsset)) {
		U* copy = nullptr;
		{
			if constexpr (TIsDerivedFrom<T, UBaseTemplate>::IsDerived) {
				auto tempA = Cast<UBaseTemplate>(diskAsset);
				if (IsValid(tempA))
					copy = Cast<U>(tempA->createOverrideCopy(caller));
				else {
					LOGERROR("UDynamicAssetManager::registerInternal: Failed to cast disk asset to UBaseTemplate for override copy");
					return nullptr;
				}
			}
			else
				copy = Cast<U>(DuplicateObject<T>(diskAsset, caller));
		}
		if (!IsValid(copy)) {
			LOGERROR("UDynamicAssetManager::registerInternal: Failed to duplicate asset");
			return nullptr;
		}
		container.Add(diskAsset, copy);
		T* ret = Cast<T>(copy);
		if (!IsValid(ret)) {
			LOGERROR("UDynamicAssetManager::registerInternal: Failed to cast new asset to type T");
			return nullptr;
		}
		return ret;
	}
	else {
		U* existingAsset = container[diskAsset];
		T* ret = Cast<T>(existingAsset);
		if (!IsValid(ret)) {
			LOGERROR("UDynamicAssetManager::registerTemplate: Failed to cast existing asset to type T");
			return nullptr;
		}
		return ret;
	}
}
template <typename T>
T* UDynamicAssetManager::registerTemplate(T* diskTemplate, UObject* caller) {
	static_assert(std::is_base_of_v<UBaseTemplate, T>, "T must be a subclass of UBaseTemplate");
	return registerInternal<T, UBaseTemplate>(diskTemplate, caller, _templateMap);
}
// passing this ptr by const reference may help avoid implicit conversions
template <typename T>
T* UDynamicAssetManager::registerWildcard(const TSoftObjectPtr<T>& diskAsset, UObject* caller) {
	static_assert(std::is_base_of_v<UObject, T>, "T must be a subclass of UObject");
	if (diskAsset.Get() == nullptr)
		diskAsset.LoadSynchronous();
	return registerInternal<T, UObject>(diskAsset.Get(), caller, _wildcardMap);
}

template<typename T, typename U>
T* UDynamicAssetManager::getKeyInternal(const T* runtimeAsset, const TMap<const TObjectPtr<U>, U*>& container) const {
	static_assert(std::is_base_of_v<UObject, U>, "U must be a subclass of UObject");
	static_assert(std::is_base_of_v<U, T>, "T must be a subclass of U");
	if (!IsValid(runtimeAsset)) {
		LOGERROR("UDynamicAssetManager::getKeyInternal: Invalid asset provided");
		return nullptr;
	}
	for (const auto& pair : container) {
		if (pair.Value == runtimeAsset) {
			T* ret = Cast<T>(pair.Key);
			if (!IsValid(ret)) {
				LOGERROR("UDynamicAssetManager::getKeyInternal: Failed to cast key asset to type T");
				return nullptr;
			}
			return ret;
		}
	}
	LOGERROR("UDynamicAssetManager::getKeyInternal: No matching key found for provided runtime asset");
	return nullptr;
}

template<typename T>
T* UDynamicAssetManager::getKey(const T* runtimeTemplate) const {
	static_assert(std::is_base_of_v<UBaseTemplate, T>, "T must be a subclass of UBaseTemplate");
	return getKeyInternal<T, UBaseTemplate>(runtimeTemplate, _templateMap);
}
template<typename T>
T* UDynamicAssetManager::getWildcardKey(const T* runtimeAsset) const {
	return getKeyInternal<T, UObject>(runtimeAsset, _wildcardMap);
}