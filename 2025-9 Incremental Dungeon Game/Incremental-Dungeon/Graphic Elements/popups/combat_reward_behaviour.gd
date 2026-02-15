extends "res://Graphic Elements/popups/my_popup.gd"

var workingDict : Dictionary = {}

const forcedInclude = [
	"scraps",
	"magic_stick_int",
	"magic_stick_str",
	"shiv"
]

func initialise() :
	Helpers.highVisScroll($Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/ScrollContainer)
	var options = IGOptions.getIGOptionsCopy()
	var globalEncyclopedia = IGOptions["optionDict"][IGOptions.options.globalEncyclopedia]
	var allItems = EquipmentDatabase.getAllEquipment().duplicate()
	var index = 0
	while (index < allItems.size()) :
		var item : Equipment = allItems[index]
		if (!item.equipmentGroups.isEligible && !item.equipmentGroups.isSignature && forcedInclude.find(item.getItemName()) == -1) :
			allItems.remove_at(index)
		else :
			index += 1
	allItems.sort_custom(func(a,b):return a.getName()<b.getName())
	#workingDict = startingSettings
	var encounteredItems
	if (globalEncyclopedia) :
		encounteredItems = SaveManager.getGlobalSettings()["globalEncyclopedia"]["items"]
	else :
		encounteredItems = IGOptions.getIGOptionsCopy()["encounteredItems"]
	for item in encounteredItems :
		if (workingDict.get(item) == null) :
			workingDict[item] = 0
	for item in allItems :
		if (encounteredItems.find(item.getItemName()) == -1) :
			var newEntry = getEntries().get_node("Sample2").duplicate()
			getEntries().add_child(newEntry)
			newEntry.visible = true
		else :
			var newEntry = getEntries().get_node("Sample").duplicate()
			getEntries().add_child(newEntry)
			newEntry.connect("wasSelected", _on_entry_selected)
			newEntry.visible = true
			newEntry.initialise(item.createSampleCopy())
	#if (!startingSettings.keys().is_empty()) :
		#var children = getEntries().get_children()
		#for index in range(2, children.size()) :
			#if (!(children[index] is Button)) : 
				#getDetails().setItemSceneRefBase(children[index].getItemSceneRef())
				#children[index].select()
				#break

func _on_entry_selected(emitter) :
	for child in getEntries().get_children() :
		if (!(child is Button) && child.getItemSceneRef() != emitter && child.name != "Sample") :
			child.deselect()
	getDetails().setItemSceneRefBase(emitter)
	
func getEntries() :
	return $Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/ScrollContainer/VBoxContainer
func getDetails() :
	return $Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/EquipmentDetails
signal finished
func _on_return_button_pressed() -> void:
	emit_signal("finished")
	queue_free()
	
func _on_equipment_details_individual_equipment_take_changed(itemSceneRef : Node, index : int) -> void:
	workingDict[itemSceneRef.getItemName()] = index

func _on_line_edit_text_changed(new_text: String) -> void:
	if (new_text == "") :
		for child in getEntries().get_children() :
			if (child == getEntries().get_node("Sample") || child == getEntries().get_node("Sample2")) :
				continue
			child.visible = true
		return
	for child in getEntries().get_children() :
		if (child == getEntries().get_node("Sample") || child == getEntries().get_node("Sample2")) :
			continue
		if (child is Button) :
			child.visible = false
		elif (child.getText().to_upper().find(new_text.to_upper()) == -1) :
			child.visible = false
		else :
			child.visible = true

func _on_my_check_box_2_pressed() -> void:
	if ($Panel/CenterContainer/Window/VBoxContainer/HBoxContainer2/myCheckBox.isPressed()) :
		for child in getEntries().get_children() :
			if (child == getEntries().get_node("Sample") || child == getEntries().get_node("Sample2")) :
				continue
			child.visible = true
		$Panel/CenterContainer/Window/VBoxContainer/HBoxContainer2/LineEdit.clear()
	else :
		for child in getEntries().get_children() :
			if (child == getEntries().get_node("Sample") || child == getEntries().get_node("Sample2")) :
				continue
			if (child is Button) :
				child.visible = false
