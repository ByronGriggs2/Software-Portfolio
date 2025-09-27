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
	$StatAmounts/DUR.text = str(currentStats.baseDUR)
	$StatAmounts/STR.text = str(currentStats.baseSTR)
	$StatAmounts/SKI.text = str(currentStats.baseSKI)
	$StatAmounts/INT.text = str(currentStats.baseINT)
	$StatScaling/DUR.text = "x"+str(currentStats.scalingDUR)
	$StatScaling/STR.text = "x"+str(currentStats.scalingSTR)
	$StatScaling/SKI.text = "x"+str(currentStats.scalingSKI)
	$StatScaling/INT.text = "x"+str(currentStats.scalingINT)
	
