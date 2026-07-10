#include "TechWidget.h"
#include "Components/Image.h"
#include "AssetRefs.h"
#include "MyGameplayStatics.h"
#include "UObject/UObjectGlobals.h"
#include "Definitions.h"
#include "Technology.h"

UTechWidget::UTechWidget(const FObjectInitializer& init) : Super(init) {
	_renderedIcon = init.CreateDefaultSubobject<UImage>(this, "_renderedIcon");
}

bool UTechWidget::initialise_UTechWidget(UTechnology* diskData, ETechnologyState state) { 
	_diskData = diskData; 
	_state = state; 

	bool success = false;
	UTexture2D* texture = getDiskTexture(success);
	if (!success)
		return false;
	_renderedIcon->SetBrushFromTexture(texture);
	return true;
}

UTexture2D* UTechWidget::getDiskTexture(bool& success) const {
	if (!IsValid(_diskData)) {
		LOGERROR("UTechWidget::initialise_UTechWidget - diskData is not valid");
		success = false;
		return nullptr;
	}
	if (!IsValid(_diskData->_icon)) {
		LOGWARNING("UTechWidget::initialise_UTechWidget - diskData icon is not valid");
		UAssetRefs* refs = nullptr;
		if (!MyGameplayStatics::getAssetRefs(refs)) {
			LOGERROR("UTechWidget::initialise_UTechWidget - failed to get asset refs");
			success = false;
			return nullptr;
		}
		success = true;
		return refs->getDefaultIcon();
	}
	success = true;
	return _diskData->_icon.Get();
}

void UTechWidget::setState(ETechnologyState newState)
{
	_state = newState;
	StateDetails newDetails;
	switch (_state) {
	case ETechnologyState::locked:
		newDetails._grayscale = true;
		break;
	case ETechnologyState::unlockable:
		newDetails._isDramatic = true;
		break;
	case ETechnologyState::unlocked:
		break;
	case ETechnologyState::hidden:
	case ETechnologyState::INVALID:
	default:
		newDetails._visible = false;
	}
	if (newDetails._visible != _currentDetails._visible)
		setVisible(newDetails._visible);
	if (newDetails._grayscale != _currentDetails._grayscale)
		setGrayscale(newDetails._grayscale);
	if (newDetails._isDramatic != _currentDetails._isDramatic)
		setDramatic(newDetails._isDramatic);
	_currentDetails = newDetails;
}

void UTechWidget::setDramatic(bool val) {

}

void UTechWidget::setVisible(bool val) {
	if (val)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Hidden);
}

void UTechWidget::setGrayscale(bool val) {
	if (val) {
		bool success = false;
		auto texture = getDiskTexture(success);
		if (!success) {
			LOGERROR("UTechWidget::setGrayscale - failed to get texture");
			return;
		}
		UAssetRefs* refs = nullptr;
		if (!MyGameplayStatics::getAssetRefs(refs)) {
			LOGERROR("UTechWidget::setGrayscale - failed to get asset refs");
			return;
		}
		auto mat = refs->getIconGrayscaleMaterial();
		UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(mat, this);
		dynamicMat->SetTextureParameterValue(TEXT("icon"), texture);
		_renderedIcon->SetBrushFromMaterial(dynamicMat);
	}
	else {
		bool success = false;
		auto texture = getDiskTexture(success);
		if (!success) {
			LOGERROR("UTechWidget::setGrayscale - failed to get texture");
			return;
		}
		_renderedIcon->SetBrushFromTexture(texture);
	}
}

void UTechWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	createTooltip();
	return Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

FReply UTechWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		_leftClick.Broadcast(_ID);
		return FReply::Handled();
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && _state == ETechnologyState::unlocked) {
		_rightClick.Broadcast(_ID);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

UTechnology* UTechWidget::getDiskData() const { return _diskData.Get(); }