#include "GIRefSingleton.h"
#include "Engine/GameInstance.h"
#include "Definitions.h"

GIRefSingleton& GIRefSingleton::getInstance(UGameInstance* GI) {
	static GIRefSingleton instance;
	if (IsValid(GI))
		instance._GI = GI;
	return instance;
}

UGameInstance* GIRefSingleton::getGameInstance() {
	return getInstance()._GI;
}