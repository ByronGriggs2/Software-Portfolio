#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputEnum.h"
#include "Kismet/GameplayStatics.h"
#include "InputGrabbingUserWidget.generated.h"
struct FInputActionValue;
class UMyGameInstanceBase;

UCLASS(Blueprintable)
class I_LOVE_VAMPIRES_2_API UInputGrabbingUserWidget : public UUserWidget
{
	GENERATED_BODY()

	UMyGameInstanceBase* _gameInstanceCache = nullptr;

public:
	UInputGrabbingUserWidget(const FObjectInitializer& init) : Super(init) { SetIsFocusable(true); }
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool handleInput_Implementation(EInput_UI input) { return false; }
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//Hacky but hey Unreal is hacky
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override { /*SetFocus();*/ Super::NativeTick(MyGeometry, InDeltaTime); }
	UFUNCTION(BlueprintNativeEvent) bool handleInput(EInput_UI input);
	UFUNCTION(BlueprintCallable) virtual bool isReady() const;
};

UCLASS(Blueprintable)
class I_LOVE_VAMPIRES_2_API UInputGrabbingPauseWidget : public UInputGrabbingUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool handleInput_Implementation(EInput_UI input) override { return Super::handleInput_Implementation(input); }
};

