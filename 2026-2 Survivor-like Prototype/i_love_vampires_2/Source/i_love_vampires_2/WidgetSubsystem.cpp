#include "WidgetSubsystem.h"
#include "Definitions.h"
#include "InputGrabbingUserWidget.h"

bool UWidgetSubsystem::registerWidget(UInputGrabbingPauseWidget* widget) {
	if (IsValid(_pauseWidget)) {
		LOGERROR("WidgetSubsystem::registerWidget - already has a registered widget");
		return false;
	}
	_pauseWidget = widget;
	return true;
}

bool UWidgetSubsystem::unregisterWidget(UInputGrabbingPauseWidget* widget) {
	if (!IsValid(_pauseWidget)) {
		LOGERROR("WidgetSubsystem::unregisterWidget - no registered widget to unregister");
		return false;
	}
	if (_pauseWidget != widget) {
		LOGERROR("WidgetSubsystem::unregisterWidget - widget to unregister does not match registered widget");
		return false;
	}
	_pauseWidget = nullptr;
	return true;
}

bool UWidgetSubsystem::setUIInputEnabled(bool enabled) {
	if (!_rootWidget.IsValid()) {
		LOGERROR("WidgetSubsystem::setUIInputEnabled - root widget is not valid");
		return false;
	}
	if (enabled) {
		_rootWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		_rootWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	return true;
}

bool UWidgetSubsystem::getUIInputEnabled() const {
	if (!_rootWidget.IsValid()) {
		LOGERROR("WidgetSubsystem::getUIInputEnabled - root widget is not valid");
		return false;
	}
	return _rootWidget->GetVisibility() == ESlateVisibility::Visible;
}

void UWidgetSubsystem::registerRootWidget(UUserWidget* widget) {
	// root widget needs to be re-registered when level changes
	/*if (_rootWidget.IsValid()) {
		LOGERROR("WidgetSubsystem::registerRootWidget - root widget is already registered");
		return;
	}*/
		_rootWidget = widget;
}