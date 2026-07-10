#pragma once
#include "GameFramework/SaveGame.h"
#include "CoreMinimal.h"
#include "KeybindSection.h"
#include "GlobalSave.generated.h"

UCLASS(Blueprintable)
class I_LOVE_VAMPIRES_2_API UGlobalSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FKeybindSaveData _keyboard;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FKeybindSaveData _gamepad;
};