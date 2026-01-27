@tool
extends Equipment

class_name Armor
@export var PHYSDEF : float = 0
@export var MAGDEF : float = 0
var type = Definitions.equipmentTypeEnum.armor

func getAdjustedCopy(scalingFactor : float) -> Armor :
	var retVal = super(scalingFactor) as Armor
	if (Helpers.equipmentIsNew(self)) :
		retVal.calculateDefenses(scalingFactor, false)
	return retVal
	
func createSampleCopy() -> Armor :
	var retVal = super() as Armor
	if (Helpers.equipmentIsNew(self)) :
		retVal.calculateDefenses(1, true)
	return retVal

const ratioMin = [0.500, 0.741, 1.350]
const ratioMax = [0.741, 1.350, 2.000]
## +4 armor (5x buffs) should be 174.9. That means +0 armor should be 20.4947. divided by 1.08 to compensate for quality
const referenceDefense = 20.4947/1.08/1.15
func calculateDefenses(scalingFactor, useAverage) :
	var myQuality = equipmentGroups.quality
	var myType = equipmentGroups.armorClass
	var qualityFactor = pow(1.08, myQuality as int)
	var ratio
	if (useAverage) :
		ratio = (ratioMin[myType] + ratioMax[myType])/2.0
	else :
		ratio = randf_range(ratioMin[myType], ratioMax[myType])
	MAGDEF = (2*referenceDefense)/(1+ratio)
	PHYSDEF = 2*referenceDefense-MAGDEF
	MAGDEF *= scalingFactor * qualityFactor
	PHYSDEF *= scalingFactor * qualityFactor
	if (!equipmentGroups.isElemental()) :
		PHYSDEF *= 1.15
		MAGDEF *= 1.15
	
func getSaveDictionary() -> Dictionary :
	var retVal = super()
	if (!Helpers.equipmentIsNew(self)) :
		return retVal
	retVal["PHYSDEF"] = PHYSDEF
	retVal["MAGDEF"] = MAGDEF
	return retVal
	
func createFromSaveDictionary(loadDict) -> Armor :
	var retVal = super(loadDict) as Armor
	if (!Helpers.equipmentIsNew(retVal)) :
		return retVal
	retVal.PHYSDEF = loadDict["PHYSDEF"]
	retVal.MAGDEF = loadDict["MAGDEF"]
	return retVal
	
func getType() :
	return type
