extends Panel

var currentStats : CharacterClass
@export var fighterStats : CharacterClass = null
@export var mageStats : CharacterClass = null
@export var rogueStats : CharacterClass = null

func setStats(index : int) :
	if (index == 0) :
		currentStats = fighterStats
	elif (index == 1) :
		currentStats = mageStats
	elif (index == 2) :
		currentStats = rogueStats
	for key in Definitions.attributeDictionary.keys() :
		$VBoxContainer/ColumnContainer/StatAmounts.get_child(1+key).text = str(currentStats.getBaseAttribute(key))
		$VBoxContainer/ColumnContainer/StatScaling.get_child(1+key).text = str(currentStats.getAttributeScaling(key))
	
