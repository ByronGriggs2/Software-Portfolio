extends VBoxContainer

func _ready() :
	for key in Definitions.baseStatDictionary.keys() :
		var newText = $SampleEntry.duplicate()
		$PanelContainer/DerivedStatDisplay.add_child(newText)
		newText.visible = true
		newText.get_node("Name").text = " " + Definitions.baseStatDictionary[key] + ": "

func initialise(CombatStatRefs : Array[NumberClass]) :
	for key in Definitions.baseStatDictionary.keys() :
		$PanelContainer/DerivedStatDisplay.get_child(key).get_node("Number").setNumberReference(CombatStatRefs[key])
