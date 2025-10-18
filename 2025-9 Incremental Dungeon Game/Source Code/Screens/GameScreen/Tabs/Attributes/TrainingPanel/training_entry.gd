extends Panel

var myEntry : AttributeTraining = null

func setResource(entry : String) -> void :
	myEntry = load(entry)
	$HBoxContainer/Name.text = myEntry.text
	for key in Definitions.attributeDictionary.keys() :
		var currentNode = $HBoxContainer.get_node(Definitions.attributeDictionary[key])
		currentNode.text = currentNode.text + str(myEntry.getScaling(key))

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
	
func setNameWidth(val) :
	$HBoxContainer/Name.custom_minimum_size.x = val
	
func getNameWidth() :
	return $HBoxContainer/Name.get_minimum_size().x
