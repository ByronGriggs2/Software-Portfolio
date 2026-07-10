#include "SpriteManager.h"
#include "PaperFlipbook.h"
#include "Definitions.h"

namespace {
	const TMap<ESprite, const char*> SPRITE_PATH = {
		{ESprite::testPlayer, "/Game/Sprites/athena.athena"},
		{ESprite::testEnemy, "/Game/Sprites/enemy.enemy"},
		{ESprite::testBullet, "/Game/Sprites/placeholder_bullet.placeholder_bullet"},
		{ESprite::testShard, "/Game/Sprites/shard.shard"},
		{ESprite::fireball, "/Game/Sprites/fireball.fireball"},
		{ESprite::explosion, "/Game/Sprites/explosion.explosion"},
		{ESprite::magic_dart, "/Game/Sprites/magic_dart.magic_dart"}
	};
}

USpriteManager::USpriteManager() {
	// pair is TPair
	const auto& pathMap = SPRITE_PATH;
	for (const auto& pair : pathMap) {
		FString tempPath = FString(pair.Value);
		FStringView paramPath = FStringView(tempPath);
		UPaperFlipbook* flipbook = LoadObject<UPaperFlipbook>(nullptr, paramPath);
		if (flipbook == nullptr) {
			LOGERROR("Failed to load sprite");
		}
		else {
			_sprites.Add(pair.Key, flipbook);
		}
	}
}

bool USpriteManager::getSprite(ESprite sprite, UPaperFlipbook*& ret) const {
	if (_sprites.Contains(sprite)) {
		ret = _sprites[sprite];
		return true;
	}
	else {
		LOGERROR("Failed to get sprite");
		return false;
	}
}