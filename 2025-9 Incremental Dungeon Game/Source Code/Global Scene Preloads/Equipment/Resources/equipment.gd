
extends Resource

class_name Equipment
@export var title : String = "Sample title"
@export_multiline var description : String = "Sample description"
#You will have to manually update this if you ever add more stats
@export var attributeBonus : Dictionary = {
	Definitions.attributeDictionary[Definitions.attributeEnum.DEX] : 0 as float,
	Definitions.attributeDictionary[Definitions.attributeEnum.DUR] : 0 as float,
	Definitions.attributeDictionary[Definitions.attributeEnum.INT] : 0 as float,
	Definitions.attributeDictionary[Definitions.attributeEnum.STR] : 0 as float
}
@export var statBonus : Dictionary = {
	Definitions.baseStatDictionary[Definitions.baseStatEnum.MAXHP] : 0 as float,
	Definitions.baseStatDictionary[Definitions.baseStatEnum.AR] : 0 as float,
	Definitions.baseStatDictionary[Definitions.baseStatEnum.PHYSDEF] : 0 as float,
	Definitions.baseStatDictionary[Definitions.baseStatEnum.MAGDEF] : 0 as float
}
