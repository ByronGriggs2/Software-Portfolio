extends Control

var unlockState : Definitions.attributeEnum = Definitions.attributeEnum.NUL

func _ready() :
	updateUnlockButton()
	
func updateUnlockButton() :
	if (!has_node("UnlockButton")) :
		return
	$UnlockButton.text = "Unlock " + Definitions.attributeDictionary[unlockState+1]

func unlockAttribute(type : Definitions.attributeEnum) :
	var tempName = Definitions.attributeDictionary[type] + "Bar"
	$AttributeBarContainer.get_node(tempName).unlock()
	
func getAttributeLevel(type : Definitions.attributeEnum) -> int : 
	var tempName = Definitions.attributeDictionary[type] + "Bar"
	return $AttributeBarContainer.get_node(tempName).getLevel()

func _on_unlock_button_pressed() -> void:
	if (unlockState == Definitions.attributeEnum.INT) : 
		return
	unlockState = unlockState + 1 as Definitions.attributeEnum
	unlockAttribute(unlockState)
	if (unlockState == Definitions.attributeEnum.INT) :
		$UnlockButton.queue_free()
	else :
		updateUnlockButton()
