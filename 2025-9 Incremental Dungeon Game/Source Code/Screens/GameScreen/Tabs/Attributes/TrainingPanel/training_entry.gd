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
	$HBoxContainer/HBoxContainer2/Name.text = myEntry.text
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
	$HBoxContainer/HBoxContainer2/PanelContainer/CheckButton.button_pressed = false
func setButton() : 
	$HBoxContainer/HBoxContainer2/PanelContainer/CheckButton.button_pressed = true
