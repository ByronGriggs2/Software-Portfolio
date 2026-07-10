#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SpriteEnum.h"
#include "SpriteManager.generated.h"
class UPaperFlipbook;

UCLASS()
class I_LOVE_VAMPIRES_2_API USpriteManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ESprite, UPaperFlipbook*> _sprites;

public:
	USpriteManager();
	bool getSprite(ESprite sprite, UPaperFlipbook*&) const;
};