#include "ContainerTemplate.h"
#include "unrealHelpers.h"
#include "Definitions.h"

void UContainerTemplate::replaceOverrides() {
	if (!slotsAreValid())
		return;
	if (unrealHelpers::isInvalidData(_type))
		_type = _defaults._type;
	_slot0->replaceOverrides();
	_slot1->replaceOverrides();
	_slot2->replaceOverrides();
}
void UContainerTemplate::dynamicDeepCopy(UObject* context) {
	if (!slotsAreValid())
		return;
	_slot0->dynamicDeepCopy(context);
	_slot1->dynamicDeepCopy(context);
	_slot2->dynamicDeepCopy(context);
}
bool UContainerTemplate::slotsAreValid() const {
	bool ret = IsValid(_slot0) && IsValid(_slot1) && IsValid(_slot2);
	if (!ret) {
		LOGERROR("UContainerTemplate - invalid slot");
	}
	return ret;
}