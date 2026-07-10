#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Definitions.h"
#include "BaseConfig.generated.h"

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UBaseConfig : public UObject {
	GENERATED_BODY()

public:
	// When you create a dynamic copy of a UDataAsset, if any of its members (or members' members) are references to another UDataAsset on disk, you should set the member to a dynamic copy of that disk asset
	// inside this function (which is only called by a dynamic copy)
	virtual void dynamicDeepCopy(UObject* context)
		PURE_VIRTUAL(UBaseConfig::dynamicDeepCopy,
			LOGERROR("dynamicDeepCopy not implemented in this class"););
	virtual void replaceOverrides()
		PURE_VIRTUAL(UBaseConfig::replaceOverrides,
			LOGERROR("replaceOverrides not implemented in this class"););
	UBaseConfig(const FObjectInitializer& init) : Super(init) {}
};