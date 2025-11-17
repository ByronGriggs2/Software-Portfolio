extends PanelContainer

var myEntry : AttributeTraining = null

func _ready() :
	for key in Definitions.attributeDictionary.keys() :
		var newEntry = $HBoxContainer/HBoxContainer/Sample.duplicate()
		$HBoxContainer/HBoxContainer.add_child(newEntry)
		newEntry.name = Definitions.attributeDictionary[key]
		newEntry.visible = true

func setResource(newEntry : AttributeTraining) -> void :
	myEntry = newEntry
	$HBoxContainer/Name.text = myEntry.text
	for key in Definitions.attributeDictionary.keys() :
		var scaling = myEntry.getScaling(key)
		if (scaling == 0) :
			$HBoxContainer/HBoxContainer.get_node(Definitions.attributeDictionary[key]).text = "-"
		else :
			$HBoxContainer/HBoxContainer.get_node(Definitions.attributeDictionary[key]).text = str(scaling)

signal requestedEnable
signal requestedDisable
func _on_check_button_toggled(toggled_on: bool) -> void:
	if (toggled_on) :
		emit_signal("requestedEnable", self)
	else :
		emit_signal("requestedDisable", self)
		
func getResource() :
	return myEntry
	
func clearButton() :
	$HBoxContainer/CheckButton.button_pressed = false
func setButton() : 
	$HBoxContainer/CheckButton.button_pressed = true
	
#func setNameWidth(val) :
	#$HBoxContainer/Name.custom_minimum_size.x = val
	#
#func getNameWidth() :
	#return $HBoxContainer/Name.get_minimum_size().x


func _on_name_resized() -> void:
	var nameWidth = $HBoxContainer/Name.size.x
	var width = 250-$HBoxContainer/CheckButton.size.x-nameWidth-3*$HBoxContainer.get_theme_constant("separation")+12-5
	$HBoxContainer/Spacer.custom_minimum_size = Vector2(width,0)
