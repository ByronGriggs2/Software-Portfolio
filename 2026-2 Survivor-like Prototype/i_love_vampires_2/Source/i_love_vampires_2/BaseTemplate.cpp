#include "BaseTemplate.h"
#include "UObject/UObjectGlobals.h"

UBaseTemplate* UBaseTemplate::createOverrideCopy(UObject* caller) const {
	UBaseTemplate* ret = DuplicateObject<UBaseTemplate>(this, GetOuter());
	ret->dynamicDeepCopy(caller);
	ret->replaceOverrides();
	return ret;
}
