extends Panel

func _ready() :
	addModSet("Source")
	
const modEntryLoader = preload("res://Screens/GameScreen/Tabs/Attributes/AttributeMods/attribute_mod_entry.tscn")
const labelLoader = preload("res://Graphic Elements/Tooltips/encyclopedia_text_label.tscn")

func addModSet(sourceName : String) :
	var newSource = labelLoader.instantiate()
	getSourceCol().add_child(newSource)
	newSource.disableWrapping()
	newSource.size_flags_horizontal = Control.SIZE_SHRINK_BEGIN
	newSource.setText(sourceName)
	if (getSourceCol().get_child_count() == 2) :
		newSource.add_theme_font_size_override("normal_font_size", 18)
		newSource.custom_minimum_size = Vector2(0,0)
	else :
		newSource.add_theme_font_size_override("normal_font_size", 14)
		newSource.custom_minimum_size = Vector2(0,0)
	var leftMod = modEntryLoader.instantiate()
	getLeftCol().add_child(leftMod)
	leftMod.setType("Pre" + myType)
	var rightMod = modEntryLoader.instantiate()
	getRightCol().add_child(rightMod)
	rightMod.setType("Post" + myType)
	if (myType == "bonus") :
		rightMod.visible = false
	
func getSourceCol() :
	return $PanelContainer/HBoxContainer/Source
func getLeftCol() :
	return $PanelContainer/HBoxContainer/Left
func getRightCol() :
	return $PanelContainer/HBoxContainer/Right
	
func findEntryIndex(entryName : String) :
	var children = getSourceCol().get_children()
	for index in range(1,children.size()) :
		if (children[index].getText() == entryName) :
			return index
	return null
	
func setMods(source : String, val : Array[float]) :
	var index = findEntryIndex(source)
	if (index == null) :
		addModSet(source)
	index = findEntryIndex(source)
	for key in Definitions.attributeDictionary.keys() :
		getLeftCol().get_child(index).setMod(key, val[key])
		if (myType != "bonus") :
			getRightCol().get_child(index).setMod(key, val[Definitions.attributeCount+key])
		
#func setBonusSingle(entryReference, type : Definitions.attributeEnum, val : float) :
	#entryReference.setBonus(type, val)
	
#########################
#var playerClass_comm : CharacterClass = null
#var waitingForPlayerClass : bool = false
#signal playerClassRequested
#signal playerClassReceived
#
#func getPlayerClass() -> CharacterClass:
	#waitingForPlayerClass = true
	#emit_signal("playerClassRequested", self)
	#if (waitingForPlayerClass) :
		#await playerClassReceived
	#return playerClass_comm
	#
#func providePlayerClass(val : CharacterClass) :
	#playerClass_comm = val
	#waitingForPlayerClass = false
	#emit_signal("playerClassReceived")
var myType : String = ""
#var catPic = preload("res://Resources/Textures/PlayerTexture/felids/cat_1.png")
##########################
func setType(type : String) :
	if (type == "bonus") :
		$Title.text = "Base Attributes"
	elif (type == "multiplier") :
		$Title.text = "Attribute Multipliers"
	else :
		return
	myType = type
	var leftChildren = getLeftCol().get_children()
	for index in range(0,leftChildren.size()) :
		if (index == 0 && type == "multiplier") :
			leftChildren[index].setText("Multipliers")
		elif (index == 0 && type == "bonus") :
			leftChildren[index].setText ("Bonuses")
		else :
			leftChildren[index].makeTitle("hello")
	var rightChildren = getRightCol().get_children()
	for index in range(0,rightChildren.size()) :
		if (index == 0 && type == "multiplier") :
			rightChildren[index].setText("Standard Multiplier")
		elif (type == "bonus") :
			rightChildren[index].visible = false
		else :
			rightChildren[index].makeTitle("hello")
	if (type == "bonus") :
		#getRightCol().size_flags_horizontal = SIZE_SHRINK_END
		var newPic = TextureRect.new()
		getRightCol().add_child(newPic)
		newPic.name = "CatPic"
		#newPic.texture = catPic
		newPic.texture_filter = CanvasItem.TEXTURE_FILTER_NEAREST_WITH_MIPMAPS
		newPic.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
		newPic.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT
		newPic.size_flags_horizontal = Control.SIZE_FILL
		newPic.size_flags_vertical = Control.SIZE_EXPAND_FILL
		#var newPic2 = newPic.duplicate()
		#getRightCol().add_child(newPic2)
		
	#var pluralSuffix : String = ""
	#if (type == "bonus") :
		#pluralSuffix = "es"
	#else :
		#pluralSuffix = "s"
	#for index in range(0,leftChildren.size()) :
		#if (index == 0) :
			#leftChildren[index].setText("Pre" + type + pluralSuffix)
		#else :
			#leftChildren[index].makeTitle("Pre"+ type)
	#var rightChildren = getRightCol().get_children()
	#for index in range(0,rightChildren.size()) :
		#if (index == 0) :
			#rightChildren[index].setText("Post"+type + pluralSuffix)
		#else :
			#rightChildren[index].makeTitle("Post"+type)

#func getEntryReference(type : bonusType, myName : String) :
	#if (myName == "Subtitle" || myName == "Title") :
		#return null
	#var parent
	#if (type == bonusType.prebonus) :
		#parent = $Panel/Prebonuses
	#elif (type == bonusType.postbonus) :
		#parent = $Panel/Postbonuses		
	#for child in parent.get_children() :
		#if (child.name == myName) :
			#return child
	#return null
		#
func getEmptyArr() -> Array[float] :
	var empty : Array[float]
	for key in Definitions.attributeDictionary.keys() :
		empty.append(0)
		empty.append(0)
	return empty
	
func updateSize() :
	var maxSize = 0
	for child in getSourceCol().get_children() :
		if (child.size.x > maxSize) :
			maxSize = child.size.x
	getSourceCol().custom_minimum_size = Vector2(maxSize,0)
#
func myUpdate(attributeMods : Array[NumberClass]) :
	#updateSize()
	var preMods : Array[Dictionary] = []
	var postMods : Array[Dictionary] = []
	if (myType == "bonus") :
		for item in attributeMods :
			preMods.append(item.getPrebonusesRaw())
			postMods.append(item.getPostbonusesRaw())
	elif (myType == "multiplier") :
		for item in attributeMods :
			preMods.append(item.getPremultipliersRaw())
			postMods.append(item.getPostmultipliersRaw())
	else :
		return
	var combinedKeys : Array[String]
	for attribute in preMods :
		for source in attribute.keys() :
			if (combinedKeys.find(source, 0) == -1) :
				combinedKeys.append(source)
	for attribute in postMods :
		for source in attribute.keys() :
			if (combinedKeys.find(source, 0) == -1) :
				combinedKeys.append(source)
	for key in combinedKeys :
		var tempArr = getEmptyArr()
		for attributeIndex in range(0,preMods.size()) :
			var tempVal = preMods[attributeIndex].get(key)
			if (tempVal == null) :
				if (myType == "multiplier") :
					tempVal = 1
				else :
					tempVal = 0
			tempArr[attributeIndex] = tempVal
		for attributeIndex in range(0,postMods.size()) :
			var tempVal = postMods[attributeIndex].get(key)
			if (tempVal == null) :
				tempVal = 0
			tempArr[Definitions.attributeCount + attributeIndex] = tempVal
		setMods(key, tempArr)
	#updateSize()
	
#func updateSize() :
	#updateSize2($Panel/Prebonuses)
	#updateSize2($Panel/Postbonuses)
	#
#func updateSize2(parent : Node) :
	#var minSize = 0
	#for child in parent.get_children() :
		#if (!(child.has_method("getMinSize"))) :
			#continue
		#var childSize = child.getMinSize()
		#if (childSize > minSize) :
			#minSize = childSize
	#for child in parent.get_children() :
		#if (!(child.has_method("getMinSize"))) :
			#continue
		#child.setMinSize(minSize)
				#
#func updateEntrySingle(type : bonusType, entryName : String, attribute : Definitions.attributeEnum, val : float) :
	#var entryRef = getEntryReference(type, entryName)
	#if (entryRef == null) :
		#addBonusSet(type, getEmptyArr(), entryName, entryName)
		#entryRef = getEntryReference(type, entryName)
	#setBonusSingle(entryRef, attribute, val)
