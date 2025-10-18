extends Control

func _ready() :		
	for key in Definitions.attributeDictionary.keys() :
		var textBox = $Source.duplicate()
		add_child(textBox)
		textBox.name = Definitions.attributeDictionary[key]
		textBox.text = "value"
		textBox.anchor_left = (1.0/(Definitions.attributeCount+1))+(key*(1.0/(Definitions.attributeCount+1)))
		textBox.anchor_right = 2*(1.0/(Definitions.attributeCount+1))+(key*(1.0/(Definitions.attributeCount+1)))
	custom_minimum_size.y = $Source.get_minimum_size().y
		
func setSource(val : String) :
	$Source.text = val
	
func setMultiplier(type : Definitions.attributeEnum, val : float) :
	get_node(Definitions.attributeDictionary[type]).text = "x" + str(val)

func makeTitle() :
	$Source.text = "Source"
	$Source.add_theme_font_size_override("normal_font_size", 25)
	for key in Definitions.attributeDictionary.keys() :
		var current = get_node(Definitions.attributeDictionary[key])
		current.add_theme_font_size_override("normal_font_size", 25)
		current.text = Definitions.attributeDictionary[key]
	custom_minimum_size.y = $Source.get_minimum_size().y
