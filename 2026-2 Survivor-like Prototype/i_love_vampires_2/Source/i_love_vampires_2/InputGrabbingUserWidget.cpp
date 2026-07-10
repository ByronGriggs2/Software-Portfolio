#include "InputGrabbingUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayer.h"
#include "AssetRefs.h"
#include "Definitions.h"
#include "InputActionValue.h"
#include "MyGameInstanceBase.h"
#include "MyGameplayStatics.h"
#include "GlobalSaveRuntime.h"
#include "WidgetSubsystem.h"

FReply UInputGrabbingUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled().SetUserFocus(GetCachedWidget().ToSharedRef());
}

void UInputGrabbingUserWidget::NativeConstruct() {
	Super::NativeConstruct();
	APawn* player = UGameplayStatics::GetPlayerPawn(this, 0);
	AMyPlayer* myPlayer = Cast<AMyPlayer>(player);
	UGameInstance* instance = nullptr;
	if (!MyGameplayStatics::myGetGameInstance(instance)) {
		LOGERROR("UInputGrabbingUserWidget::NativeConstruct - failed to get game instance");
		return;
	}
	_gameInstanceCache = Cast<UMyGameInstanceBase>(instance);
	if (!IsValid(_gameInstanceCache)) {
		LOGERROR("UInputGrabbingUserWidget::NativeConstruct - failed to cast game instance to UMyGameInstanceBase");
		RemoveFromParent();
		return;
	}
	if (IsValid(myPlayer)) {
		if (!myPlayer->setIMCEnabled(AMyPlayer::inputContext::action, false)) {
			LOGERROR("UInputGrabbingUserWidget::NativeConstruct - failed to set menu input context enabled");
		}
	}
	UWidgetSubsystem* sub = nullptr;
	if (!MyGameplayStatics::getWidgetSubsystem(sub))
		return;
	sub->setUIInputEnabled(true);
	SetFocus();
}

bool UInputGrabbingUserWidget::isReady() const {
	if (!IsValid(_gameInstanceCache)) {
		LOGERROR("UInputGrabbingUserWidget::isReady - game instance cache is not valid");
		return false;
	}
	return true;
}

void UInputGrabbingUserWidget::NativeDestruct() {
	APawn* player = UGameplayStatics::GetPlayerPawn(this, 0);
	AMyPlayer* myPlayer = Cast<AMyPlayer>(player);

	if (IsValid(myPlayer) && !myPlayer->setIMCEnabled(AMyPlayer::inputContext::action, true)) {
		LOGERROR("UInputGrabbingUserWidget::~UInputGrabbingUserWidget - failed to set menu input context disabled");
	}
	UWidgetSubsystem* sub = nullptr;
	if (!MyGameplayStatics::getWidgetSubsystem(sub))
		return;
	sub->setUIInputEnabled(false);
	Super::NativeDestruct();
}

FReply UInputGrabbingUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	if (!IsValid(_gameInstanceCache))
		return FReply::Unhandled();

	FKey key = InKeyEvent.GetKey();
	UGlobalSaveRuntime* subsystem = _gameInstanceCache->GetSubsystem<UGlobalSaveRuntime>();
	if (!IsValid(subsystem)) {
		LOGERROR("UInputGrabbingUserWidget::NativeOnKeyDown - failed to get global save runtime subsystem");
		SetFocus();
		return FReply::Unhandled();
	}
	// Need to change this for controller support
	auto map = subsystem->getUIReverseMap(false);
	if (map.Contains(key)) {
		bool handled = false;
		for (EInput_UI input : map[key]) {
			handled = handled || handleInput(input);
		}
		if (handled) {
			SetFocus();
			return FReply::Handled();
		}
	}
	SetFocus();
	return FReply::Unhandled();
}

void UInputGrabbingPauseWidget::NativeConstruct() {
	UWidgetSubsystem* guard = nullptr;
	if (MyGameplayStatics::getWidgetSubsystem(guard)) {
		if (IsValid(guard) && guard->registerWidget(this))
			UGameplayStatics::SetGamePaused(this, true);
	}
	Super::NativeConstruct();
}

void UInputGrabbingPauseWidget::NativeDestruct() {
	UWidgetSubsystem* guard = nullptr;
	if (MyGameplayStatics::getWidgetSubsystem(guard)) {
		if (IsValid(guard) && guard->unregisterWidget(this))
			UGameplayStatics::SetGamePaused(this, false);
	}
	Super::NativeDestruct();
}