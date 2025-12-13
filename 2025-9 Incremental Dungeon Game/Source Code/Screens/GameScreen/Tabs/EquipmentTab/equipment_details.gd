extends Panel

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")

var currentItemSceneRef = null
@export var isInIGOptions : bool = false
@export var spriteScale : int = 15

func _ready() :
	resetDetails()
	
func setItemSceneRefBase(itemSceneRef) :
	currentItemSceneRef = itemSceneRef
	if (currentItemSceneRef == null) :
		resetDetails()
	if (itemSceneRef == null) :
		return
	setOptionButton(itemSceneRef)
	updateText()
	
func updateText_sprite() :#
	var mySprite = $Picture/CenterContainer/Sprite
	var newSprite = currentItemSceneRef.getSprite()
	mySprite.is32 = newSprite.is32
	mySprite.is48 = newSprite.is48
	mySprite.setTexture(newSprite.getTexture())
	mySprite.setRegionRect(newSprite.getRegionRect())
	if (isInIGOptions) :
		mySprite.setScale(10)
	else :
		mySprite.setScale(spriteScale)
	mySprite.updateSize()
	
func updateText_clearChildren(myText) :
	myText = ""
	for child in myText.get_children() :
		child.queue_free()
	while (myText.get_child_count() != 0) :
		await get_tree().process_frame
		
func updateText_weapon(myText) :
	$Text/VBoxContainer/Extra.text = ""
	myText = myText + "Attack Bonus:\n\t" + str(Helpers.myRound(currentItemSceneRef.getAttack(),3)) + "\n\n"
	myText = updateText_weapon_scaling(myText)
	myText = updateText_weapon_action(myText)
	myText += "\n"
	return myText
	
func updateText_weapon_scaling(myText) :
	myText = myText + "Scaling:\n\t"
	var letterCache : Array[String] = []
	for key in Definitions.attributeDictionary.keys() :
		if (key == Definitions.attributeEnum.DUR || key == Definitions.attributeEnum.SKI) :
			letterCache.append("--")
			continue
		var scalingVal = currentItemSceneRef.core.getScaling(key)
		myText += Definitions.attributeDictionaryShort[key] + ": " + Weapon.scalingToLetter(scalingVal) + " "
		letterCache.append(Weapon.scalingToLetter(scalingVal))
		if ((key as int) != Definitions.attributeDictionary.keys().size()-1) :
			myText += "          "
	myText += "\n\n"
	return myText
	
func updateText_weapon_action(myText) :
	myText += "Primary Attack: "
	var attack = currentItemSceneRef.getBasicAttack()
	myText += attack.getName() + "\n"
	myText += "\t\tType: " + Definitions.damageTypeDictionary[attack.getDamageType()] + "\n"
	myText += "\t\tPower: " + str(Helpers.myRound(attack.getPower(),3)) + "\n"
	myText += "\t\tWarmup: " + str(Helpers.myRound(attack.getWarmup(),3)) + " seconds\n"
	return myText

func updateText_armor(myText) :
	$Text/VBoxContainer/Extra.text = ""
	myText = "Physical defense:\n    " + str(currentItemSceneRef.getPhysicalDefense()) + "\n"
	myText = myText + "Magic defense:\n    " + str(currentItemSceneRef.getMagicDefense()) + "\n"
	myText = myText + "\n"
	return myText
	
func updateText_accessory(myText) :
	$Text/VBoxContainer/Extra.text = ""
	myText = ""
	return myText
	
func updateText_currency(myText) :
	$Text/VBoxContainer/Extra.setText("Currency")
	myText = ""
	return myText
	
func updateText_modifiers(myText) :
	var modifiers : ModifierPacket = currentItemSceneRef.core.getModifierPacket()
	var addEndl : bool = false
	for key in Definitions.attributeDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var modString = modifiers.getModStringOrNull_attr(key, subkey)
			if (modString != null) :
				myText += modString + "\n"
				addEndl = true
	for key in Definitions.baseStatDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var modString = modifiers.getModStringOrNull_combatStat(key,subkey)
			if (modString != null) :
				myText += modString + "\n"
				addEndl = true
	for key in Definitions.otherStatDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var modString = modifiers.getModStringOrNull_otherStat(key,subkey)
			if (modString != null) :
				myText += modString + "\n"
				addEndl = true
	if (addEndl) :
		myText += "\n"
	return myText
	
func updateText() :
	if (currentItemSceneRef == null) :
		return
	if (!myTooltips.is_empty()) :
		for child in myTooltips :
			child.queue_free()
		myTooltips.clear()
	updateText_sprite()
	$Text/VBoxContainer/Title.text = currentItemSceneRef.getTitle()
	var myText : String = ""
	if (currentItemSceneRef.core is Weapon) :
		myText = updateText_weapon(myText)
	elif (currentItemSceneRef.core is Armor) :
		myText = updateText_armor(myText)
	elif (currentItemSceneRef.core is Accessory) :
		myText = updateText_accessory(myText)
	elif (currentItemSceneRef.core is Currency) :
		myText = updateText_currency(myText)
	else :
		myText = ""
		$Text/VBoxContainer/Extra.text = ""
	myText = updateText_modifiers(myText)
	myText = myText + currentItemSceneRef.getDesc()
	$Text/VBoxContainer.visible = true
	$Picture/CenterContainer.visible = true
	$Text/VBoxContainer/Extra.visible = $Text/VBoxContainer/Extra.text != ""
	await $Text/VBoxContainer/Panel/ScrollContainer/Description.setText(myText)
	if (currentItemSceneRef.core is Weapon) :
		addWeaponScalingTooltips()

func resetDetails() :
	$Text/VBoxContainer.visible = false
	$Picture/CenterContainer.visible = false
	
func setOptionButton(itemSceneRef : Node) :
	var optionButton = $Text/VBoxContainer/HBoxContainer/OptionButton
	var chosenItem = IGOptions.getIGOptionsCopy()["individualEquipmentTake"].get(itemSceneRef.getItemName())
	if (chosenItem == null) :
		optionButton.select(0)
	else :
		optionButton.select(chosenItem)

signal individualEquipmentTakeChanged
func _on_option_button_item_selected(index: int) -> void:
	if (isInIGOptions) :
		emit_signal("individualEquipmentTakeChanged", currentItemSceneRef, index)
	else :
		var optionsCopy = IGOptions.getIGOptionsCopy()
		optionsCopy["individualEquipmentTake"][currentItemSceneRef.getItemName()] = index
		IGOptions.saveAndUpdateIGOptions(optionsCopy)

func updateFromOptions(optionDict : Dictionary) :
	if (currentItemSceneRef == null) :
		return
	var currentEquipSetting = optionDict["individualEquipmentTake"].get(currentItemSceneRef.getItemName())
	if (currentEquipSetting == null) :
		currentEquipSetting = 0
	$Text/VBoxContainer/HBoxContainer/OptionButton.select(currentEquipSetting)

var myTooltips : Array[Node] = []
## Attack Bonus
##   32

## Scaling : 
##   DEX: A      INT: A         STR: A
func addWeaponScalingTooltips() :
	const scalingStr : String = "Scaling:\n\t"
	const dexStr : String = "DEX: "
	const intStr : String = "INT: "
	const strStr : String = "STR: "
	const fontSize = 18
	var myText : RichTextLabel = $Text/VBoxContainer/Panel/ScrollContainer/Description
	var scalingStartIndex = myText.get_parsed_text().find(scalingStr)
	var DEXStartIndex = myText.get_parsed_text().find(dexStr, scalingStartIndex) + dexStr.length()
	var DEXEndIndex = myText.get_parsed_text().find(" ", DEXStartIndex)
	var INTStartIndex = myText.get_parsed_text().find(intStr, scalingStartIndex) + intStr.length()
	var INTEndIndex = myText.get_parsed_text().find(" ", INTStartIndex)
	var STRStartIndex = myText.get_parsed_text().find(strStr, scalingStartIndex) + strStr.length()
	var STREndIndex = myText.get_parsed_text().find(" ", STRStartIndex)
	var strArray : Array[String] = [
		"\t" + dexStr,
		myText.get_parsed_text().substr(DEXStartIndex, DEXEndIndex-DEXStartIndex),
		myText.get_parsed_text().substr(DEXEndIndex,INTStartIndex-DEXEndIndex),
		myText.get_parsed_text().substr(INTStartIndex, INTEndIndex-INTStartIndex),
		myText.get_parsed_text().substr(INTEndIndex, STRStartIndex-INTEndIndex),
		myText.get_parsed_text().substr(STRStartIndex, STREndIndex-STRStartIndex)
	]
	var strWidths = await Helpers.getTextWidthWaitFrameArray(fontSize, strArray)
	var lineNumber = 5
	
	if (currentItemSceneRef == null) :
		return
	for key in Definitions.attributeDictionary.keys() :
		if (key == Definitions.attributeEnum.DUR || key == Definitions.attributeEnum.SKI) :
			continue
		if (currentItemSceneRef.core.getScaling(key) == 0) :
			continue
		var lineHeight = myText.get_theme_font("normal_font").get_height(fontSize)
		var yCoord1 = (lineNumber-1) * lineHeight + lineHeight/4.0
		var xCoord1
		if (key == Definitions.attributeEnum.DEX) :
			xCoord1 = strWidths[0]
		elif (key == Definitions.attributeEnum.INT) :
			xCoord1 = strWidths[0] + strWidths[1] + strWidths[2]
		elif (key == Definitions.attributeEnum.STR) :
			xCoord1 = strWidths[0] + strWidths[1] + strWidths[2] + strWidths[3] + strWidths[4]
		else :
			return
		var yCoord2 = yCoord1 + lineHeight
		var xCoord2
		if (key == Definitions.attributeEnum.DEX) :
			xCoord2 = xCoord1+strWidths[1]
		elif (key == Definitions.attributeEnum.INT) :
			xCoord2 = xCoord1+strWidths[3]
		elif (key == Definitions.attributeEnum.STR) :
			xCoord2 = xCoord1+strWidths[5]
		else :
			return
		var offset = myText.global_position-global_position
		xCoord1 += offset.x
		xCoord2 += offset.x
		yCoord1 += offset.y
		yCoord2 += offset.y
		var newTooltip = tooltipLoader.instantiate()
		add_child(newTooltip)
		myTooltips.append(newTooltip)
		#newTooltip.setTitle(Definitions.attributeDictionaryShort[key] + " Scaling: " + letterCache[key])
		#newTooltip.setDesc(str(currentItemSceneRef.core.getScaling(key)))
		newTooltip.setTitle(str(Helpers.myRound(currentItemSceneRef.core.getScaling(key),3)))
		newTooltip.setDesc("")
		newTooltip.setTooltipWidth(80)
		newTooltip.setPos(Vector2(xCoord1, yCoord1), Vector2(xCoord2, yCoord2))
		
#func getLineWidths(myText : RichTextLabel, letterCache : Array[String], lineStartIndex, firstCharInLine) :
	#var textArray : Array[String] = []
	#for key in Definitions.attributeDictionary.keys() :
		#if (key == Definitions.attributeEnum.DUR || key == Definitions.attributeEnum.SKI) :
			#textArray.append("")
			#textArray.append("")
			#continue
		#var charIndex = myText.get_parsed_text().find(" " + letterCache[key] + " ", lineStartIndex)
		#textArray.append(letterCache[key])
		#textArray.append(myText.get_parsed_text().substr(firstCharInLine, charIndex-firstCharInLine))
		#lineStartIndex = charIndex + 1
	#textArray.append(" ")
	#return await Helpers.getTextWidthWaitFrameArray(myText.get_theme_font_size("font_size", "Button"), textArray)
