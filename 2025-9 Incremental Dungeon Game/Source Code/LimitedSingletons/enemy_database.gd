extends Node

var resourceLoader = preload("res://ResourceReferences/ActorPresetReferences.gd")
var resources : Node = null
var killedDictionary : Dictionary = {}
var itemsObtainedDictionary : Dictionary = {}

func _init() :
	add_to_group("Saveable")
	resources = resourceLoader.new()
	add_child(resources)
	
func getEnemy(enemyName) -> Resource :
	return resources.getActorPreset(enemyName)
	
signal enemyDataChanged
func getEnemyKilled(enemyName : String) -> bool :
	return killedDictionary.get(enemyName) != null && killedDictionary[enemyName] != 0
func setEnemyKilled(enemyName : String, val : int) :
	killedDictionary[enemyName] = val
	emit_signal("enemyDataChanged", enemyName)
func incrementKills(enemyName : String) :
	killedDictionary[enemyName] += 1
	emit_signal("enemyDataChanged", enemyName)
func getAllEnemyDropsCollected(enemyName : String) -> bool :
	if (!getEnemyKilled(enemyName)) :
		return false
	for itemKey in itemsObtainedDictionary[enemyName].keys() :
		if (!itemsObtainedDictionary[enemyName][itemKey]) :
			return false
	return true
func setItemCollected(enemyName : String, item : String) :
	itemsObtainedDictionary[enemyName][item] = true
	emit_signal("enemyDataChanged", enemyName)
	
var myReady : bool = false
func _ready() :
	myReady = true
	
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["killed"]=killedDictionary
	retVal["items"]=itemsObtainedDictionary
	return retVal
	
func beforeLoad(newGame) :
	if (newGame) :
		for key in resources.ActorPresetDictionary.keys() :
			killedDictionary[key] = 0
			var emptyDict : Dictionary = {}
			itemsObtainedDictionary[key] = emptyDict
			for item in getEnemy(key).drops :
				itemsObtainedDictionary[key][item.getName()] = false

func onLoad(loadDict : Dictionary) :
	killedDictionary = loadDict["killed"]
	itemsObtainedDictionary = loadDict["items"]
	await get_tree().process_frame
	emit_signal("enemyDataChanged", "ALL")

func getEnemyList() :
	var retVal : Array = []
	retVal = killedDictionary.keys()
	retVal.sort_custom(func(a,b):return a<b)
	return retVal
	
func getItemObtained(enemy, item) :
	if (!getEnemyKilled(enemy)) :
		return false
	return itemsObtainedDictionary[enemy][item]
	
func getKillCount(enemy) :
	return killedDictionary[enemy]
