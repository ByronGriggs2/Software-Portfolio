#include "BaseAttributeComponent.h"
#include "Definitions.h"
#include "BaseAttributeSet.h"

void UBaseAttributeComponent::zeroOffsets() {
    if (!IsValid(_offsets)) {
        LOGERROR("UBaseAttributeComponent::zeroOffsets - offsets not valid");
        return;
    }
    for (TFieldIterator<FProperty> propIt(_offsets->GetClass()); propIt; ++propIt)
    {
        FProperty* property = *propIt;
        FFloatProperty* floatProp = CastField<FFloatProperty>(property);
        if (floatProp == nullptr)
            continue;
        floatProp->SetFloatingPointPropertyValue(_offsets, 0);
    }
}

void UBaseAttributeComponent::modifyAttributes(ABaseAttributeSet* attr) {
    if (attr != nullptr && !IsValid(attr)) {
        LOGERROR("UBaseAttributeComponent::modifyAttributes - attr is invalid and non-null");
        return;
    }
    LOGERROR("UBaseAttributeComponent::modifyAttributes - \"virtual\" function");
}