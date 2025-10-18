extends Control

func initialise(equipmentName : String) :
	var newItem = SceneLoader.createEquipmentScene(equipmentName)
	add_child(newItem)
	var HBox = newItem.get_node("SuperButton").get_node("HBoxContainer")
	var newText = $SampleTextLabel.duplicate()
	HBox.add_child(newText)
	newText.visible = true
	newText.text = newItem.getTitle()
	newItem.connect("wasSelected", _on_button_selected)
	
func getItemSceneRef() :
	return get_child(1)

signal wasSelected
func _on_button_selected(itemSceneRef) :
	emit_signal("wasSelected", itemSceneRef)
