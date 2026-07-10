// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "menuFunctions.generated.h"


/**
 * 
 */
enum test {a,b,c};

UCLASS()
class I_LOVE_VAMPIRES_2_API UmenuFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void createPreparationMenu(FString& saveSlot);
	void testFunc() {
		test var;
		someFunc(var);
	}
	void someFunc(test v) {
		if (v == a) {
			return;
		}
	}
};
