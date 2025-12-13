extends VBoxContainer

func _ready() :
	for key in Definitions.otherStatDictionary.keys() :
		var newText = $SampleEntry.duplicate()
		$PanelContainer/OtherStatDisplay.add_child(newText)
		newText.visible = true
		newText.get_node("Name").text = " " + Definitions.otherStatDictionary[key] + ": "

func initialise(OtherStatRefs : Array[NumberClass]) :
	for key in Definitions.otherStatDictionary.keys() :
		$PanelContainer/OtherStatDisplay.get_child(key).get_node("Number").setNumberReference(OtherStatRefs[key])
