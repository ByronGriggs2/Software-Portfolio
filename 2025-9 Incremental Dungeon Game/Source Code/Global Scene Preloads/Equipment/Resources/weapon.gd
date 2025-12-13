@tool
extends Equipment

class_name Weapon
@export var scaling : Dictionary = {}
@export var attackBonus : float
@export var basicAttack : AttackAction
@export var isUnarmed : bool = false
var type = Definitions.equipmentTypeEnum.weapon

func reset() :
	super()
	resetScaling()
	
func resetNew() :
	super()
	if (scaling == null || scaling == {} || scaling.keys().size() != Definitions.attributeDictionary.keys().size()) :
		resetScaling()
	
func resetScaling() :
	scaling = {}
	for key in Definitions.attributeDictionary.keys() :
		scaling[Definitions.attributeDictionary[key]] = 0.0
		
func getScaling(key : int) :
	return scaling[Definitions.attributeDictionary[key as Definitions.attributeEnum]]
	
func getScalingArray() -> Array[float] :
	var retVal : Array[float] = []
	for key in scaling.keys() :
		retVal.append(scaling[key])
	return retVal
		
##Where E = 1 and S = 6, letter = log1.5(x/0.7)+2
static func scalingToLetter(scalingVal : float) -> String :
	if (scalingVal < -0.001) :
		return "N"
	elif (is_zero_approx(scalingVal)) :
		return "--"
	elif (scalingVal <= 0.25) :
		return "E"
	elif (scalingVal <= 0.5) :
		return "D"
	elif (scalingVal <= 0.75) :
		return "C"
	elif (scalingVal <= 1) :
		return "B"
	elif (scalingVal <= 1.25) :
		return "A"
	else :
		return "S"
