#pragma once
#include "CoreMinimal.h"
#include "Definitions.h"
#include <functional>
#include <unordered_map>
#include "helpers.h"
#include "BaseAttributeData.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UBaseAttributeData : public UObject
{
	GENERATED_BODY()

public:
	virtual void replaceOverrides()
		PURE_VIRTUAL(UBaseAttributeData::replaceOverrides,
			LOGERROR("replaceOverrides not implemented in this class"););
	virtual void applyToAllStats(const std::function<void(float&)>& func);
	virtual void applyToAllStats(const std::function<void(const float&)>& func) const;
	void zeroSentinelOverride();
	UBaseAttributeData(const FObjectInitializer& init) : Super(init) {}
};

template<typename T>
class DefaultProxy {
public:
	const static std::unordered_map<float(T::*), float, helpers::MemberPtrHash>& get() {
		const static std::unordered_map<float(T::*), float, helpers::MemberPtrHash> map;
		return map;
	}
};