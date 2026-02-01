extends Resource

class_name Encounter
@export var enemies : Array[ActorPreset]
@export var rewards : Array[Equipment]
@export var introTitle = ""
@export_multiline var introText = ""
@export var victoryTitle = "After the Fight"
@export_multiline var victoryText = ""

func getRewards(magicFind) -> Array[Equipment] :
	var retVal = rewards.duplicate()
	for enemy in enemies :
		var drops = enemy.getDrops(magicFind)
		for drop in drops :
			EnemyDatabase.setItemCollected(enemy.getResourceName(), drop.getName())
		if (!drops.is_empty()) :
			retVal.append_array(drops)
	return retVal

func getSaveDictionary() -> Dictionary :
	var retVal = {}
	retVal["enemies"] = []
	for enemy in enemies :
		retVal["enemies"].append(enemy.getSaveDictionary())
	retVal["rewards"] = []
	retVal["introTitle"] = introTitle
	retVal["introText"] = introText
	retVal["victoryTitle"] = victoryTitle
	retVal["victoryText"] = victoryText
	return retVal

## Do not make this virtual
static func createFromSaveDictionary(val : Dictionary) -> Encounter :
	var retVal = Encounter.new()
	var tempEnemies = val["enemies"]
	for enemy in tempEnemies :
		retVal.enemies.append(ActorPreset.createFromSaveDictionary(enemy))
	#retVal.rewards = []
	retVal.introTitle = val["introTitle"]
	retVal.introText = val["introText"]
	retVal.victoryTitle = val["victoryTitle"]
	retVal.victoryText = val["victoryText"]
	return retVal
