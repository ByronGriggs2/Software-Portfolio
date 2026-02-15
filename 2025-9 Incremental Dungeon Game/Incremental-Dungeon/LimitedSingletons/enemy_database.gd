extends Node

var killedDictionary : Dictionary = {}
var itemsObtainedDictionary : Dictionary = {}

func _init() :
	add_to_group("Saveable")
	connect("enemyDataChanged", _on_enemy_data_changed)
	
func getAllEnemies() -> Array :
	var actorList = MegaFile.getAllNewActor()
	actorList.append_array(MegaFile.getAllOldActor())
	return actorList
	
func getEnemy(enemyName) :
	var try = MegaFile.getNewActor(enemyName)
	if (try != null) :
		return try
	return MegaFile.getOldActor(enemyName)
	
signal enemyDataChanged
func getEnemyKilled(enemyName : String) -> bool :
	return killedDictionary.get(enemyName) != null && killedDictionary[enemyName] != 0
#func setEnemyKilled(enemyName : String, val : int) :
	#killedDictionary[enemyName] = val
	#emit_signal("enemyDataChanged", enemyName)
func incrementKills(enemyName : String) :
	killedDictionary[enemyName] += 1
	var current = SaveManager.getGlobalSettings()
	if (current["globalEncyclopedia"]["beastiary"].get(enemyName) == null) :
		current["globalEncyclopedia"]["beastiary"][enemyName] = 1
	else :
		current["globalEncyclopedia"]["beastiary"][enemyName] += 1
	SaveManager.saveGlobalSettings(current)
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
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
	
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["killed"]=killedDictionary
	retVal["items"]=itemsObtainedDictionary
	return retVal
	
var typicalAdasRatio : float = 0.45
func getAdasRatio() :
	return typicalAdasRatio
func beforeLoad(newGame) :
	myReady = false
	var sum = 0
	var attacks = MegaFile.getAllNewAction()
	for attack in attacks :
		sum += attack.getPower()/attack.getWarmup()
	typicalAdasRatio = sum/attacks.size()
	if (newGame) :
		for actor in MegaFile.getAllNewActor() :
			resetEntry(actor.resource_path.get_file().get_basename())
		for actor in MegaFile.getAllOldActor() :
			resetEntry(actor.resource_path.get_file().get_basename())
	myReady = true
	emit_signal("myReadySignal")
	if (newGame) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
	
func resetEntry(key) :
	killedDictionary[key] = 0
	var emptyDict : Dictionary = {}
	itemsObtainedDictionary[key] = emptyDict
	var enemy = getEnemy(key)
	for item in enemy.drops :
		itemsObtainedDictionary[key][item.getName()] = false

func onLoad(loadDict : Dictionary) :
	myReady = false
	killedDictionary = loadDict["killed"]
	var current = SaveManager.getGlobalSettings()
	var changed : bool = false
	for key in killedDictionary.keys() :
		if (current["globalEncyclopedia"]["beastiary"].get(key) == null || current["globalEncyclopedia"]["beastiary"][key] < killedDictionary[key]) :
			current["globalEncyclopedia"]["beastiary"][key] = killedDictionary[key]
			changed = true
	if (changed) :
		SaveManager.saveGlobalSettings(current)
	itemsObtainedDictionary = loadDict["items"]
	await get_tree().process_frame
	emit_signal("enemyDataChanged", "ALL")
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")

const forcedInclude = [
	"goblin",
	"hobgoblin",
	"orc",
	"rat",
	"zombie",
	"athena",
	"apophis"
]

#func getEnemyList() :
	#var retVal : Array = []
	#retVal = killedDictionary.keys().duplicate()
	#
	#retVal.sort_custom(func(a,b):return )
	#return retVal
	
func getItemObtained(enemy, item) :
	if (!getEnemyKilled(enemy)) :
		return false
	return itemsObtainedDictionary[enemy][item]
	
func getKillCount(enemy) :
	return killedDictionary[enemy]
	
var souls : int = 0
func getSoulCount() :
	return souls

func _on_enemy_data_changed(_var) :
	souls = 0
	var unlock : bool = true
	for enemy in killedDictionary.keys() :
		if (killedDictionary[enemy] > 0) :
			souls += 1
		elif (enemy != "athena" && (forcedInclude.find(enemy) != -1 || getEnemy(enemy).enemyGroups.isEligible)) :
			unlock = false
	if (unlock && Definitions.steamEnabled) :
		SteamWrapper.unlockAchievement(Definitions.achievementEnum.all_monsters)
