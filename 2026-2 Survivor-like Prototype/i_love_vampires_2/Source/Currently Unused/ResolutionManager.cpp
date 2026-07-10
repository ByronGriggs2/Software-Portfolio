#include "ResolutionManager.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"

FIntPoint UResolutionManager::detectResolution() const {
	const UGameUserSettings* settings = GetGameUserSettings();
	return settings->GetDesktopResolution();
}

FIntPoint UResolutionManager::letterboxResolution(const FIntPoint& val) const {

}