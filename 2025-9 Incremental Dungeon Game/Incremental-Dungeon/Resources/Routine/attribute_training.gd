@tool
extends Resource

class_name AttributeTraining

@export var text : String = "sample text"
@export var hidden : bool = true
@export var scaling : Dictionary

func getText() :
	return text

func initDictionary() :
	if (Definitions == null) :
		return
	for key in Definitions.attributeDictionary.keys() :
		if (scaling.get(Definitions.attributeDictionary[key]) == null) :
			scaling[Definitions.attributeDictionary[key]] = 0 as float

func _init() :
	initDictionary()
	
func _get_property_list() -> Array :
	initDictionary()
	return []

func getScaling(type : Definitions.attributeEnum) :
	return scaling[Definitions.attributeDictionary[type]]

func getMultipliers() -> Array[float] :
	var retVal : Array[float] = []
	for key in Definitions.attributeDictionary.keys() :
		retVal.append(getScaling(key))
	return retVal

func getResourceName() :
	return resource_path.get_file().get_basename()
