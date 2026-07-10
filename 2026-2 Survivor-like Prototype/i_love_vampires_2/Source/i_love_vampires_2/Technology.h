#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/SoftObjectPtr.h"
#include "Technology.generated.h"
class UTexture2D;

//The UBaseTemplate garbage is only necessary when you want to mess with the data at runtime. These can just be standard data assets.
UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UTechnology : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString _name = "_invalid_";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString _desc = "Default Description";
	//UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<TSoftObjectPtr<UTechnology>> _prerequisites = {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UTexture2D> _icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int _price = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int _maxLevel = 1;
};