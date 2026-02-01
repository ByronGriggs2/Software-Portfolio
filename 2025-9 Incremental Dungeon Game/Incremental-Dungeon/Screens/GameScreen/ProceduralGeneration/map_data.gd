extends Resource

class_name MapData
@export var rows : Array[MapRow] = []
@export var bossEncounter : Encounter
@export var shopName : String = ""
@export var environmentName : String

## Do not make this virtual
static func createFromSaveDictionary(val : Dictionary) :
	if (val.is_empty()) :
		return null
	var retVal = MapData.new()
	var tempRows = val["rows"]
	for row in tempRows :
		retVal.rows.append(MapRow.createFromSaveDictionary(row))
	retVal.bossEncounter = Encounter.createFromSaveDictionary(val["boss"])
	retVal.environmentName = val["environmentName"]
	retVal.shopName = val["shopName"]
	return retVal
	
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["rows"] = []
	for row in rows :
		retVal["rows"].append(row.getSaveDictionary())
	retVal["boss"] = bossEncounter.getSaveDictionary()
	retVal["environmentName"] = environmentName
	retVal["shopName"] = shopName
	return retVal
