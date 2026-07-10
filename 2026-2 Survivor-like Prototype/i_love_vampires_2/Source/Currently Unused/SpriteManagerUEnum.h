#pragma once
#include "CoreMinimal.h"
#include "SpriteManagerUEnum.generated.h"

UENUM(BlueprintType)
enum class ESprite : uint8
{
	testPlayer UMETA(DisplayName = "testPlayer"),
	testEnemy UMETA(DisplayName = "testEnemy"),
	testBullet UMETA(DisplayName = "testBullet"),
	testShard UMETA(DisplayName = "testShard")
};