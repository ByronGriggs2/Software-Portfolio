#include "BaseAttributes.h"
#include "Definitions.h"
#include "Math/UnrealMathUtility.h"

UBaseAttributes* UBaseAttributes::getDiscretizedCopy(UObject* outer) const {
	LOGERROR("This is a \"virtual\" function");
	if (!IsValid(outer)) {
		LOGERROR("UBaseAttributes::getDiscretizedCopy - outer not valid");
		return nullptr;
	}
	return DuplicateObject<UBaseAttributes>(this, outer, FName());
}

