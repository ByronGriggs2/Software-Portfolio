#include "AssetRefs.h"
#include "InputMappingContext.h"
#include "Definitions.h"
#include "Combatant.h"
#include "InputAction.h"
#include "Materials/MaterialInterface.h"
#include "DamageNumber.h"
#include "Engine/Texture2D.h"

UAssetRefs::UAssetRefs() {}

UInputAction* UAssetRefs::getMoveAction() const {
	return _moveAction;
}
UInputAction* UAssetRefs::getCancelAction() const {
	return _cancelAction;
}

UInputMappingContext* UAssetRefs::getActionContext() const {
	return _actionIMC;
}
UInputMappingContext* UAssetRefs::getMenuContext() const {
	return _menuIMC;
}
UCombatantTemplate* UAssetRefs::getBasePlayerTemplate() const {
	return _basePlayerTemplate;
}

UMaterialInterface* UAssetRefs::getSpriteMaterial() const {
	return _translucentMaterial;
}

TSubclassOf<UDamageNumber> UAssetRefs::getDamagePopupWidgetClass() const {
	return _damagePopupWidgetClass;
}

UTexture2D* UAssetRefs::getDefaultIcon() const {
	return _defaultIcon;
}

UMaterialInterface* UAssetRefs::getIconGrayscaleMaterial() const {
	return _iconGrayscaleMaterial;
}

UCombatantTemplate* UAssetRefs::getTestEnemy() const {
	return _testEnemy;
}