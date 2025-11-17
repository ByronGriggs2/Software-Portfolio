extends "res://Graphic Elements/popups/my_popup.gd"

const myCheckBox = preload("res://Graphic Elements/Buttons/my_check_box.tscn")
var optionDictCopy : Dictionary = {}

func _ready() :
	optionDictCopy = IGOptions.getIGOptionsCopy()
	for key in IGOptions.optionNameDictionary.keys() :
		if (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.checkBox) :
			var newElement = myCheckBox.instantiate()
			getOptionsContainer().add_child(newElement)
			getOptionsContainer().move_child(newElement, 1+key)
			newElement.setText(IGOptions.optionNameDictionary[key])
			newElement.set_pressed_no_signal(optionDictCopy[key])
			getOptionsContainer().queue_sort()
		else :
			pass
		
func getOptionsContainer() :
	return $Panel/CenterContainer/Window/VBoxContainer/VBoxContainer

func _on_save_pressed() -> void:
	updateOptionDict()
	IGOptions.saveAndUpdateIGOptions(optionDictCopy)

signal finished
func _on_return_pressed() -> void:
	emit_signal("finished")
	queue_free()

const tutorialListLoader = preload("res://Screens/GameScreen/Tutorials/tutorial_list.tscn")
func _on_tutorials_pressed() -> void:
	var myTutorialList = tutorialListLoader.instantiate()
	add_child(myTutorialList)
	myTutorialList.initialise(optionDictCopy["individualTutorialDisable"].duplicate())
	myTutorialList.connect("tutorialListFinished", _on_tutorial_list_finished)
	myTutorialList.nestedPopupInit(self)

func _on_tutorial_list_finished(newDict : Dictionary) :
	optionDictCopy["individualTutorialDisable"] = newDict
	
func getCheckboxValue(key) -> bool :
		return getOptionsContainer().get_child(1+key).is_pressed()

func updateOptionDict() :
	for key in IGOptions.optionNameDictionary.keys() :
		if (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.checkBox) :
			optionDictCopy[key] = getCheckboxValue(key)
		else :
			pass
