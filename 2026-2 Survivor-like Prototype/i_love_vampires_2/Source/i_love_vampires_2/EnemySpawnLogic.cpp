#include "EnemySpawnLogic.h"
#include "MapTemplate.h"
#include "Combatant.h"
#include "Definitions.h"
#include "helpers.h"
#include <cmath>

bool Interval::tick(float newTime) {
	// Should have been removed
	if (_dirty) {
		LOGERROR("Interval should have been removed");
		return false;
	}
	if (_immediate && _triggerCount == 0) {
		_triggerCount++;
		_nextTriggerTime = _startTime + _period;
		return true;
	}
	bool ret = false;
	if (newTime > _nextTriggerTime) {
		ret = true;
		_triggerCount++;
		_nextTriggerTime = _startTime + _period * _triggerCount;
	}
	return ret;
}

EnemySpawnLogic::EnemySpawnLogic(UMapTemplate* map) : _map(map) {
	if (!IsValid(_map)) {
		LOGERROR("EnemySpawnLogic::EnemySpawnLogic - map is not valid");
	}
}

TArray<UCombatantTemplate*> EnemySpawnLogic::getEnemiesForTick(float totalTime) {
	pretick();
	mapEnemies_generic(totalTime);
	return posttick(totalTime);
}

void EnemySpawnLogic::mapEnemies_generic(float totalTime) {
	if (totalTime < 1.0 * _MINUTE) {
		const float freq = std::ceil((totalTime / _MINUTE) * 4.0);
		spawnUnits(EEnemyType::weakling, freq, totalTime);
	}
	else if (totalTime < 2 * _MINUTE) {
		spawnUnits(EEnemyType::weakling, 4, totalTime);
		spawnUnits(EEnemyType::grunt, 2, totalTime);
	}
	else if (totalTime < 3 * _MINUTE) {
		spawnUnits(EEnemyType::grunt, 4, totalTime);
	}
	else if (totalTime < 3.5 * _MINUTE) {
		spawnUnits(EEnemyType::swarmer, 6, totalTime);
	}
	else if (totalTime < 4 * _MINUTE) {
		spawnUnits(EEnemyType::swarmer, 4, totalTime);
		spawnUnits(EEnemyType::grunt, 3, totalTime);
	}
	else if (totalTime < 5 * _MINUTE) {
		spawnUnits(EEnemyType::grunt, 8, totalTime);
	}
	else if (totalTime < 5.5 * _MINUTE) {
		static bool spawned = false;
		if (!spawned) {
			spawnUnits(EEnemyType::fencer, 1, totalTime, true);
			spawned = true;
		}
		spawnUnits(EEnemyType::veteran, 2, totalTime);
	}
	else if (totalTime < 6 * _MINUTE) {
		spawnUnits(EEnemyType::grunt, 6, totalTime);
		spawnUnits(EEnemyType::suicide, 2, totalTime);
	}
	else if (totalTime < 6.5 * _MINUTE) {
		static bool spawned = false;
		if (!spawned) {
			spawnUnits(EEnemyType::barbarian, 1, totalTime, true);
			spawned = true;
		}
		spawnUnits(EEnemyType::suicide, 2, totalTime);
		spawnUnits(EEnemyType::swarmer, 4, totalTime);
	}
	else if (totalTime < 7 * _MINUTE) {
		spawnUnits(EEnemyType::suicide, 2, totalTime);
		spawnUnits(EEnemyType::swarmer, 6, totalTime);
		spawnUnits(EEnemyType::grunt, 2, totalTime);
	}
	else if (totalTime < 8 * _MINUTE) {
		static bool spawned = false;
		if (!spawned) {
			spawnUnits(EEnemyType::boss1, 1, totalTime, true);
			spawned = true;
		}
		// Note that these enemies will spawn at greatly increased frequency while the boss is alive
		spawnUnits(EEnemyType::swarmer, 4, totalTime);
		spawnUnits(EEnemyType::grunt, 4, totalTime);
		spawnUnits(EEnemyType::suicide, 2, totalTime);
	}
	else if (totalTime < 10 + 8*_MINUTE) {
		spawnUnits(EEnemyType::ranger, 2, totalTime);
		spawnUnits(EEnemyType::grunt, 4, totalTime);
	}
	else if (totalTime < 8.5 * _MINUTE) {
		spawnUnits(EEnemyType::ranger, 4, totalTime);
		spawnUnits(EEnemyType::veteran, 4, totalTime);
	}
	else if (totalTime < 9 * _MINUTE) {
		spawnUnits(EEnemyType::ranger, 4, totalTime);
		spawnUnits(EEnemyType::barbarian, 6, totalTime);
	}
	else if (totalTime < 9.5 * _MINUTE) {
		static bool spawned = false;
		if (!spawned) {
			spawnUnits(EEnemyType::knight, 1, totalTime, true);
			spawned = true;
		}
		spawnUnits(EEnemyType::veteran, 4, totalTime);
	}
	else if (totalTime < 10 * _MINUTE) {
		spawnUnits(EEnemyType::veteran, 6, totalTime);
		spawnUnits(EEnemyType::fencer, 1, totalTime);
		spawnUnits(EEnemyType::ranger, 2, totalTime);
	}
	else if (totalTime < 11 * _MINUTE) {
		spawnUnits(EEnemyType::fencer, 4, totalTime);
		spawnUnits(EEnemyType::ranger, 2, totalTime);
		spawnUnits(EEnemyType::barbarian, 2, totalTime);
	}
	else if (totalTime < 11.5 * _MINUTE) {
		static bool spawned = false;
		if (!spawned) {
			spawnUnits(EEnemyType::golem, 1, totalTime, true);
			spawned = true;
		}
		spawnUnits(EEnemyType::knight, 2, totalTime);
		spawnUnits(EEnemyType::ranger, 1, totalTime);
	}
	else if (totalTime < 12 * _MINUTE) {
		spawnUnits(EEnemyType::knight, 4, totalTime);
		spawnUnits(EEnemyType::ranger, 2, totalTime);
		spawnUnits(EEnemyType::fencer, 2, totalTime);
	}
	else if (totalTime < 12.5 * _MINUTE) {
		// time for an AOE dps check
		spawnUnits(EEnemyType::swarmer, 10, totalTime);
		spawnUnits(EEnemyType::barbarian, 2, totalTime);
		spawnUnits(EEnemyType::ranger_swarming, 8, totalTime);
	}
	else if (totalTime < 13 * _MINUTE) {
		spawnUnits(EEnemyType::swarmer, 6, totalTime);
		spawnUnits(EEnemyType::barbarian, 6, totalTime);
		spawnUnits(EEnemyType::ranger_swarming, 4, totalTime);
	}
	else if (totalTime < 14 * _MINUTE) {
		// bit of a single target dps check
		spawnUnits(EEnemyType::golem, 1, totalTime);
		spawnUnits(EEnemyType::knight, 3, totalTime);
		spawnUnits(EEnemyType::fencer, 4, totalTime);
		spawnUnits(EEnemyType::ranger, 3, totalTime);
	}
	else if (totalTime < 15 * _MINUTE){
		static bool spawned = false;
		if (!spawned) {
			spawnUnits(EEnemyType::boss2, 1, totalTime, true);
			spawned = true;
		}
		spawnUnits(EEnemyType::suicide, 2, totalTime);
		spawnUnits(EEnemyType::ranger, 2, totalTime);
		spawnUnits(EEnemyType::fencer, 1, totalTime);
		spawnUnits(EEnemyType::ranger_swarming, 2, totalTime);
		spawnUnits(EEnemyType::golem, 0.5, totalTime);
	}
	else if (totalTime < 16 * _MINUTE) {
		spawnUnits(EEnemyType::suicide, 2, totalTime);
		spawnUnits(EEnemyType::ranger, 2, totalTime);
		spawnUnits(EEnemyType::fencer, 2, totalTime);
		spawnUnits(EEnemyType::knight, 5, totalTime);
		spawnUnits(EEnemyType::veteran, 6, totalTime);
	}
	else if (totalTime < 16.5 * _MINUTE) {
		spawnUnits(EEnemyType::ranger, 8, totalTime);
		spawnUnits(EEnemyType::ranger_swarming, 12, totalTime);
		spawnUnits(EEnemyType::barbarian, 6, totalTime);
	}
	else if (totalTime < 17 * _MINUTE) {
		static bool spawned = false;
		if (!spawned) {
			spawnUnits(EEnemyType::minotaur, totalTime, true);
			spawned = true;
		}
		spawnUnits(EEnemyType::ranger, 6, totalTime);
		spawnUnits(EEnemyType::fencer, 4, totalTime);
	}
	else if (totalTime < 18 * _MINUTE) {
		spawnUnits(EEnemyType::veteran, 20, totalTime);
		spawnUnits(EEnemyType::knight, 8, totalTime);
		spawnUnits(EEnemyType::ranger, 3, totalTime);
	}
	else if (totalTime < 19 * _MINUTE) {
		spawnUnits(EEnemyType::ranger, 2, totalTime);
		spawnUnits(EEnemyType::fencer, 2, totalTime);
		spawnUnits(EEnemyType::golem, 1, totalTime);
		spawnUnits(EEnemyType::barbarian, 2, totalTime);
		spawnUnits(EEnemyType::ranger_swarming, 4, totalTime);
		spawnUnits(EEnemyType::swarmer, 6, totalTime);
		spawnUnits(EEnemyType::weakling, 10, totalTime);
		spawnUnits(EEnemyType::grunt, 6, totalTime);
		spawnUnits(EEnemyType::veteran, 4, totalTime);
		spawnUnits(EEnemyType::minotaur, 2, totalTime);
		spawnUnits(EEnemyType::suicide, 4, totalTime);
	}
	else if (totalTime < 19.25 * _MINUTE) {
		spawnUnits(EEnemyType::veteran, 25, totalTime);
		spawnUnits(EEnemyType::ranger, 4, totalTime);
		spawnUnits(EEnemyType::minotaur, 1, totalTime);
	}
	else if (totalTime < 19.75 * _MINUTE) {
		spawnUnits(EEnemyType::golem, 10, totalTime);
		spawnUnits(EEnemyType::knight, 4, totalTime);
	}
	else {
		static bool spawned = false;
		if (!spawned) {
			spawnUnits(EEnemyType::boss3, 1, totalTime, true);
			spawned = true;
		}
	}
}

void EnemySpawnLogic::pretick() {
	for (auto& pair : _intervals) {
		pair.Value.pretick();
	}
}

TArray<UCombatantTemplate*> EnemySpawnLogic::posttick(float newTime) {
	TArray<UCombatantTemplate*> ret;
	TArray<EEnemyType> dirtyKeys;
	// if the value is not dirty at this point, it indicates that it has been created or maintained this frame
	for (const auto& pair : _intervals) {
		if (pair.Value.isDirty())
			dirtyKeys.Add(pair.Key);
	}
	for (const auto& key : dirtyKeys) {
		_intervals.Remove(key);
	}
	for (auto& pair : _intervals) {
		if (pair.Value.tick(newTime)) {
			ret.Emplace(_map->getEnemyTemplate(pair.Key));
		}
	}
	return ret;
}

void EnemySpawnLogic::spawnUnits(EEnemyType type, float frequency, float totalTime, bool immediate) {
	if (!_intervals.Contains(type)) {
		_intervals.Emplace(type, Interval(totalTime, 1.0f / frequency, immediate));
	}
	else if (!(helpers::nearEq(_intervals[type].getPeriod(), 1.0f / frequency))) {
		_intervals.Remove(type);
		_intervals.Emplace(type, Interval(totalTime, 1.0f / frequency, immediate));
	}
	_intervals[type].removeDirty();
}