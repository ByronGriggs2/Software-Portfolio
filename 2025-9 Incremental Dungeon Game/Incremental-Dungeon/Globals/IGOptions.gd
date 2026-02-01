extends Node

enum optionType {checkBox,dropdown}
enum options {tutorialsEnabled, globalEncyclopedia}#,inventoryBehaviour}
const optionNameDictionary = {
	options.tutorialsEnabled : "Tutorials enabled",
	options.globalEncyclopedia : "Global encyclopedia"
	#options.inventoryBehaviour : "Inventory Behaviour"
}
const optionTypeDictionary = {
	options.tutorialsEnabled : optionType.checkBox,
	options.globalEncyclopedia : optionType.checkBox
	#options.inventoryBehaviour : optionType.dropdown
}
const optionDefaultDictionary = {
	options.tutorialsEnabled : true,
	options.globalEncyclopedia : false
}
const dropdownDictionary = {
	#options.inventoryBehaviour : ["Wait", "Discard"]
}


var optionDict : Dictionary = {}

###########################################
## Internal
func defaultDefined(key) -> bool :
	return optionDefaultDictionary.get(key) != null
func getDefault(key) :
	return optionDefaultDictionary[key]
func getDefaultOptionDict() -> Dictionary :
	var tempDict : Dictionary = {}
	for key in optionNameDictionary.keys() :
		if (defaultDefined(key)) :
			tempDict[key] = getDefault(key)
		elif (optionTypeDictionary[key] == optionType.checkBox) :
			tempDict[key] = false
		elif (optionTypeDictionary[key] == optionType.dropdown) :
			tempDict[key] = 0
		else :
			pass
	var emptyDict : Dictionary = {}
	tempDict["individualTutorialDisable"] = emptyDict.duplicate()
	#tempDict["individualEquipmentTake"] = emptyDict.duplicate()
	var emptyStrArray : Array = []
	tempDict["encounteredItems"] = emptyStrArray
	tempDict["filter"] = {
		"type" : "whitelist",
		"element" : [true,true,true,true,true],
		"quality" : [true,true,true,true,true,true],
		"equipmentType" : [true,true,true],
		"weaponType" : [true,true],
		"directDowngrade" : [true,false,false,false,false]
		}
	tempDict["hyperMode"] = false
	return tempDict
#########################################
## Getters
func getIGOptionsCopy() -> Dictionary :
	return optionDict.duplicate()
#########################################
## Setters
func saveAndUpdateIGOptions(newDict) :
	optionDict = newDict
	var toUpdate = get_tree().get_nodes_in_group("CaresAboutOptions")
	for node : Node in toUpdate :
		node.updateFromOptions(optionDict)
func saveIGOptionsNoUpdate(newDict) :
	optionDict = newDict
func addToTutorialListNoSignal(val : Encyclopedia.tutorialName) :
	optionDict["individualTutorialDisable"][val] = false
#########################################
## Saving
#const myLoadDependencyName = Definitions.loadDependencyName.IGOptions_name
#const myLoadDependencies : Array = []
#func afterDependencyLoaded(dependency : Definitions.loadDependencyName) :
	#pass
	
func updateGlobalSettings_items() :
	var current = SaveManager.getGlobalSettings()
	for itemName in optionDict["encounteredItems"] : 
		if (current["globalEncyclopedia"]["items"].find(itemName) == -1) :
			current["globalEncyclopedia"]["items"].append(itemName)
	SaveManager.saveGlobalSettings(current)
	
const forcedInclude = [
	"scraps",
	"magic_stick_int",
	"magic_stick_str",
	"shiv"
]

func checkEquipmentEncyclopedia() :
	if (!Definitions.steamEnabled) :
		return
	var globalItemList = SaveManager.getGlobalSettings()["globalEncyclopedia"]["items"]
	var unlock : bool = true
	var items = EquipmentDatabase.getAllEquipment()
	for item : Equipment in items :
		if (Helpers.isDLC(item)) :
			continue
		if (!item.equipmentGroups.isEligible && !item.equipmentGroups.isSignature && forcedInclude.find(item.getItemName()) == -1) :
			continue
		if (globalItemList.find(item.getItemName()) == -1) :
			unlock = false
			break
	if (unlock) :
		Helpers.unlockAchievement(Definitions.achievementEnum.all_equipment)

func getSaveDictionary() -> Dictionary :
	var tempDict : Dictionary = {}
	tempDict["optionDict"] = optionDict
	return tempDict
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	add_to_group("Saveable")
	myReady = true
	emit_signal("myReadySignal")
	
func beforeLoad(newGame : bool) :
	myReady = false
	if (newGame) :
		saveAndUpdateIGOptions(getDefaultOptionDict())
	myReady = true
	emit_signal("myReadySignal")
	if (newGame) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
		
func onLoad(loadDict : Dictionary) :
	myReady = false
	var myLoadDict = loadDict.duplicate(true)
	myLoadDict["optionDict"] = compensateForOldSaves(myLoadDict["optionDict"])
	saveAndUpdateIGOptions(myLoadDict["optionDict"])
	updateGlobalSettings_items()
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")
	
func compensateForOldSaves(myLoadDict) -> Dictionary :
	var temp = myLoadDict
	if (temp.get("filter") == null) :
		temp["filter"] = getDefaultOptionDict()["filter"].duplicate(true)
	if (temp["filter"].get("weaponType") == null) :
		temp["filter"]["weaponType"] = getDefaultOptionDict()["filter"]["weaponType"].duplicate(true)
	if (temp["filter"]["directDowngrade"].size() != getDefaultOptionDict()["filter"]["directDowngrade"].size()) :
		temp["filter"]["directDowngrade"] = getDefaultOptionDict()["filter"]["directDowngrade"].duplicate(true)
	for key in optionDefaultDictionary :
		if (temp.get(key) == null) :
			temp[key] = optionDefaultDictionary[key]
	return temp
func getHypermode() -> bool :
	if (optionDict.get("hyperMode") == null) :
		return false
	return optionDict["hyperMode"]
