extends Resource

class_name ActorPreset
@export var text : String = "Undef Actor"
@export var MAXHP : int = -1
@export var HP : int = -1
@export var PHYSATK : float = -1
@export var PHYSDEF : float = -1
@export var MAGATK : float = -1
@export var MAGDEF : float = -1
@export var dead : bool = false
@export var actions : Array[Resource]

func setStat(stat : String, val) :
	if (stat == "MAXHP") : MAXHP = val
	elif (stat == "HP") : HP = val
	elif (stat == "PHYSATK") : PHYSATK = val
	elif (stat == "PHYSDEF") : PHYSDEF = val
	elif (stat == "MAGATK") : MAGATK = val
	elif (stat == "MAGDEF") : MAGDEF = val
	else :
		return
