extends Resource

class_name ActorPreset
@export var text : String = "Undef Actor"
@export var MAXHP : float = -1
@export var HP : float = -1
@export var PHYSATK : float = -1
@export var PHYSDEF : float = -1
@export var MAGATK : float = -1
@export var MAGDEF : float = -1
@export var dead : bool = false
@export var actions : Array[Resource]

func setStat(type : Definitions.baseStatEnum, val) :
	if (type == Definitions.baseStatEnum.MAXHP) : MAXHP = val
	elif (type == Definitions.baseStatEnum.HP) : HP = val
	elif (type == Definitions.baseStatEnum.PHYSATK) : PHYSATK = val
	elif (type == Definitions.baseStatEnum.PHYSDEF) : PHYSDEF = val
	elif (type == Definitions.baseStatEnum.MAGATK) : MAGATK = val
	elif (type == Definitions.baseStatEnum.MAGDEF) : MAGDEF = val
	else :
		return

func getStat(type : Definitions.baseStatEnum) -> float :
	if (type == Definitions.baseStatEnum.MAXHP) : return MAXHP
	elif (type == Definitions.baseStatEnum.HP) : return HP
	elif (type == Definitions.baseStatEnum.PHYSATK) : return PHYSATK
	elif (type == Definitions.baseStatEnum.PHYSDEF) : return PHYSDEF
	elif (type == Definitions.baseStatEnum.MAGATK) : return MAGATK
	elif (type == Definitions.baseStatEnum.MAGDEF) : return MAGDEF
	else :
		return -1
