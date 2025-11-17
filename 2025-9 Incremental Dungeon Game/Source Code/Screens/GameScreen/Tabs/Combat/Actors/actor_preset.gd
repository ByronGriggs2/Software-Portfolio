extends Resource

class_name ActorPreset
@export var text : String = "Undef Actor"
@export var MAXHP : float = -1
var HP : float = 1
@export var AR : float = -1
@export var PHYSDEF : float = -1
@export var MAGATK : float = -1
@export var MAGDEF : float = -1
var dead : bool = false
@export var actions : Array[Action]
@export var drops : Array[Equipment]
@export var dropChances : Array[float]

func setStat(type : Definitions.baseStatEnum, val : float) :
	if (type == Definitions.baseStatEnum.MAXHP) : MAXHP = val
	elif (type == Definitions.baseStatEnum.AR) : AR = val
	elif (type == Definitions.baseStatEnum.PHYSDEF) : PHYSDEF = val
	elif (type == Definitions.baseStatEnum.MAGDEF) : MAGDEF = val
	else :
		return

func getStat(type : Definitions.baseStatEnum) -> float :
	if (type == Definitions.baseStatEnum.MAXHP) : return MAXHP
	elif (type == Definitions.baseStatEnum.AR) : return AR
	elif (type == Definitions.baseStatEnum.PHYSDEF) : return PHYSDEF
	elif (type == Definitions.baseStatEnum.MAGDEF) : return MAGDEF
	else :
		return -1
		
func getDrops() -> Array[Equipment] :
	var retVal : Array[Equipment] = []
	for index in range(0,drops.size()) :
		if (randf() < dropChances[index]) :
			retVal.append(drops[index])
	return retVal

func getHP() :
	return HP
func setHP(val) :
	HP = val
