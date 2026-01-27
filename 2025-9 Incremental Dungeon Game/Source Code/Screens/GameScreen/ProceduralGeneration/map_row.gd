extends Resource

class_name MapRow
@export var centralEncounter : Encounter
@export var leftEncounters : Array[Encounter]
@export var rightEncounters : Array[Encounter]

func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["central"] = centralEncounter.getSaveDictionary()
	retVal["left"] = []
	for encounter in leftEncounters :
		retVal["left"].append(encounter.getSaveDictionary())
	retVal["right"] = []
	for encounter in rightEncounters : 
		retVal["right"].append(encounter.getSaveDictionary())
	return retVal
	
## Do not make this virtual
static func createFromSaveDictionary(val : Dictionary) -> MapRow :
	var retVal = MapRow.new()
	retVal.centralEncounter = Encounter.createFromSaveDictionary(val["central"])
	var tempLeft = val["left"]
	for encounter in tempLeft :
		retVal.leftEncounters.append(Encounter.createFromSaveDictionary(encounter))
	var tempRight = val["right"]
	for encounter in tempRight :
		retVal.rightEncounters.append(Encounter.createFromSaveDictionary(encounter))
	return retVal
