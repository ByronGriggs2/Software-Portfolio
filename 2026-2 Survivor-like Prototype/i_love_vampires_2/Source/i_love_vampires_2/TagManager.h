#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Tickable.h"
#include "MyGameplayTag.h"
#include <memory>
#include <type_traits>
#include <vector>
#include "TagManager.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API UTagManager : public UObject, public FTickableGameObject {
	GENERATED_BODY()

	std::vector<std::unique_ptr<MyGameplayTag>> _tags;

public:
	virtual ETickableTickType GetTickableTickType() const override { return IsTemplate() ? ETickableTickType::Never : ETickableTickType::Always; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(A, STATGROUP_Tickables); }

	virtual void Tick(float DeltaTime) override;
	void addTag(std::unique_ptr<MyGameplayTag> tag) { _tags.push_back(std::move(tag)); }
	template<typename T>
	bool hasTag() const;
	template<typename T>
	bool removeTags();
	template<typename T>
	std::vector<T> getTags() const;
};

template<typename T>
bool UTagManager::hasTag() const {
	static_assert(std::is_base_of_v<MyGameplayTag, T>, "hasTag template parameter must be a subclass of MyGameplayTag");
	for (const auto& tag : _tags) {
		if (dynamic_cast<T*>(tag.get()) != nullptr) {
			return true;
		}
	}
	return false;
}

template<typename T>
bool UTagManager::removeTags() {
	static_assert(std::is_base_of_v<MyGameplayTag, T>, "removeTags template parameter must be a subclass of MyGameplayTag");
	bool ret = false;
	for (auto it = _tags.begin(); it != _tags.end();) {
		if (dynamic_cast<T*>(it->get()) != nullptr) {
			it = _tags.erase(it);
			ret = true;
		}
		else {
			++it;
		}
	}
	return ret;
}

template<typename T>
std::vector<T> UTagManager::getTags() const {
	static_assert(std::is_base_of_v<MyGameplayTag, T>, "getTags template parameter must be a subclass of MyGameplayTag");
	std::vector<T> tags;
	for (const auto& tag : _tags) {
		if (T* castedTag = dynamic_cast<T*>(tag.get())) {
			tags.push_back(*castedTag);
		}
	}
	return tags;
}