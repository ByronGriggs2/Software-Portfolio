extends Panel

var currentTraining : AttributeTraining = null

func _process(_delta) :
	$AttributeMultipliers.myUpdate(playerModsCache)
	$AttributeBonuses.myUpdate(playerModsCache)

func _on_training_panel_training_changed(newVal) -> void:
	currentTraining = newVal
	$AttributeLevels.setMultipliers(currentTraining)
		
func getAttributeLevels() -> Array[int] :
	var retVal : Array[int] = []
	for key in Definitions.attributeDictionary.keys() :
		retVal.append($AttributeLevels.getLevel(key))
	return retVal
	
signal tutorialRequested
var firstTimeSelected : bool = true
func _on_visibility_changed() -> void:
	if (visible && firstTimeSelected) :
		firstTimeSelected = false
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.trainingTab, Vector2(0,0))

func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	for key in Definitions.attributeDictionary.keys() :
		var attributeLevelKey : String = str(key) + "attributeLevel"
		var levelProgressKey : String = str(key) + "attributeLevelProgress"
		tempDict[attributeLevelKey] = $AttributeLevels.getLevel(key)
		tempDict[levelProgressKey] = $AttributeLevels.getProgress(key)
	if (currentTraining == null) :
		tempDict["currentTraining_scriptName"] = "null"
	else :
		tempDict["currentTraining_scriptName"] = currentTraining.get_path()
	tempDict["firstTimeSelected"] = firstTimeSelected
	return tempDict
	
var myReady : bool = false
func _ready() :
	myReady = true
		
func beforeLoad(newSave : bool) :
	if (newSave) :
		currentTraining = null
		$TrainingPanel.setCurrentTraining(null)
		$AttributeLevels.setMultipliers(currentTraining)
	for key in Definitions.attributeEnum.keys() :
		var attrNum = NumberClass.new()
		playerModsCache.append(attrNum)
	
func onLoad(loadDict) -> void :
	if (loadDict["currentTraining_scriptName"] == "null") :
		currentTraining = null
		$TrainingPanel.setCurrentTraining(null)
		$AttributeLevels.setMultipliers(currentTraining)
	else :
		currentTraining = load(loadDict["currentTraining_scriptName"])
		$TrainingPanel.setCurrentTraining(currentTraining)
		$AttributeLevels.setMultipliers(currentTraining)
	for key in Definitions.attributeDictionary.keys() :
		var attributeLevelKey : String = str(key) + "attributeLevel"
		var levelProgressKey : String = str(key) + "attributeLevelProgress"
		$AttributeLevels.setLevel(key, loadDict[attributeLevelKey])
		$AttributeLevels.setProgress(key, loadDict[levelProgressKey])
	firstTimeSelected = loadDict["firstTimeSelected"]

signal playerClassRequested
func _on_attribute_multipliers_player_class_requested(emitter) -> void:
	emit_signal("playerClassRequested", emitter)
	
var playerModsCache : Array[NumberClass] = []
func setPlayerMods(newMods : Array[NumberClass]) :
	playerModsCache = newMods
