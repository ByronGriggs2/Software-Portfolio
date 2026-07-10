#include "EffectStruct.h"
#include "helpers.h"

bool FEffectStruct::operator==(const FEffectStruct& other) const {
	return _type == other._type &&
		helpers::nearEq(_magnitude, other._magnitude) &&
		helpers::nearEq(_duration, other._duration) &&
		helpers::nearEq(_chance, other._chance);
}