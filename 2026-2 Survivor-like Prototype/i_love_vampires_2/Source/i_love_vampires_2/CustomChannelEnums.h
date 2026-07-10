#pragma once

enum class collisionType {
	player,
	friendly,
	enemy,
	friendlyAttack,
	enemyAttack,
	powerup
};
// dependencies
// friendlyAttack, enemyAttack - Uppercut.h