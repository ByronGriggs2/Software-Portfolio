extends "res://Graphic Elements/popups/my_popup.gd"

const myCheckBox = preload("res://Graphic Elements/Buttons/my_check_box.tscn")
const labelLoader = preload("res://Graphic Elements/Tooltips/encyclopedia_text_label.tscn")
const myOffset : int = 3
var optionDictCopy : Dictionary = {}

var tooltipRef = null
const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
func _ready() :
	optionDictCopy = IGOptions.getIGOptionsCopy()
	for key in IGOptions.optionNameDictionary.keys() :
		if (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.checkBox) :
			var newElement = myCheckBox.instantiate()
			redLambda(newElement, key)
			var text
			if (key == IGOptions.options.globalEncyclopedia) :
				text = "[color=#8A50A1]" + IGOptions.optionNameDictionary[key] + "[/color]"
				var tooltip : Control = tooltipLoader.instantiate()
				newElement.getTextRef().add_child(tooltip)
				tooltip.setTitle(IGOptions.optionNameDictionary[key])
				tooltip.setDesc("Use the encyclopedia maintained across all save files. This is intended for achievement hunters.\n\nTo reset this encyclopedia, see the settings in the main menu.")
				tooltip.set_anchors_preset(Control.PRESET_FULL_RECT)
				tooltip.setCurrentLayer(layer)
				tooltipRef = tooltip
				newElement.connect("resized", _on_globalEncyclopedia_resized)
			else :
				text = IGOptions.optionNameDictionary[key]
			newElement.setText(text)
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
	await $Panel/CenterContainer/Window/VBoxContainer/VBoxContainer/DiscardFilter.initialise(optionDictCopy["filter"])
	$Panel/CenterContainer/Window/VBoxContainer/VBoxContainer/DiscardFilter.setCurrentLayer(layer)
	
func _on_globalEncyclopedia_resized() :
	if (tooltipRef != null) :
		tooltipRef.custom_minimum_size = tooltipRef.get_parent().size
		
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
	
func getCheckboxValue(key) -> bool :
		return getOptionsContainer().get_child(myOffset+key).is_pressed()

func updateOptionDict() :
	for key in IGOptions.optionNameDictionary.keys() :
		if (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.checkBox) :
			optionDictCopy[key] = getCheckboxValue(key)
		elif (IGOptions.optionTypeDictionary[key] == IGOptions.optionType.dropdown) :
			optionDictCopy[key] = getOptionsContainer().get_child(key+myOffset).get_child(1).get_selected()
		else :
			pass
	optionDictCopy["filter"] = $Panel/CenterContainer/Window/VBoxContainer/VBoxContainer/DiscardFilter.getData()

const encyclopediaLoader = preload("res://Graphic Elements/popups/encyclopedia.tscn")
func _on_encyclopedia_pressed() -> void:
	var encyclopedia = encyclopediaLoader.instantiate()
	add_child(encyclopedia)
	encyclopedia.nestedPopupInit(self)

const keybindsLoader = preload("res://Screens/MainOptions/keybinds.tscn")
func _on_keybinds_pressed() -> void:
	var keybinds = keybindsLoader.instantiate()
	add_child(keybinds)
	keybinds.nestedPopupInit(self)

const creditsLoader = preload("res://Graphic Elements/popups/credits.tscn")
func _on_credits_pressed() -> void:
	var credits = creditsLoader.instantiate()
	add_child(credits)
	credits.nestedPopupInit(self)
