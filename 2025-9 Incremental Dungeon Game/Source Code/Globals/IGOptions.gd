extends Node

enum optionType {checkBox}
enum options {tutorialsEnabled}
const optionNameDictionary = {
	options.tutorialsEnabled : "Tutorials enabled"
}
const optionTypeDictionary = {
	options.tutorialsEnabled : optionType.checkBox
}
const optionDefaultDictionary = {
	options.tutorialsEnabled : true
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
		else :
			pass
	var emptyDict : Dictionary = {}
	tempDict["individualTutorialDisable"] = emptyDict
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
#########################################
## Saving
#const myLoadDependencyName = Definitions.loadDependencyName.IGOptions_name
#const myLoadDependencies : Array = []
#func afterDependencyLoaded(dependency : Definitions.loadDependencyName) :
	#pass

func getSaveDictionary() -> Dictionary :
	var tempDict : Dictionary = {}
	tempDict["optionDict"] = optionDict
	return tempDict
var myReady : bool = false

func _ready() :
	add_to_group("Saveable")
	myReady = true
	
func beforeLoad(newGame : bool) :
	if (newGame) :
		saveAndUpdateIGOptions(getDefaultOptionDict())
		
func onLoad(loadDict : Dictionary) :
	saveAndUpdateIGOptions(loadDict["optionDict"])
