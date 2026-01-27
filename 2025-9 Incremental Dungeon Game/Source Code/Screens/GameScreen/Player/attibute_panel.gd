extends PanelContainer

func _ready() :
	for key in Definitions.attributeDictionary.keys() :
		var newText = $SampleEntry.duplicate()
		$VBoxContainer/AttributeDisplay.add_child(newText)
		newText.visible = true
		newText.get_node("Name").text = " " + Definitions.attributeDictionary[key] + ": "

func initialise(attributeNumbers : Array[NumberClass]) :
	for key in Definitions.attributeDictionary.keys() :
		$VBoxContainer/AttributeDisplay.get_child(key).get_node("Number").setNumberReference(attributeNumbers[key])
