extends Resource

class_name Encounter
@export var enemies : Array[ActorPreset]
@export var rewards : Array[Equipment]
@export var introTitle = ""
@export_multiline var introText = ""
@export var victoryTitle = "After the Fight"
@export_multiline var victoryText = ""

func getRewards() -> Array[Equipment] :
	var retVal = rewards.duplicate()
	for enemy in enemies :
		var drops = enemy.getDrops()
		if (!drops.is_empty()) :
			retVal.append_array(drops)
	return retVal
