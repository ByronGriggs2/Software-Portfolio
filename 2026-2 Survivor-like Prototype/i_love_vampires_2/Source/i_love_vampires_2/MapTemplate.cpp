#include "MapTemplate.h"
#include "MyGameplayStatics.h"
#include "AssetRefs.h"
#include "Combatant.h"
#include "unrealHelpers.h"
#include "PaperFlipbook.h"

void UMapTemplate::replaceOverrides() {
	UAssetRefs* refs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(refs)) {
		LOGERROR("UMapTemplate::replaceOverrides - failed to get asset refs");
		return;
	}
	
	if (unrealHelpers::isInvalidData<UCombatantTemplate>(_boss1)) {
		LOGERROR("UMapTemplate::replaceOverrides - boss1 not set");
		_boss1 = refs->getTestEnemy();
	}
	if (unrealHelpers::isInvalidData<UCombatantTemplate>(_boss2)) {
		LOGERROR("UMapTemplate::replaceOverrides - boss2 not set");
		_boss2 = refs->getTestEnemy();
	}
	if (unrealHelpers::isInvalidData<UCombatantTemplate>(_boss3)) {
		LOGERROR("UMapTemplate::replaceOverrides - boss3 not set");
		_boss3 = refs->getTestEnemy();
	}
	if (unrealHelpers::isInvalidData<UPaperFlipbook>(_background)) {
		LOGERROR("UMapTemplate::replaceOverrides - background not set");
		_background = refs->getTestOverlay();
	}
}

void UMapTemplate::dynamicDeepCopy(UObject* context) {
	for (auto& pair : _enemyTemplates) {
		if (IsValid(pair.Value)) {
			pair.Value = unrealHelpers::getDynamicWildcard<UCombatantTemplate>(pair.Value, context);
		}
	}
	if (IsValid(_boss1)) {
		_boss1 = unrealHelpers::getDynamicWildcard<UCombatantTemplate>(_boss1, context);
	}
	if (IsValid(_boss2)) {
		_boss2 = unrealHelpers::getDynamicWildcard<UCombatantTemplate>(_boss2, context);
	}
	if (IsValid(_boss3)) {
		_boss3 = unrealHelpers::getDynamicWildcard<UCombatantTemplate>(_boss3, context);
	}
	if (IsValid(_background)) {
		_background = unrealHelpers::getDynamicWildcard<UPaperFlipbook>(_background, context);
	}
}