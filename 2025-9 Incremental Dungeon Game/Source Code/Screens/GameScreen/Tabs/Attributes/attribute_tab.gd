extends Panel

var unlockState : int = -1

func _ready() :
	updateUnlockButton()
	
func updateUnlockButton() :
	if (!has_node("UnlockButton")) :
		return
	$UnlockButton.text = "Unlock " + Definitions.attributeDictionary[unlockState+1]

func unlockAttribute(type : Definitions.attributeEnum) :
	$AttributeBarContainer.get_child(type).unlock()
	
func getAttributeLevel(type : Definitions.attributeEnum) -> int : 
	return $AttributeBarContainer.get_child(type).getLevel()

func setAttributeLevel(type : Definitions.attributeEnum, val : int ) -> void :
	$AttributeBarContainer.get_child(type).setLevel(val)
	
func getProgress(type : Definitions.attributeEnum) -> float :
	return $AttributeBarContainer.get_child(type).getValue()

func setProgress(type : Definitions.attributeEnum, val : float) -> void :
	$AttributeBarContainer.get_child(type).setValue(val)

func _on_unlock_button_pressed() -> void:
	if (unlockState == Definitions.attributeEnum.size()-1) : 
		return
	unlockState = unlockState + 1 as Definitions.attributeEnum
	unlockAttribute(unlockState)
	if (unlockState == Definitions.attributeEnum.size()-1) :
		$UnlockButton.queue_free()
	else :
		updateUnlockButton()
		
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	for key in Definitions.attributeDictionary.keys() :
		var attributeLevelKey : String = str(key) + "attributeLevel"
		var levelProgressKey : String = str(key) + "attributeLevelProgress"
		tempDict[attributeLevelKey] = getAttributeLevel(key)
		tempDict[levelProgressKey] = getProgress(key)
	return tempDict
		
func setFromSaveDictionary(loadDict) -> void :
	for key in Definitions.attributeDictionary.keys() :
		var attributeLevelKey : String = str(key) + "attributeLevel"
		var levelProgressKey : String = str(key) + "attributeLevelProgress"
		setAttributeLevel(key, loadDict[attributeLevelKey])
		setProgress(key, loadDict[levelProgressKey])
