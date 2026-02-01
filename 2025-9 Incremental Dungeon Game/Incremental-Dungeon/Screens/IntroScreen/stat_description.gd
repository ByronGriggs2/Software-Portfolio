extends PanelContainer

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
		var titleIndex = 3 + 3*key
		var baseIndex = titleIndex + 1
		var scalingIndex = titleIndex + 2
		$VBoxContainer/ColumnContainer.get_child(baseIndex).text = str(currentStats.getBaseAttribute(key))
		$VBoxContainer/ColumnContainer.get_child(scalingIndex).text = str(currentStats.getAttributeScaling(key))+"x"
	
