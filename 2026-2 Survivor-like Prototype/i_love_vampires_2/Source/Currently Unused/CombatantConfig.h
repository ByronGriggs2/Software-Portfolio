#pragma once

#include "CoreMinimal.h"
#include "SpriteEnum.h"
#include "CombatantConfig.generated.h"
USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FCombatantConfig
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	FString _name = "Combatant";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	ESprite _sprite = static_cast<ESprite>(0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FCombatantTemplate")
	TArray<FName> _startingWeapons;
};