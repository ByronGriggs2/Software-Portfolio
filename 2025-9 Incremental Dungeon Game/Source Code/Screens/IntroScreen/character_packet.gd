extends Resource

class_name CharacterPacket
var portraitDictionary : Dictionary = {}
var portraitRef : Node = null
var myClass : CharacterClass = null
var myName : String = ""

func getPortraitDictionary() -> Dictionary :
	if (portraitDictionary == {} && portraitRef != null) :
		portraitDictionary = portraitRef.getSaveDictionary()
	return portraitDictionary.duplicate(true)
func setPortrait(val : Node) : 
	portraitRef = val
	portraitDictionary = portraitRef.getSaveDictionary()
func setPortraitExtraSafe(val : Node) :
	portraitDictionary = val.getSaveDictionary()
func setClass(val : CharacterClass) :
	myClass = val
func getClass() -> CharacterClass :
	if (myClass == null) :
		return CharacterClass.new()
	return myClass
func setName(val) :
	myName = val
func getName() -> String :
	return myName
	
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["portrait"] = portraitDictionary
	retVal["class"] = myClass.resource_path
	retVal["name"] = myName
	return retVal
	
static func createFromSaveDictionary(loadDict) -> CharacterPacket :
	var retVal = CharacterPacket.new()
	retVal.portraitDictionary = loadDict["portrait"]
	retVal.myClass = load(loadDict["class"])
	retVal.myName = loadDict["name"]
	return retVal
