extends Panel

var currentTraining : AttributeTraining = null

func _on_training_panel_training_changed(newVal) -> void:
	currentTraining = newVal
	$AttributeLevels.setMultipliers(currentTraining)
		
func getAttributeFinal(type : Definitions.attributeEnum) -> float :
	#placeholder
	return $AttributeLevels.getLevel(type)

func addPlayerReference(playerReference) :
	$AttributeMultipliers.addPlayerReference(playerReference)

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
	return tempDict
	
var myReady : bool = false
func _ready() :
	myReady = true
		
func beforeLoad(newSave : bool) :
	if (newSave) :
		currentTraining = null
		$TrainingPanel.setCurrentTraining(null)
		$AttributeLevels.setMultipliers(currentTraining)
	
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
	
func afterLoad() :
	pass
