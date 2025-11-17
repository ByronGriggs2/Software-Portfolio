extends HBoxContainer

enum multiplierType {premultiplier, postmultiplier}
var myType : multiplierType

func _ready() :		
	for key in Definitions.attributeDictionary.keys() :
		var textBox = $Source.duplicate()
		add_child(textBox)
		#move_child(textBox, get_child_count()-2)
		textBox.name = Definitions.attributeDictionary[key]
		textBox.text = "value"
		textBox.visible = true
		#textBox.anchor_left = (1.0/(Definitions.attributeCount+3))+(key*(1.0/(Definitions.attributeCount+3)))
		#textBox.anchor_right = 2*(1.0/(Definitions.attributeCount+3))+(key*(1.0/(Definitions.attributeCount+3)))
	#custom_minimum_size.y = $Source.get_minimum_size().y
		
func setSource(val : String) :
	$Source.text = val
	
func setMultiplier(type : Definitions.attributeEnum, val : float) :
	#get_node(Definitions.attributeDictionary[type]).text = "x" + str(val)
	var typeString : String
	if (myType == multiplierType.premultiplier) :
		typeString = "Premultiplier"
	elif (myType == multiplierType.postmultiplier) :
		typeString = "Postmultiplier"
	var tempText = ModifierPacket.getStrOrNull_static(typeString, val, "", false)
	if (tempText == null) :
		if (myType == multiplierType.premultiplier) :
			tempText = "x1.0"
		elif (myType == multiplierType.postmultiplier) :
			tempText = "x0.0"
	get_node(Definitions.attributeDictionary[type]).text = tempText
func makeTitle() :
	$Source.text = ""
	$Source.add_theme_font_size_override("normal_font_size", 18)
	for key in Definitions.attributeDictionary.keys() :
		var current = get_node(Definitions.attributeDictionary[key])
		current.add_theme_font_size_override("normal_font_size", 18)
		current.text = Definitions.attributeDictionaryShort[key]
		#current.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	#custom_minimum_size.y = $Source.get_minimum_size().y
