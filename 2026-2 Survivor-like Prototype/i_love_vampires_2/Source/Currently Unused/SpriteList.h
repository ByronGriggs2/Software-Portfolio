#pragma once

#include <unordered_map>
#include "SpriteEnum.h"


const std::unordered_map<ESprite, const char*> SPRITE_PATHS = {
	{ESprite::testPlayer, "/Game/Sprites/athena.athena"},
	{ESprite::testEnemy, "/Game/Sprites/enemy.enemy"},
	{ESprite::testBullet, "/Game/Sprites/placeholder_bullet.placeholder_bullet"},
	{ESprite::testShard, "/Game/Sprites/shard.shard"}
};