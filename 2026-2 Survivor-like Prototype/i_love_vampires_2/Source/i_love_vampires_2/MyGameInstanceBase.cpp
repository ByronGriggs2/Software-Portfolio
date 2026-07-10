#include "MyGameInstanceBase.h"
#include "GIRefSingleton.h"

UMyGameInstanceBase::UMyGameInstanceBase() : Super() {
	GIRefSingleton::getInstance(this);
}