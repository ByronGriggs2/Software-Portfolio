extends PanelContainer

func _ready() :
	for key in Definitions.attributeDictionary.keys() :
		var newText = $SampleEntry.duplicate()
		$VBoxContainer/AttributeDisplay.add_child(newText)
		newText.get_node("Name").setText(Definitions.attributeDictionary[key])
		newText.get_node("Name").currentLayer += 1
	for elem in $VBoxContainer/AttributeDisplay.get_children() :
		if (elem.get_node("Name").updateRunning) :
			await elem.get_node("Name").doneRunning
		elem.visible = true

func initialise(attributeNumbers : Array[NumberClass]) :
	for key in Definitions.attributeDictionary.keys() :
		$VBoxContainer/AttributeDisplay.get_child(key).get_node("Number").setNumberReference(attributeNumbers[key])
