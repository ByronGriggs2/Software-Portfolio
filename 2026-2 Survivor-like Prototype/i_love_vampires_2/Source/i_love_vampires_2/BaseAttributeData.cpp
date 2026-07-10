#include "BaseAttributeData.h"

void UBaseAttributeData::applyToAllStats(const std::function<void(float&)>& func) {
	for (TFieldIterator<FProperty> It(GetClass()); It; ++It)
	{
		if (FFloatProperty* FloatProp = CastField<FFloatProperty>(*It)) {
			float* Value = FloatProp->ContainerPtrToValuePtr<float>(this);
			func(*Value);
		}
	}
}
void UBaseAttributeData::applyToAllStats(const std::function<void(const float&)>& func) const {
	for (TFieldIterator<FProperty> It(GetClass()); It; ++It)
	{
		if (FFloatProperty* FloatProp = CastField<FFloatProperty>(*It)) {
			const float* Value = FloatProp->ContainerPtrToValuePtr<float>(this);
			func(*Value);
		}
	}
}
void UBaseAttributeData::zeroSentinelOverride() {
	applyToAllStats([](float& stat) {
		if (stat == SENTINEL_FLOAT) {
			stat = 0;
		}
		});
}