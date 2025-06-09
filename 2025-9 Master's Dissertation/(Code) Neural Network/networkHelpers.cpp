#include "pch.h"
#include "networkHelpers.h"
#include "definitions.h"

float networkHelpers::getInitializationValue() {
	return rand() % (2 * static_cast<int>(helpers::actionCount)) - 2 * static_cast<float>(helpers::actionCount);
}