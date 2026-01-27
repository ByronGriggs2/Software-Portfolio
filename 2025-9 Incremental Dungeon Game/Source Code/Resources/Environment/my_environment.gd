extends Resource

class_name MyEnvironment
@export var title : String = ""
@export_multiline var introText : String = ""
@export var permittedFactions : Array[EnemyGroups.factionEnum] = []
@export var firePermitted : bool = false
@export var icePermitted : bool = false
@export var earthPermitted : bool = false
@export var waterPermitted : bool = false

func getName() :
	return title
##Assumes this is never duplicated
func getFileName() -> String :
	var var1 = resource_path
	var var2 = var1.get_file()
	var var3 = var2.get_basename()
	return var3
	#return resource_path.get_file().get_basename()
