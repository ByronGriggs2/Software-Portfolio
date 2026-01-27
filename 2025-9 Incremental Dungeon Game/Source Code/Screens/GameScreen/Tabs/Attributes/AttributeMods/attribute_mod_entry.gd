extends HBoxContainer
var myType : String = ""
func _ready() :		
	for key in Definitions.attributeDictionary.keys() :
		var textBox = $Sample.duplicate()
		add_child(textBox)
		textBox.visible = true
		textBox.name = Definitions.attributeDictionary[key]
		textBox.text = "value"
		#textBox.anchor_left = (1.0/(Definitions.attributeCount+1))+(key*(1.0/(Definitions.attributeCount+1)))
		#textBox.anchor_right = 2*(1.0/(Definitions.attributeCount+1))+(key*(1.0/(Definitions.attributeCount+1)))
	#custom_minimum_size.y = $Source.get_minimum_size().y
		#
#func setSource(val : String) :
	#$Source.text = val
	
func setMod(type : Definitions.attributeEnum, val : float) :
	var value = ModifierPacket.getStrOrNull_static(myType, val, "", false)
	if (value == null) :
		if (myType == "Premultiplier") :
			value = "x1.00"
		else :
			value = "--"
	get_child(1+type).text = " " + value + " "

func makeTitle(newType : String) :
	setType(newType)
	for key in Definitions.attributeDictionary.keys() :
		var current = get_child(1+key)
		current.add_theme_font_size_override("normal_font_size", 18)
		current.text = Definitions.attributeDictionaryShort[key]
	#custom_minimum_size.y = $Source.get_minimum_size().y
	
func setType(val : String) :
	myType = val

#func getMinSize() :
	#var minSize = 0
	#for child in get_children() :
		#if (child.visible) :
			#if (child.size.x > minSize) :
				#minSize = child.size.x
	#return minSize
#
#func setMinSize(val) :
	#for child in get_children() :
		#if (child.visible) :
			#child.custom_minimum_size = Vector2(val,0)
