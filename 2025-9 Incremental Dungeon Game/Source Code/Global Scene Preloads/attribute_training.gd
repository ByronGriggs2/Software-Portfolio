@tool
extends Resource

class_name AttributeTraining

@export var text : String = "sample text"
@export var scaling : Dictionary

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
