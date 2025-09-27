extends Resource

class_name CharacterClass
@export var text : String = "Undef Class"
@export var baseDUR = -1
@export var scalingDUR : float = -1
@export var baseSTR = -1
@export var scalingSTR : float = -1
@export var baseSKI = -1
@export var scalingSKI : float = -1
@export var baseINT = -1
@export var scalingINT : float= -1

func getBaseAttribute(type : Definitions.attributeEnum) :
	if (type == Definitions.attributeEnum.DUR) :
		return baseDUR
	elif (type == Definitions.attributeEnum.STR) :
		return baseSTR
	elif (type == Definitions.attributeEnum.SKI) :
		return baseSKI
	elif (type == Definitions.attributeEnum.INT) :
		return baseINT
func getAttributeScaling(type : Definitions.attributeEnum) :
	if (type == Definitions.attributeEnum.DUR) :
		return scalingDUR
	elif (type == Definitions.attributeEnum.STR) :
		return scalingSTR
	elif (type == Definitions.attributeEnum.SKI) :
		return scalingSKI
	elif (type == Definitions.attributeEnum.INT) :
		return scalingINT
