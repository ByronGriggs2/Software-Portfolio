extends Node

var environment : MyEnvironment = null
var itemPool : Array = []

var workingPool : Array = []

func reset(env : MyEnvironment, items : Array) :
	environment = env
	itemPool = items
	filterItemPool_env()
	
func getItemPoolForEnemy(newEnemy : ActorPreset) :
	filterWorkingPool_enemy(newEnemy)
	return workingPool.duplicate()

func filterItemPool_env() :
	var index = 0
	while (index < itemPool.size()) :
		var remove : bool = false
		var preItem = itemPool[index]
		var item = preItem.equipmentGroups
		remove = remove || (!item.isEligible)
		## Elemental accessories can be dropped in nonelemental areas with a penalty to raritiy
		if (!environment.firePermitted && !environment.icePermitted && !environment.earthPermitted && !environment.waterPermitted) :
			pass
		else :
			remove = remove || (item.isFire && !environment.firePermitted)
			remove = remove || (item.isIce && !environment.icePermitted)
			remove = remove || (item.isEarth && !environment.earthPermitted)
			remove = remove || (item.isWater && !environment.waterPermitted)
		if (remove) :
			itemPool.remove_at(index)
		else :
			index += 1
			
func filterWorkingPool_enemy(preEnemy : ActorPreset) :
	workingPool = itemPool.duplicate()
	var enemy = preEnemy.enemyGroups
	## Dragon ignores all tags
	if (enemy.equipmentLevel == EnemyGroups.enemyTechEnum.dragon) :
		return
	var index = 0
	while (index < workingPool.size()) :
		var remove : bool = false
		var preItem = workingPool[index]
		var item : EquipmentGroups = workingPool[index].equipmentGroups
		##Technology Tags
		if (item.technology == EquipmentGroups.technologyEnum.perennial) :
			pass
		elif (item.technology == EquipmentGroups.technologyEnum.natural) :
			remove = remove || !(enemy.equipmentLevel == EnemyGroups.enemyTechEnum.none || enemy.equipmentLevel == EnemyGroups.enemyTechEnum.poor)
		elif (item.technology == EquipmentGroups.technologyEnum.crude) :
			remove = remove || !(enemy.equipmentLevel == EnemyGroups.enemyTechEnum.poor)
		elif (item.technology == EquipmentGroups.technologyEnum.advanced) :
			remove = remove || !(enemy.equipmentLevel == EnemyGroups.enemyTechEnum.well)
		elif (item.technology == EquipmentGroups.technologyEnum.superior) :
			remove = remove || !(enemy.equipmentLevel == EnemyGroups.enemyTechEnum.well || enemy.equipmentLevel == EnemyGroups.enemyTechEnum.elite)
		else :
			return
		## Offensive (Weapon) tags
		if (preItem is Weapon) :
			remove = remove || (item.weaponClass == EquipmentGroups.weaponClassEnum.melee && enemy.enemyRange == EnemyGroups.enemyRangeEnum.ranged)
			remove = remove || (item.weaponClass == EquipmentGroups.weaponClassEnum.ranged && enemy.enemyRange == EnemyGroups.enemyRangeEnum.melee)
		## Defensive (Armor) tags
		if (preItem is Armor) :
			if (item.armorClass == EquipmentGroups.armorClassEnum.light) :
				remove = remove || !(enemy.enemyArmor == EnemyGroups.enemyArmorEnum.magical || enemy.enemyArmor == EnemyGroups.enemyArmorEnum.resistant)
			elif (item.armorClass == EquipmentGroups.armorClassEnum.medium) :
				remove = remove || !(enemy.enemyArmor == EnemyGroups.enemyArmorEnum.resistant || enemy.enemyArmor == EnemyGroups.enemyArmorEnum.hardened)
			elif (item.armorClass == EquipmentGroups.armorClassEnum.heavy) :
				remove = remove || !(enemy.enemyArmor == EnemyGroups.enemyArmorEnum.hardened || enemy.enemyArmor == EnemyGroups.enemyArmorEnum.ironclad)
			else :
				return
		## To catch mistakes I make in the editor....
		if (item.isSignature) :
			remove = true
		if (remove) :
			workingPool.remove_at(index)
		else :
			index += 1
			
#func filterItemPool_env_itemWhitelists() :
	#for preItem in itemPool :
		#var item = preItem.equipmentGroups
		#var remove : bool = false
		#if (item.isFire && !environment.isHot) :
			#remove = true
		#elif (item.isWater && !environment.isWet) :
			#remove = true
		#elif (item.isIce && !environment.isCold) :
			#remove = true
		#elif (item.isEarth && !environment.isDesert && !environment.isRainforest) :
			#remove = true
		#elif (item.isMagical && !environment.isMagical) :
			#remove = true
		#if (remove) :
			#itemPool.remove_at(itemPool.find(preItem))
	
#func filterItemPool_enemy() :
	#filterItemPool_enemy_enemyWhitelists()
#
#func filterItemPool_enemy_enemyWhitelists() :
	#for preItem in workingPool :
		#var item = preItem.equipmentGroups
		#var remove : bool = false
		#if (preItem is Armor && enemy.droppedArmorClasses.find(item.armorClass) == -1) :
			#remove = true
		#elif (preItem is Weapon && enemy.droppedWeaponClasses.find(item.weaponClass) == -1) :
			#remove = true
		#elif (enemy.droppedTechnologyClasses.find(item.technology) == -1) :
			#remove = true
		#if (remove) :
			#workingPool.remove_at(workingPool.find(preItem))
