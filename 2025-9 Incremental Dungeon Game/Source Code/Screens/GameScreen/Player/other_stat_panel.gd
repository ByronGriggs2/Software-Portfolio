extends VBoxContainer

func _ready() :
	for key in Definitions.otherStatDictionary.keys() :
		var newText = $SampleEntry.duplicate()
		if (Definitions.otherStatEnum.routineSpeed_0 <= key && key <= Definitions.otherStatEnum.routineSpeed_4) :
			$PanelContainer/OtherStatDisplay/RoutineSpeedFoldout/VBoxContainer.add_child(newText)
			newText.visible = true
		elif (key == Definitions.otherStatEnum.routineSpeed_5) :
			$PanelContainer/OtherStatDisplay/RoutineSpeedFoldout/HBoxContainer.add_child(newText)
			newText.visible = true
		else :
			$PanelContainer/OtherStatDisplay.add_child(newText)
		if (key < Definitions.otherStatEnum.routineSpeed_0) :
			$PanelContainer/OtherStatDisplay.move_child(newText, key)
			newText.visible = true
			
		var symbol = ""
		if (key < Definitions.otherStatEnum.routineSpeed_0) :
			symbol = ""
		elif (key == Definitions.otherStatEnum.routineSpeed_4) :
			symbol = "└> " 
		else :
			symbol = "├> "
		newText.get_node("Name").text = " " + symbol + Definitions.otherStatDictionary[key] + ": "

func initialise(OtherStatRefs : Array[NumberClass]) :
	for key in Definitions.otherStatDictionary.keys() :
		if (Definitions.otherStatEnum.routineSpeed_0 <= key && key <= Definitions.otherStatEnum.routineSpeed_4) :
			$PanelContainer/OtherStatDisplay/RoutineSpeedFoldout/VBoxContainer.get_child(key-Definitions.otherStatEnum.routineSpeed_0).get_node("Number").setNumberReference(OtherStatRefs[key])
		elif (key == Definitions.otherStatEnum.routineSpeed_5) :
			var hbox = $PanelContainer/OtherStatDisplay/RoutineSpeedFoldout/HBoxContainer
			hbox.get_child(2).get_node("Number").setNumberReference(OtherStatRefs[key])
			hbox.get_child(2).get_node("Name").visible = false
		#var otherStat = $PanelContainer/OtherStatDisplay.get_child(key).get_node("Number")
		elif (key < Definitions.otherStatEnum.routineSpeed_0) :
			$PanelContainer/OtherStatDisplay.get_child(key).get_node("Number").setNumberReference(OtherStatRefs[key])
		else :
			## might be off by one idk
			$PanelContainer/OtherStatDisplay.get_child(key+1).get_node("Number").setNumberReference(OtherStatRefs[key])

func _on_texture_button_pressed() -> void:
	$PanelContainer/OtherStatDisplay/RoutineSpeedFoldout/VBoxContainer.visible = !$PanelContainer/OtherStatDisplay/RoutineSpeedFoldout/VBoxContainer.visible
