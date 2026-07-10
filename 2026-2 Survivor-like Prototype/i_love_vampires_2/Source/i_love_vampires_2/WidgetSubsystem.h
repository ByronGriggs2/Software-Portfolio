#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WidgetSubsystem.generated.h"

class UInputGrabbingPauseWidget;

UCLASS()
class I_LOVE_VAMPIRES_2_API UWidgetSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

	UInputGrabbingPauseWidget* _pauseWidget = nullptr;	
	TWeakObjectPtr<UUserWidget> _rootWidget = nullptr;

public:
	bool registerWidget(UInputGrabbingPauseWidget* widget);
	bool unregisterWidget(UInputGrabbingPauseWidget* widget);
	bool setUIInputEnabled(bool enabled);
	bool getUIInputEnabled() const;

	UFUNCTION(BlueprintCallable) void registerRootWidget(UUserWidget* widget);
};