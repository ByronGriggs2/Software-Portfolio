extends "res://Graphic Elements/popups/my_popup.gd"

var workingDict : Dictionary = {}

func initialise(startingSettings : Dictionary) :
	workingDict = startingSettings
	for item in IGOptions.getIGOptionsCopy()["encounteredItems"] :
		if (workingDict.get(item) == null) :
			workingDict[item] = 0
	for key in workingDict.keys() :
		var newEntry = getEntries().get_node("Sample").duplicate()
		getEntries().add_child(newEntry)
		newEntry.connect("wasSelected", _on_entry_selected)
		newEntry.visible = true
		newEntry.initialise(key)
	if (!startingSettings.keys().is_empty()) :
		getDetails().setItemSceneRefBase(getEntries().get_child(1).getItemSceneRef())

func _on_entry_selected(emitter) :
	for child in getEntries().get_children() :
		if (child.getItemSceneRef() != emitter && child.name != "Sample") :
			child.deselect()
	getDetails().setItemSceneRefBase(emitter)
	
func getEntries() :
	return $Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/ScrollContainer/VBoxContainer
func getDetails() :
	return $Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/EquipmentDetails
signal finished
func _on_return_button_pressed() -> void:
	emit_signal("finished", workingDict)
	queue_free()
	
func _on_equipment_details_individual_equipment_take_changed(itemSceneRef : Node, index : int) -> void:
	workingDict[itemSceneRef.getItemName()] = index
