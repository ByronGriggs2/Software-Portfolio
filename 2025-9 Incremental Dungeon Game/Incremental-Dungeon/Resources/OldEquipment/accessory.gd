@tool
extends Equipment

class_name Accessory
var type = Definitions.equipmentTypeEnum.accessory
@export var boostedAttr1 : Definitions.attributeEnum = -1
@export var boostedAttr2 : Definitions.attributeEnum = -1

const referenceValue = 4.094723*(31.8/60.0)
func getAdjustedCopy(scalingFactor : float) -> Accessory :
	var retVal = super(scalingFactor) as Accessory
	if (!Helpers.equipmentIsNew(retVal) || scalingFactor == -1) :
		return retVal
	var qualityModifier = pow(1.08,equipmentGroups.quality as int)
	if (!retVal.equipmentGroups.isElemental()) :
		qualityModifier *= 1.15
	if (boostedAttr2 != -1 && boostedAttr1 != -1) :
		retVal.myPacket.attributeMods[Definitions.attributeDictionary[boostedAttr1]]["Prebonus"] = qualityModifier*scalingFactor*referenceValue/2.0
		retVal.myPacket.attributeMods[Definitions.attributeDictionary[boostedAttr2]]["Prebonus"] = qualityModifier*scalingFactor*referenceValue/2.0
	elif (boostedAttr1 != -1) :
		retVal.myPacket.attributeMods[Definitions.attributeDictionary[boostedAttr1]]["Prebonus"] = qualityModifier*scalingFactor*referenceValue
	return retVal

func getSaveDictionary() -> Dictionary :
	var retVal = super()
	if (!Helpers.equipmentIsNew(self)) :
		return retVal
	if (boostedAttr1 == -1) :
		retVal["attrVal1"] = -1
	else :
		retVal["attrVal1"] = myPacket.attributeMods[Definitions.attributeDictionary[boostedAttr1]]["Prebonus"]
	return retVal
	
func createFromSaveDictionary(loadDict) -> Accessory :
	var retVal = super(loadDict)
	if (!Helpers.equipmentIsNew(retVal)) :
		return retVal
	if (loadDict["attrVal1"] == -1) :
		return retVal
	retVal.myPacket.attributeMods[Definitions.attributeDictionary[retVal.boostedAttr1]]["Prebonus"] = loadDict["attrVal1"]
	if (retVal.boostedAttr2 != -1) :
		retVal.myPacket.attributeMods[Definitions.attributeDictionary[retVal.boostedAttr2]]["Prebonus"] = loadDict["attrVal1"]
	return retVal

func getType() :
	return type
