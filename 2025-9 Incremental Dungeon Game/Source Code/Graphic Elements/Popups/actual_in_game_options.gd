extends "res://Graphic Elements/popups/my_popup.gd"

const myCheckBox = preload("res://Graphic Elements/Buttons/my_check_box.tscn")
const labelLoader = preload("res://Graphic Elements/Tooltips/encyclopedia_text_label.tscn")
const myOffset : int = 5
var optionDictCopy : Dictionary = {}

func _ready() :
	optionDictCopy = IGOptions.getIGOptionsCopy()
	for key in IGOptions.optionNameDictionary.keys() :
		if (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.checkBox) :
			var newElement = myCheckBox.instantiate()
			redLambda(newElement, key)
			newElement.setText(IGOptions.optionNameDictionary[key])
			newElement.set_pressed_no_signal(optionDictCopy[key])
			getOptionsContainer().queue_sort()
		elif (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.dropdown) :
			var con = HBoxContainer.new()
			redLambda(con, key)
			con.alignment = BoxContainer.ALIGNMENT_BEGIN
			var title = labelLoader.instantiate()
			con.add_child(title)
			title.add_theme_font_size_override("normal_font_size", 20)
			title.currentLayer = self.layer
			title.size_flags_horizontal = Control.SIZE_SHRINK_BEGIN
			title.disableWrapping()
			title.custom_minimum_size = Vector2(0,0)
			await title.setText(IGOptions.optionNameDictionary[key])
			title.makeAllTextBlack()
			var newElement = OptionButton.new()
			con.add_child(newElement)
			newElement.add_theme_font_size_override("normal_font_size", 20)
			newElement.size_flags_horizontal = Control.SIZE_SHRINK_BEGIN
			for val in IGOptions.dropdownDictionary[key] :
				newElement.add_item(val)
			newElement.select(optionDictCopy[key])
		else :
			pass
		
func redLambda(elem : Node, key) :
	getOptionsContainer().add_child(elem)
	getOptionsContainer().move_child(elem, key+myOffset)

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
	
const combatRewardLoader = preload("res://Graphic Elements/popups/combat_reward_behaviour.tscn")
func _on_combat_reward_behaviour_my_pressed(_emitter) -> void:
	var combatRewardSettings = combatRewardLoader.instantiate()
	add_child(combatRewardSettings)
	combatRewardSettings.initialise(optionDictCopy["individualEquipmentTake"].duplicate())
	combatRewardSettings.connect("finished", _on_combat_reward_settings_finished)
	combatRewardSettings.nestedPopupInit(self)

func _on_combat_reward_settings_finished(newSettings : Dictionary) :
	optionDictCopy["individualEquipmentTake"] = newSettings
	
func getCheckboxValue(key) -> bool :
		return getOptionsContainer().get_child(1+key).is_pressed()

func updateOptionDict() :
	for key in IGOptions.optionNameDictionary.keys() :
		if (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.checkBox) :
			optionDictCopy[key] = getCheckboxValue(key)
		elif (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.dropdown) :
			optionDictCopy[key] = getOptionsContainer().get_child(key+myOffset).get_child(1).get_selected()
		else :
			pass

const encyclopediaLoader = preload("res://Graphic Elements/Tooltips/encyclopedia_list.tscn")
func _on_encyclopedia_pressed() -> void:
	var encyclopedia = encyclopediaLoader.instantiate()
	add_child(encyclopedia)
	encyclopedia.nestedPopupInit(self)

const beastiaryLoader = preload("res://Screens/GameScreen/beastiary.tscn")
func _on_beastiary_pressed() -> void:
	var beast = beastiaryLoader.instantiate()
	add_child(beast)
	beast.nestedPopupInit(self)

const keybindsLoader = preload("res://Screens/MainOptions/keybinds.tscn")
func _on_keybinds_pressed() -> void:
	var keybinds = keybindsLoader.instantiate()
	add_child(keybinds)
	keybinds.nestedPopupInit(self)
