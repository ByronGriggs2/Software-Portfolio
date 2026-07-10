#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Definitions.h"
class UObject;
class UEnhancedInputLocalPlayerSubsystem;
class APlayerController;
class UGameInstance;
class UAssetRefs;
class UCombatantManager;
//class USpriteManager;
class ACombatGameModeBase;
class UDynamicAssetManager;
class USpriteSorter;
class UAutomaticAssetRefs;
class UWidgetSubsystem;

class MyGameplayStatics {
public:
	MyGameplayStatics() = delete;
	static bool myGetEnhancedInputLocalPlayerSubsystem(const UObject* caller, UEnhancedInputLocalPlayerSubsystem*&);
	static bool myGetPlayerController(const UObject* caller, APlayerController*&);
	static bool myGetGameInstance(UGameInstance*&);
	static bool getAssetRefs(UAssetRefs*&);
	static bool getAutomaticAssetRefs(UAutomaticAssetRefs*&);
	static bool getCombatantManager(const UObject* caller, UCombatantManager*&);
	//static bool getSpriteManager(const UObject* caller, USpriteManager*&);
	static bool getDynamicAssetManager(const UObject* caller, UDynamicAssetManager*&);
	static bool getSpriteSorter(const UObject* caller, USpriteSorter*& ret);
	static bool getCombatGameMode(const UObject* caller, ACombatGameModeBase*& ret);
	static bool getWidgetSubsystem(UWidgetSubsystem*& ret);
};