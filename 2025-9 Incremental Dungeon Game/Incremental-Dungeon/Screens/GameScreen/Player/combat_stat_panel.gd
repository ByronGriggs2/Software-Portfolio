extends VBoxContainer

func _ready() :
	#for key in Definitions.baseStatDictionary.keys() :
		#var newText = $SampleEntry.duplicate()
		#$PanelContainer/CombatStatDisplay.add_child(newText)
		#newText.visible = true
		#newText.get_node("Name").text = " " + Definitions.baseStatDictionary[key] + ": "
	for key in Definitions.baseStatDictionary.keys() :
		var newText = $SampleEntry.duplicate()
		$PanelContainer/CombatStatDisplay.add_child(newText)
		newText.get_node("Name").setText(Definitions.baseStatDictionary[key])
		newText.get_node("Name").currentLayer += 1
	for elem in $PanelContainer/CombatStatDisplay.get_children() :
		if (elem.get_node("Name").updateRunning) :
			await elem.get_node("Name").doneRunning
		elem.visible = true

func initialise(CombatStatRefs : Array[NumberClass]) :
	for key in Definitions.baseStatDictionary.keys() :
		$PanelContainer/CombatStatDisplay.get_child(key).get_node("Number").setNumberReference(CombatStatRefs[key])
