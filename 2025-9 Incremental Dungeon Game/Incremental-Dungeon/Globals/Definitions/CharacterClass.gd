@tool
extends Resource

class_name CharacterClass
@export var classEnum : Definitions.classEnum = Definitions.classEnum.fighter
@export var baseAttributes : Dictionary = {}
@export var attributeScaling : Dictionary = {}
#@export var baseActions : Array[Action]

func initDictionary() :
	if (Definitions == null) :
		return
	for key in Definitions.attributeDictionary.keys() :
		if (baseAttributes.get(Definitions.attributeDictionary[key]) == null) :
			baseAttributes[Definitions.attributeDictionary[key]] = 0 as float
		if (attributeScaling.get(Definitions.attributeDictionary[key]) == null) :
			attributeScaling[Definitions.attributeDictionary[key]] = 0 as float
		
func getBaseAttribute(type : Definitions.attributeEnum) :
	return baseAttributes[Definitions.attributeDictionary[type]]
	
func getAttributeScaling(type : Definitions.attributeEnum) :
	return attributeScaling[Definitions.attributeDictionary[type]]
	
func getText() -> String :
	return Definitions.classDictionary[classEnum]
