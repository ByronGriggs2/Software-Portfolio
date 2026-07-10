#pragma once
#include "CoreMinimal.h"
#include "Definitions.h"
#include "BaseLevelContainer.generated.h"

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API UBaseLevelContainer : public UObject {
	GENERATED_BODY()

public:
	virtual void replaceOverrides()
		PURE_VIRTUAL(UBaseLevelContainer::replaceOverrides,
			LOGERROR("replaceOverrides not implemented in this class"););
	virtual void dynamicDeepCopy(UObject* context)
		PURE_VIRTUAL(UBaseLevelContainer::dynamicDeepCopy,
			LOGERROR("dynamicDeepCopy not implemented in this class"););
};
