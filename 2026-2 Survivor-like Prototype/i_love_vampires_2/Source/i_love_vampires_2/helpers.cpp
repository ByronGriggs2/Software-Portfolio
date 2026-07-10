#include "helpers.h"
#include <cmath>
#include "Definitions.h"

bool helpers::nearEq(float left, float right) {
	return std::abs(left - right) < EPSILON;
}