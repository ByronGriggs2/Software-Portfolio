extends Node

var environment : MyEnvironment = null
var itemPool : Array[Equipment] = []

var workingPool : Array[Equipment] = []
var enemy : EnemyGroups = null

func reset(env : MyEnvironment, items : Array[Equipment]) :
	environment = env
	itemPool = items
	filterItemPool_env()
	
func getItemPoolForEnemy(newEnemy : ActorPreset) :
	enemy = newEnemy.enemyGroups
	workingPool = itemPool
	filterItemPool_enemy()
	return workingPool.duplicate()
	
func filterItemPool_env() :
	filterItemPool_env_itemWhitelists()
	
func filterItemPool_env_itemWhitelists() :
	for preItem in itemPool :
		var item = preItem.equipmentGroups
		var remove : bool = false
		if (item.isFire && !environment.isHot) :
			remove = true
		elif (item.isWater && !environment.isWet) :
			remove = true
		elif (item.isIce && !environment.isCold) :
			remove = true
		elif (item.isEarth && !environment.isDesert && !environment.isRainforest) :
			remove = true
		elif (item.isMagical && !environment.isMagical) :
			remove = true
		if (remove) :
			itemPool.remove_at(itemPool.find(preItem))
	
func filterItemPool_enemy() :
	filterItemPool_enemy_enemyWhitelists()

func filterItemPool_enemy_enemyWhitelists() :
	for preItem in workingPool :
		var item = preItem.equipmentGroups
		var remove : bool = false
		if (preItem is Armor && enemy.droppedArmorClasses.find(item.armorClass) == -1) :
			remove = true
		elif (preItem is Weapon && enemy.droppedWeaponClasses.find(item.weaponClass) == -1) :
			remove = true
		elif (enemy.droppedTechnologyClasses.find(item.technology) == -1) :
			remove = true
		if (remove) :
			workingPool.remove_at(workingPool.find(preItem))
