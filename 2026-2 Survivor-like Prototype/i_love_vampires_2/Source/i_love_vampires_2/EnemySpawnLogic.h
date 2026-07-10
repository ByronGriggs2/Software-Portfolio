#pragma once
#include "CoreMinimal.h"
#include "MapTemplate.h"

class UMapTemplate;
class UCombatantTemplate;

class Interval {
	int _triggerCount = 0;
	float _nextTriggerTime = 0;
	const float _period = 1;
	const float _startTime = 0;
	bool _dirty = true;
	const bool _immediate = false;

public:
	Interval() = delete;
	Interval(float startTime, float period, bool immediate = false) : _nextTriggerTime(startTime), _period(period), _startTime(startTime), _immediate(immediate) {}
	Interval(const Interval& other) : _triggerCount(other._triggerCount), _nextTriggerTime(other._nextTriggerTime), _period(other._period),  _startTime(other._startTime), _dirty(other._dirty), _immediate(other._immediate) {}
	void pretick() { _dirty = true; }
	bool tick(float newTime);
	bool isDirty() const { return _dirty; }
	float getPeriod() const { return _period; }
	void removeDirty() { _dirty = false; }
};

class EnemySpawnLogic {
	const static inline float _MINUTE = 60;
	

	TObjectPtr<UMapTemplate> _map;

	TMap<EEnemyType, Interval> _intervals;

	// Calling spawnUnits on tick X implies that the game is in a state (tied to a time interval) in which those enemies should be spawned at the specified frequency.
	// Not calling spawnUnits in a subsequent tick implies that that interval has ended, and calling it again later will restart the spawn logic.
	// The intended use is, every frame/tick, call spawnUnits for every enemy type that should be spawning in the time interval for which tick is called.
	// Note that this means if you want to have Y enemies from source A and Z enemies from source B, you will have to sum them rather than calling multiple times (which causes a LOGERROR)
	void spawnUnits(EEnemyType type, float frequency, float totalTime, bool immediate = false);

	//For simplicity just make one of these functions per map
	void mapEnemies_generic(float totalTime);

	void pretick();
	TArray<UCombatantTemplate*> posttick(float totalTime);

public:
	EnemySpawnLogic() = delete;
	// disk copy
	EnemySpawnLogic(UMapTemplate* map);
	EnemySpawnLogic(const EnemySpawnLogic& other) = delete;
	EnemySpawnLogic(EnemySpawnLogic&& other) = delete;
	EnemySpawnLogic& operator=(const EnemySpawnLogic& other) = delete;
	EnemySpawnLogic& operator=(EnemySpawnLogic&& other) = delete;
	TArray<UCombatantTemplate*> getEnemiesForTick(float totalTime);
};