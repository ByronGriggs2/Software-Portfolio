#pragma once

/*
!!For testing!!
*/
#if UE_BUILD_DEVELOPMENT
// All periodic attacks start off cooldown and trigger once per second, ignoring status effects and attributes
#define QUICK_ATTACK true
#define DISABLE_EXPERIENCE false
#define COMBATANT_HEALTH_MULTIPLIER 1
#define SHOW_ALL_NUMBERS false
#define FORCE_START_OFF_COOLDOWN true












#else
#define QUICK_ATTACK false
#define DISABLE_EXPERIENCE false
#define ENEMY_HEALTH_MULTIPLIER 1
#define SHOW_ALL_NUMBERS false
#define COMBATANT_HEALTH_MULTIPLIER 1
#define FORCE_START_OFF_COOLDOWN true
#endif