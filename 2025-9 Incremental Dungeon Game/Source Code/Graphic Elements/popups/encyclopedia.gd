extends "res://Graphic Elements/popups/my_popup_button.gd"

var optionDictCopy : Dictionary = {}
func _ready() :
	optionDictCopy = IGOptions.getIGOptionsCopy()

const keywordLoader = preload("res://Graphic Elements/Tooltips/encyclopedia_list.tscn")
func _on_keywords_pressed() -> void:
	var keywords = keywordLoader.instantiate()
	add_child(keywords)
	keywords.nestedPopupInit(self)

const beastiaryLoader = preload("res://Screens/GameScreen/beastiary.tscn")
func _on_beastiary_pressed() -> void:
	var beast = beastiaryLoader.instantiate()
	add_child(beast)
	beast.nestedPopupInit(self)

const equipmentLoader = preload("res://Graphic Elements/popups/combat_reward_behaviour.tscn")
func _on_equipment_pressed() -> void:
	var equips = equipmentLoader.instantiate()
	add_child(equips)
	equips.initialise()
	equips.connect("finished", _on_combat_reward_settings_finished)
	equips.nestedPopupInit(self)

func _on_combat_reward_settings_finished() :
	#optionDictCopy["individualEquipmentTake"] = newSettings
	#IGOptions.saveAndUpdateIGOptions(optionDictCopy)
	pass
	
const tutorialListLoader = preload("res://Screens/GameScreen/Tutorials/tutorial_list.tscn")
func _on_tutorials_pressed() -> void:
	var myTutorialList = tutorialListLoader.instantiate()
	add_child(myTutorialList)
	myTutorialList.initialise(optionDictCopy["individualTutorialDisable"].duplicate())
	myTutorialList.connect("tutorialListFinished", _on_tutorial_list_finished)
	myTutorialList.nestedPopupInit(self)
	
func _on_tutorial_list_finished(newDict : Dictionary) :
	optionDictCopy["individualTutorialDisable"] = newDict
	IGOptions.saveAndUpdateIGOptions(optionDictCopy)
	
