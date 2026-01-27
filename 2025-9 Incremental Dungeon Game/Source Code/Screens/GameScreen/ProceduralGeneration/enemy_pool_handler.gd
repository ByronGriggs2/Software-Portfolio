extends Node

var environment : MyEnvironment = null
var enemyPool : Array = []
var miscPool : Array = []
var enemyCounts : Array[int] = [0,0,0]
var miscEnemyCounts : Array[int] = [0,0,0]
func reset(env : MyEnvironment, enemies : Array) :
	environment = env
	enemyPool = enemies
	filterEnemyPool_env()
	updateEnemyCounts()
	
func initialiseMiscPool(newMiscPool) :
	miscEnemyCounts = [0,0,0]
	miscPool = newMiscPool
	for enemy in miscPool :
		miscEnemyCounts[enemy.enemyGroups.enemyQuality] += 1

func updateEnemyCounts() :
	enemyCounts = [0,0,0]
	for enemy in enemyPool :
		enemyCounts[enemy.enemyGroups.enemyQuality] += 1
		
func getEnemyPool() :
	return enemyPool.duplicate()
	
func filterEnemyPool_env() :
	var index = 0
	while (index < enemyPool.size()) :
		var preEnemy = enemyPool[index]
		var enemy = preEnemy.enemyGroups
		var remove = (environment.permittedFactions.find(enemy.faction) == -1)
		if (remove) :
			var pos = enemyPool.find(preEnemy)
			enemyPool.remove_at(pos)
		else :
			index += 1

#func filterEnemyPool_env() :
	#filterEnemyPool_env_envWhitelist() 
	#filterEnemyPool_env_envBlacklist()
	#filterEnemyPool_env_enemyWhitelist()
#
#func filterEnemyPool_env_envWhitelist() :
	#for preEnemy in enemyPool :
		#var enemy = preEnemy.enemyGroups
		#var remove : bool = false
		#if (environment.isHot && !enemy.isHot) :
			#remove = true
		#elif (environment.isCold && !enemy.isCold) :
			#remove = true
		#if (remove) :
			#enemyPool.remove_at(enemyPool.find(preEnemy))
#func filterEnemyPool_env_envBlacklist() :
	#for preEnemy in enemyPool :
		#var enemy = preEnemy.enemyGroups
		#var remove : bool = false
		#if (environment.isWet && enemy.isGreenskin) :
			#remove = true
		#elif (environment.isDesert && enemy.isMerfolk) :
			#remove = true
		#elif (environment.isRainforest && enemy.isUndead) :
			#remove = true
		#elif (environment.isMartial && enemy.isMindless) :
			#remove = true
		#elif (environment.isHighTech && enemy.isMindless) :
			#remove = true
		#if (remove) :
			#enemyPool.remove_at(enemyPool.find(preEnemy))
#func filterEnemyPool_env_enemyWhitelist() :
	#for preEnemy in enemyPool :
		#var enemy = preEnemy.enemyGroups
		#var remove : bool = false
		#if (enemy.isElemental && !environment.isMagical) :
			#remove = true
		#if (enemy.isFae && !environment.isMagical) :
			#remove = true
		#if (remove) :
			#enemyPool.remove_at(enemyPool.find(preEnemy))
func getEnemyOfType(type : EnemyGroups.enemyQualityEnum) :
	var isMisc = miscEnemyCounts[type] != 0 && randi_range(0,7) == 0
	var currentCount = 0
	if (isMisc) :
		var roll = randi_range(0, miscEnemyCounts[type]-1)
		for enemy in miscPool : 
			if (enemy.enemyGroups.enemyQuality == type) :
				if (currentCount == roll) :
					return enemy
				else :
					currentCount += 1
	else :
		var roll = randi_range(0, enemyCounts[type]-1)
		for enemy in enemyPool :
			if (enemy.enemyGroups.enemyQuality == type) :
				if (currentCount == roll) :
					return enemy
				else :
					currentCount += 1
