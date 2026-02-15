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
	if (enemies.size() > 0):
		if (enemies.size() == 5 && enemies[2].getResourceName() == "apophis") :
			retVal["apophisFactor"] = enemies[2].myScalingFactor
		retVal["scalingFactor"] = enemies[0].myScalingFactor
		retVal["enemies"] = []
		for enemy in enemies :
			retVal["enemies"].append(enemy.getSaveDictionary())
	if (rewards.size() > 0) :
		retVal["rewards"] = []
	if (introTitle != "") :
		retVal["introTitle"] = introTitle
	if (introText != "") :
		retVal["introText"] = introText
	if (victoryTitle != "After the Fight") :
		retVal["victoryTitle"] = victoryTitle
	if (victoryText != "") :
		retVal["victoryText"] = victoryText
	return retVal

## Do not make this virtual
static func createFromSaveDictionary(val : Dictionary) -> Encounter :
	var scalingFactor = val.get("scalingFactor")
	var apophisFactor = val.get("apophisFactor")
	var retVal = Encounter.new()
	var tempEnemies = val.get("enemies")
	if (tempEnemies != null) :
		for index in range(0,tempEnemies.size()) :
			var enemy = tempEnemies[index]
			if (apophisFactor != null && index == 2) :
				retVal.enemies.append(ActorPreset.createFromSaveDictionary(enemy, apophisFactor))
			else :
				retVal.enemies.append(ActorPreset.createFromSaveDictionary(enemy, scalingFactor))
	#retVal.rewards = []
	if (val.get("introTitle") == null) :
		pass
	else :
		retVal.introTitle = val["introTitle"]
	if (val.get("introText") == null) :
		pass
	else :
		retVal.introText = val["introText"]
	if (val.get("victoryTitle") == null) :
		pass
	else :
		retVal.victoryTitle = val["victoryTitle"]
	if (val.get("victoryText") == null) :
		pass
	else : 
		retVal.victoryText = val["victoryText"]
	return retVal
