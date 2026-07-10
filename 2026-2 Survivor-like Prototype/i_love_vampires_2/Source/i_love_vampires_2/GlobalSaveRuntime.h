#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KeybindSection.h"
#include "GlobalSaveRuntime.generated.h"
class UGlobalSave;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UGlobalSaveRuntime : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	KeybindSection _keyboard;
	KeybindSection _gamepad;

public:
	UFUNCTION(BlueprintPure)
	UGlobalSave* getGlobalSave() const;
	UFUNCTION(BlueprintCallable)
	void loadGlobalSave(UGlobalSave* save);

	TMap<FKey, TArray<EInput_UI>> getUIReverseMap(bool isGamepad) const { return isGamepad ? _gamepad.getUIReverseMap() : _keyboard.getUIReverseMap(); }
};