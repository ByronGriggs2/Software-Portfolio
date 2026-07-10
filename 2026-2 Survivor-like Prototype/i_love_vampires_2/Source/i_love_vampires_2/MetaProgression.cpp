#include "MetaProgression.h"
#include "Definitions.h"
#include "TechWidget.h"
#include "Technology.h"
#include "Components/Image.h"
#include "MetaProgressionEnums.h"

void UMetaProgression::NativeOnInitialized() {
	int i = 0;
	for (const auto& tech : _techWidgets) {
		tech->_leftClick.AddDynamic(this, &UMetaProgression::onLeftClick);
		tech->_rightClick.AddDynamic(this, &UMetaProgression::onRightClick);
		tech->giveID(i);
		i++;
	}
}

void UMetaProgression::onLeftClick(int techID) {
	TObjectPtr<UTechWidget> tech = _techWidgets[techID];
	if (!IsValid(tech)) {
		LOGERROR("UMetaProgression::onLeftClick - tech widget is not valid");
		return;
	}
	const UTechnology* techData = tech->getDiskData();
	if (!IsValid(techData)) {
		LOGERROR("UMetaProgression::onLeftClick - techData is not valid");
		return;
	}
	const int price = techData->_price;
	if (_resourcePoints < price) {
		notEnoughResource();
		return;
	}
	switch (tech->getState()) {
	case ETechnologyState::unlockable:
		break;
	case ETechnologyState::unlocked:
		if (techData->_maxLevel <= tech->getLevel())
			return;
		break;
	case ETechnologyState::hidden:
	case ETechnologyState::locked:
	default:
		return;
		break;
	}
	_resourcePoints -= techData->_price;
	tech->upgrade();
}