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
	
	$StatAmounts/SKI.text = str(currentStats.getBaseAttribute(Definitions.attributeEnum.DEX))
	$StatAmounts/DUR.text = str(currentStats.getBaseAttribute(Definitions.attributeEnum.DUR))
	$StatAmounts/INT.text = str(currentStats.getBaseAttribute(Definitions.attributeEnum.INT))
	$StatAmounts/STR.text = str(currentStats.getBaseAttribute(Definitions.attributeEnum.STR))

	$StatScaling/DUR.text = "x"+str(currentStats.getAttributeScaling(Definitions.attributeEnum.DEX))
	$StatScaling/STR.text = "x"+str(currentStats.getAttributeScaling(Definitions.attributeEnum.DUR))
	$StatScaling/SKI.text = "x"+str(currentStats.getAttributeScaling(Definitions.attributeEnum.INT))
	$StatScaling/INT.text = "x"+str(currentStats.getAttributeScaling(Definitions.attributeEnum.STR))
	
