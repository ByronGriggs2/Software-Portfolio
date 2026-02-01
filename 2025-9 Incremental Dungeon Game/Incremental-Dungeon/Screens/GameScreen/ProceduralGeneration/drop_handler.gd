extends Node

const signatureEquipmentList = {
	"orc_warlord" : "adamantium_greataxe",
	"iron_golem" : "blunderbuss",
	"death" : "deaths_scythe",
	"titan" : "crackling_greataxe",
	"lich1" : "tome_death_2",
	"lich2" : "lich_crown",
	"amelia" : "amelia_wand",
	"lindwurm" : "coating_earth_2",
	"lava_snake" : "coating_fire_2",
	"ice_hydra" : "tome_ice_2",
	"arch_fiend" : "tome_fire_2",
	"ilsuiw" : "tome_water_2",
	"balrog" : "ring_authority",
	"tormentor" : "rampart_agony",
	"hellion" : "ring_malice",
	"hell_sentinel" : "rune_advanced_automation",
	"champion_of_poseidon" : "shield_5",
	"kraken" : "armor_cephalopod",
	"fire_dragon" : "dragonbone_plate",
	"water_dragon" : "dragonbone_plate",
	"ice_dragon" : "dragonbone_plate",
	"gold_dragon" : "dragonbone_plate",
	"shadow_dragon" : "dragonbone_plate",
	"swamp_dragon" : "dragonbone_plate",
	"iron_dragon" : "dragonbone_plate",
	"bone_dragon" : "dragonbone_plate",
	"apophis" : "dragonbone_plate",
	"polyphemus" : "guantlets_cyclops_strength"
}

var itemPool : Array
#
func reset(items) :
	itemPool = items
	updateQualityCounts()
	#
##func getDrops(enemy : ActorPreset) -> Array :
	#
##func getDrops(enemy) -> Array :
	##updateQualityCounts()
	##var dropCount = getDropCount()
	##var qualities = getDropQualities(dropCount)
	##var items = getItemsOfQualities(qualities)
	##if (items.size() == 0) :
		##print("problem")
	##return handleSignatureItem(enemy, items)
#
#func handleSignatureItem(enemy : ActorPreset, itemList : Array) -> Array :
	#var retVal = itemList
	#var enemyName = enemy.resource_path.get_file().get_basename()
	#if (signatureEquipmentList.get(enemyName) == null) :
		#return retVal
	#var roll = randi_range(0,3)
	#if (roll != 0) :
		#return retVal
	#var signatureItem : Equipment = EquipmentDatabase.getEquipment(signatureEquipmentList[enemyName])
	#if (retVal.is_empty()) :
		#retVal.append(signatureItem)
	#else :
		#retVal[0] = signatureItem
	#return retVal
	#
#const chances : Dictionary = {
	#1 : 0.222,
	#2 : 0.167,
	#3 : 0.148
#}
#func getDropChances(count : int) -> Array[float] :
	#var retVal : Array[float] = []
	#for index in range(0,count) :
		#retVal.append(chances[count])
	#return retVal

## Changing drops per normal/veteran/elite to drops per side/central/boss
const dropChance = [0.4,0.4,1.3]
func getDropCount(enemy : ActorPreset, magicFind : float) -> int:
	var averageVal
	if (enemy.enemyGroups.enemyQuality == EnemyGroups.enemyQualityEnum.normal || enemy.enemyGroups.enemyQuality == EnemyGroups.enemyQualityEnum.veteran) :
		var roll = randf_range(0,1)
		if (roll <= 0.65) :
			averageVal = 0
		elif (roll <= 0.95) :
			averageVal = 1
		else :
			averageVal = 2
	elif (enemy.enemyGroups.enemyQuality == EnemyGroups.enemyQualityEnum.elite) :
		var roll = randf_range(0,1)
		if (roll <= 0.267) :
			averageVal = 0
		elif (roll <= 0.617) :
			averageVal = 1
		elif (roll <= 0.817) :
			averageVal = 2
		else :
			averageVal = 3
	else :
		averageVal = 0
	averageVal *= magicFind
	var retVal : int = floor(averageVal)
	var roll = randf_range(0,1)
	if (averageVal-floor(averageVal)>=roll) :
		retVal += 1
	return retVal

func createDropsForEnemy(enemy : ActorPreset, scalingFactor : float, penaliseElemental : bool, magicFind : float) -> Array[Equipment] :
	var retVal : Array[Equipment] = []
	var dropCount = getDropCount(enemy, magicFind)
	if (dropCount == 0) :
		return retVal
	var signatureDropped : bool = false
	if (enemy.enemyGroups.enemyQuality == EnemyGroups.enemyQualityEnum.elite) :
		var roll = randf_range(0,100)
		if (roll < 15*magicFind) : 
			signatureDropped = true
	if (signatureDropped) :
		retVal.append(getSignature(enemy).getAdjustedCopy(scalingFactor))
		dropCount -= 1
	var qualities = getDropQualities(dropCount, magicFind)
	#for quality in qualities :
		#if (quality == EquipmentGroups.qualityEnum.legendary) :
			#print ("legendary rolled")
	for index in range(0,dropCount) :
		var type = rollType()
		while (qualities[index] == EquipmentGroups.qualityEnum.legendary && type == Definitions.equipmentTypeEnum.accessory) :
			type = rollType()
		var actualQuality = reduceToValidQuality(qualities[index], type)
		if (actualQuality == null) :
			continue
		var item
		if (penaliseElemental) :
			item = getItemOfQuality_penaliseElemental(actualQuality, type, false)
		else :
			item = getItemOfQuality(actualQuality, type)
		if (item == null) :
			continue
		retVal.append(item.getAdjustedCopy(scalingFactor))
	return retVal
	
func getSignature(enemy : ActorPreset) :
	if (enemy.getResourceName() == "lich") :
		if (randi_range(0,1)==0) :
			return EquipmentDatabase.getEquipment(signatureEquipmentList["lich1"])
		else :
			return EquipmentDatabase.getEquipment(signatureEquipmentList["lich2"])
	else :
		return EquipmentDatabase.getEquipment(signatureEquipmentList[enemy.getResourceName()])
	
const qualityThresholds : Array[float] = [1.5,5,15,40,100]
func getDropQualities(count : int, magicFind : float) -> Array[EquipmentGroups.qualityEnum] :
	var retVal : Array[EquipmentGroups.qualityEnum] = []
	for index in range(0,count) :
		var roll = randf_range(0,100)
		for qualityIndex in range(0,qualityThresholds.size()) :
			if (roll <= qualityThresholds[qualityIndex]*magicFind) :
				retVal.append((4-qualityIndex) as EquipmentGroups.qualityEnum)
				if (4-qualityIndex == EquipmentGroups.qualityEnum.legendary) :
					pass
					#print("legendary drop")
				break
	return retVal
	
#func getItemsOfQualities(qualities : Array[EquipmentGroups.qualityEnum], enemy : ActorPreset) -> Array[Equipment] :
	#var retVal : Array[Equipment] = []
	#var signatureSpawned : bool = false
	#for quality in qualities :
		#if (enemy.enemyGroups.enemyQuality == EnemyGroups.enemyQualityEnum.elite && quality == EquipmentGroups.qualityEnum.legendary) :
			#if ()
		#var actualQuality = reduceToValidQuality(quality)
		#if (actualQuality == null) :
			#continue
		#var newItem = getItemOfQuality(actualQuality)
		#if (newItem != null) :
			#retVal.append(newItem)
	#return retVal
	
func reduceToValidQuality(startingQuality, type) :
	var currentQuality = startingQuality
	while ((countDict["T"+str(type)].get(currentQuality) == null || countDict["T"+str(type)][currentQuality] == 0) && currentQuality != -1) :
		currentQuality -= 1
	if (currentQuality == -1) :
		return null
	return currentQuality
	
func rollType() :
	const baseWeights = [6,4,3]
	var actualWeights = [0,0,0]
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (key == Definitions.equipmentTypeEnum.currency) :
			continue
		if (countDict.get("T"+str(key)) != null && countDict["T"+str(key)].get("total") != null && countDict["T"+str(key)]["total"] > 0) :
			actualWeights[key] = baseWeights[key]
	var total = actualWeights[0] + actualWeights[1] + actualWeights[2]
	if (total == 0) :
		return null
	var chosenType
	var roll = randi_range(0,total-1)
	if (roll < actualWeights[0]) :
		return 0
	elif (roll < actualWeights[0] + actualWeights[1]) :
		return 1
	else :
		return 2
	
func getItemOfQuality(quality, type) :
	var realType = type
	#if (quality == EquipmentGroups.qualityEnum.legendary && type == Definitions.equipmentTypeEnum.accessory) :
		#while (realType == Definitions.equipmentTypeEnum.accessory) :
			#realType = rollType()
	var roll = randi_range(0,countDict["T"+str(realType)][quality]-1)
	var current = 0
	for item : Equipment in itemPool :
		if (item.getType() == realType && item.equipmentGroups.quality == quality) :
			if (roll == current) :
				return item
			current += 1
	return null
	
func getItemOfQuality_penaliseElemental(quality, type, isHalved : bool) :
	var poolContainsNonelemental : bool = false
	for item in itemPool :
		if (!item.equipmentGroups.isElemental()) :
			poolContainsNonelemental = true
			break
	if (!poolContainsNonelemental) :
		return getItemOfQuality(quality, type)
	
	var sample = getItemOfQuality(quality, type)
	if (sample == null) :
		return null
	var maxVal
	if (isHalved) :
		maxVal = 1
	else :
		maxVal = 3
	while (sample.equipmentGroups.isElemental() && randi_range(0,maxVal) != 0) :
		sample = getItemOfQuality(quality,type)
	return sample
	
#func getItemsOfQualities(qualities : Array[EquipmentGroups.qualityEnum]) :
	#var indices : Array[int] = []
	#for quality in qualities :
		##var actualQuality = reduceToValidQuality(quality, indices)
		#var newIndex = getIndexOfQuality(quality, indices)
		#if (newIndex != -1) :
			#indices.append(newIndex)
	#var retVal : Array[Equipment] = []
	#for index in indices :
		#retVal.append(itemPool[index])
	#return retVal
	#
#func reduceToValidQuality(startingQuality, excludedIndices) :
	#var currentQuality = startingQuality
	#while (true) :
		#var availableSpots = qualityCounts[currentQuality] - getQualityCountInList(currentQuality, excludedIndices)
		#if (availableSpots <= 0) :
			#currentQuality -= 1
		#if (availableSpots > 0 || currentQuality == -1) :
			#break
	#if (currentQuality == -1) :
		#return null
	#return currentQuality
	#
#func getIndexOfQuality(targetQuality, excludedIndices) :
	#var actualQuality = reduceToValidQuality(targetQuality, excludedIndices)
	#if (actualQuality == null) :
		#return -1
	#var adjustedCount = qualityCounts[actualQuality] - getQualityCountInList(actualQuality, excludedIndices)
	#var roll = randi_range(0,adjustedCount-1)
	#var currentCount = 0
	#for index in range(0, itemPool.size()) :
		#if (excludedIndices.find(index) != -1) :
			#continue
		#if (itemPool[index].equipmentGroups.quality == actualQuality) :
			#if (currentCount == roll) :
				#return index
			#else :
				#currentCount += 1
	##for index in range(0, itemPool.size()) :
		##if (excludedIndices.find(index) != -1) :
			##continue
		##if (itemPool[index].equipmentGroups.quality == targetQuality) :
			##if (currentCount == roll) :
				##return index
			##else :
				##currentCount += 1
	#return -1
			#
#func getQualityCountInList(targetQuality, list) :
	#var count = 0
	#for item in list :
		#if itemPool[item].equipmentGroups.quality == targetQuality :
			#count += 1
	#return count
		#
var countDict : Dictionary = {}
func updateQualityCounts() :
	countDict = {}
	for item in itemPool :
		var type = item.getType()
		if (countDict.get("T"+str(type)) == null) :
			countDict["T"+str(type)] = {"total" : 1}
		else :
			countDict["T"+str(type)]["total"] += 1
		var quality = item.equipmentGroups.quality
		#if (countDict.get("Q"+str(quality)) == null) :
			#countDict["Q"+str(quality)] = {"total" : 1}
		#else :
			#countDict["Q"+str(quality)]["total"] += 1
		if (countDict["T"+str(type)].get(quality) == null) :
			countDict["T"+str(type)][quality] = 1
		else :
			countDict["T"+str(type)][quality] += 1
		#if (countDict["Q"+str(quality)].get(type) == null) :
			#countDict["Q"+str(quality)][type] = 1
		#else :
			#countDict["Q"+str(quality)][type] += 1
