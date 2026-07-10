#pragma once
#include "CoreMinimal.h"
#include "SpriteEnum.generated.h"

UENUM(BlueprintType)
enum class ESprite : uint8
{
	//Combatants
		//Players
	testPlayer UMETA(DisplayName = "testPlayer"),
	//Enemies
	testEnemy UMETA(DisplayName = "testEnemy"),
	//Projectiles
	testBullet UMETA(DisplayName = "testBullet"),
	fireball UMETA(DisplayName = "fireball"),
	magic_dart UMETA(DisplayName = "magic_dart"),
	//AOE
	explosion UMETA(DisplayName = "explosion"),
	//Other
	testShard UMETA(DisplayName = "testShard"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies (C++)
// testShard
//     - ExperienceShard.h