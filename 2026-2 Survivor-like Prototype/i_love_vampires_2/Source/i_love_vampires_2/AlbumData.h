#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AlbumData.generated.h"

UCLASS(Blueprintable)
class I_LOVE_VAMPIRES_2_API UAlbumData : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString _albumName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString _artistName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString _hyperlink;
};