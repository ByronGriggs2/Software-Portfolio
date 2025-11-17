extends HBoxContainer
enum bonusType {prebonus,postbonus}
var myType : bonusType
func _ready() :		
	for key in Definitions.attributeDictionary.keys() :
		var textBox = $Source.duplicate()
		add_child(textBox)
		textBox.name = Definitions.attributeDictionary[key]
		textBox.text = "value"
		#textBox.anchor_left = (1.0/(Definitions.attributeCount+1))+(key*(1.0/(Definitions.attributeCount+1)))
		#textBox.anchor_right = 2*(1.0/(Definitions.attributeCount+1))+(key*(1.0/(Definitions.attributeCount+1)))
	#custom_minimum_size.y = $Source.get_minimum_size().y
		
func setSource(val : String) :
	$Source.text = val
	
func setBonus(type : Definitions.attributeEnum, val : float) :
	#get_node(Definitions.attributeDictionary[type]).text = "x" + str(val)
	var typeString : String
	if (myType == bonusType.prebonus) :
		typeString = "Prebonus"
	elif (myType == bonusType.postbonus) :
		typeString = "Postbonus"
	var tempText = ModifierPacket.getStrOrNull_static(typeString, val, "", false)
	if (tempText == null) :
		tempText = "+0.0"
	get_node(Definitions.attributeDictionary[type]).text = tempText

func makeTitle() :
	$Source.text = ""
	$Source.add_theme_font_size_override("normal_font_size", 18)
	#$Source.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	for key in Definitions.attributeDictionary.keys() :
		var current = get_node(Definitions.attributeDictionary[key])
		current.add_theme_font_size_override("normal_font_size", 18)
		current.text = Definitions.attributeDictionaryShort[key]
	#custom_minimum_size.y = $Source.get_minimum_size().y

func getMinSize() :
	var minSize = 0
	for child in get_children() :
		if (child.visible) :
			if (child.size.x > minSize) :
				minSize = child.size.x
	return minSize

func setMinSize(val) :
	for child in get_children() :
		if (child.visible) :
			child.custom_minimum_size = Vector2(val,0)
