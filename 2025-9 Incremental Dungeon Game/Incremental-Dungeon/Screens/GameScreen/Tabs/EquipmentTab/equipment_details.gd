extends Panel

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")

var currentItemSceneRef = null
var currentlyEquippedItem : Equipment = null
func getItemSceneRef() :
	return currentItemSceneRef
@export var isInIGOptions : bool = false
@export var isInRewards : bool = false
@export var spriteScale : int = 12

signal currentlyEquippedItemRequested()
signal currentlyEquippedItemReceived
var currentlyEquipped_comm : Equipment = null
var waitingOnCurrentlyEquipped : bool = false
func getCurrentlyEquipped(type) -> Equipment :
	waitingOnCurrentlyEquipped = true
	emit_signal("currentlyEquippedItemRequested", self, type)
	if (waitingOnCurrentlyEquipped) :
		await currentlyEquippedItemReceived
	return currentlyEquipped_comm
func provideCurrentlyEquippedItem(val) :
	currentlyEquipped_comm = val
	waitingOnCurrentlyEquipped = false
	emit_signal("currentlyEquippedItemReceived")

var currentLayer = 0
func _ready() :
	if (isInIGOptions) :
		currentLayer = Helpers.getTopLayer() + 1
		$VBoxContainer/Text/VBoxContainer/HBoxContainer/EncyclopediaTextLabel.currentLayer = currentLayer
		$VBoxContainer/Text/VBoxContainer/Panel/ScrollContainer/VBoxContainer/Description.currentLayer = currentLayer
		$VBoxContainer/Picture/Elements.setLayer(currentLayer+1)
		$VBoxContainer/Text/VBoxContainer/TagContainerEquipment.setLayer(currentLayer+1)
	addElementTooltips()
	resetDetails()
	
func setItemSceneRefBase(itemSceneRef) :
	currentItemSceneRef = itemSceneRef
	if (isInRewards && itemSceneRef != null) :
		currentlyEquippedItem = await getCurrentlyEquipped(itemSceneRef.getType())
	updateElements()
	if (currentItemSceneRef == null) :
		resetDetails()
	else :
		$VBoxContainer/Text/VBoxContainer/TagContainerEquipment.setEquipment(currentItemSceneRef.core)
		setOptionButton(itemSceneRef)
		updateText()
	
func updateElements() :
	var elements = $VBoxContainer/Picture/Elements
	elements.clearAllSymbols()
	if (currentItemSceneRef == null || currentItemSceneRef.core.equipmentGroups == null) :
		return
	if (currentItemSceneRef.core.equipmentGroups.isEarth) :
		elements.setSymbol("Earth")
	if (currentItemSceneRef.core.equipmentGroups.isFire) :
		elements.setSymbol("Fire")
	if (currentItemSceneRef.core.equipmentGroups.isIce) :
		elements.setSymbol("Ice")
	if (currentItemSceneRef.core.equipmentGroups.isWater) :
		elements.setSymbol("Water")
	
func addElementTooltips() :
	return
	#var elements = $VBoxContainer/Picture/Elements
	#elements.setLayer(currentLayer+1)
	#var children = $VBoxContainer/Picture/Elements.get_children()
	#for index in range(0,children.size()) :
		#var newTooltip = tooltipLoader.instantiate()
		#children[index].add_child(newTooltip)
		#var upperLeft = Vector2(0,0)
		#var bottomRight = Vector2(16,16) * children[index].getScale()
		#var key
		#if (index == 0) :
			#key = "Earth"
		#elif (index == 1) :
			#key = "Fire"
		#elif (index == 2) :
			#key = "Ice"
		#else :
			#key = "Water"
		#newTooltip.initialise(key)
		#newTooltip.setPos(upperLeft, bottomRight)
		#newTooltip.currentLayer = currentLayer+1
	
func updateText_sprite() :#
	var mySprite = $VBoxContainer/Picture/CenterContainer/Sprite
	var newSprite = currentItemSceneRef.getSprite()
	mySprite.is32 = newSprite.is32
	mySprite.is48 = newSprite.is48
	mySprite.setTexture(newSprite.getTexture())
	mySprite.setRegionRect(newSprite.getRegionRect())
	mySprite.setFlipped(newSprite.getFlipped())
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
	$VBoxContainer/Text/VBoxContainer/Extra.text = ""
	if (isInIGOptions) :
		myText += "Typical "
	myText += "Bonus to Base DR:\n\t" + Helpers.engineeringRound(currentItemSceneRef.getAttack(),3)
	if (isInRewards && currentlyEquippedItem != null) :
		var current = currentlyEquippedItem as Weapon
		myText += getImprovementText(currentItemSceneRef.getAttack(), current.attackBonus, false,false)
	myText += "\n\n"
	myText = updateText_weapon_scaling(myText)
	myText = updateText_weapon_action(myText)
	myText += "\n"
	return myText

func getImprovementText(newVal, oldVal, inverted : bool, includeBrackets : bool) -> String :
	var oldMagnitude
	if (is_equal_approx(oldVal,0)) :
		oldMagnitude=99
	else :
		oldMagnitude = floor(log(abs(oldVal))/log(10))
	var newMagnitude
	if (is_equal_approx(newVal,0)) :
		newMagnitude = 99
	else :
		newMagnitude = floor(log(abs(newVal))/log(10))
	var improvement = newVal-oldVal
	var improvementMagnitude = floor(log(abs(improvement))/log(10))
	var resultantSigFigs
	if (improvementMagnitude >= oldMagnitude && improvementMagnitude >= newMagnitude) :
		resultantSigFigs = 3
	elif (improvementMagnitude < oldMagnitude && improvementMagnitude < newMagnitude) :
		resultantSigFigs = 3-(min(oldMagnitude,newMagnitude)-improvementMagnitude)
		if (resultantSigFigs <= 0) :
			improvement = 0
			resultantSigFigs = 3
	else :
		resultantSigFigs = 3
	
	var retVal : String = ""
	retVal += "\t\t"
	if (true) :
		retVal += "["
	if (is_equal_approx(improvement,0)) :
		retVal += "--"
	elif (improvement < 0) :
		if (inverted) :
			retVal += "[color=green]"
		else :
			retVal += "[color=red]"
	else :
		if (inverted) :
			retVal += "[color=red]+"
		else :
			retVal += "[color=green]+"
	if (!is_equal_approx(improvement,0)) :
		retVal += Helpers.engineeringRound(improvement, resultantSigFigs)
	if (is_equal_approx(improvement,0)) :
		retVal += ""
	else :
		retVal += "[/color]"
	if (true) :
		retVal += "]"
	return retVal
	
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
	var current : Weapon
	if (currentlyEquippedItem != null) :
		current = currentlyEquippedItem as Weapon
	myText += "Primary Attack: "
	var attack = currentItemSceneRef.getBasicAttack()
	myText += attack.getName() + "\n"
	myText += "\t\tType: " + Definitions.damageTypeDictionary[attack.getDamageType()]
	if (isInRewards && currentlyEquippedItem != null) :
		var newType = attack.getDamageType()
		var oldType = current.basicAttack.getDamageType()
		if (newType == oldType) :
			myText += "\t\t[--]"
		else :
			myText += "\t\t[[color=red]"+Definitions.damageTypeDictionary[oldType]+"[/color]]"
	myText += "\n"
	myText += "\t\tAction Power: " + Helpers.engineeringRound(attack.getPower(),3)
	if (isInRewards && currentlyEquippedItem != null) :
		myText += getImprovementText(attack.getPower(),current.basicAttack.getPower(), false,false)
	myText += "\n"
	myText += "\t\tWarmup: " + Helpers.engineeringRound(attack.getWarmup(),3) + " seconds"
	if (isInRewards && currentlyEquippedItem != null) :
		myText += getImprovementText(attack.getWarmup(), current.basicAttack.getWarmup(), true,false)
	myText += "\n"
	return myText

func updateText_armor(myText) :
	var current : Armor
	if (currentlyEquippedItem != null) :
		current = currentlyEquippedItem as Armor
	$VBoxContainer/Text/VBoxContainer/Extra.text = ""
	if (isInIGOptions) :
		myText += "Typical "
	myText += "Bonus to Base Physical Defense:\n    " + Helpers.engineeringRound(currentItemSceneRef.getPhysicalDefense(),3)
	if (isInRewards && currentlyEquippedItem != null) :
		myText += getImprovementText(currentItemSceneRef.getPhysicalDefense(), current.PHYSDEF, false,false)
	myText += "\n"
	if (isInIGOptions) :
		myText += "Typical "
	myText = myText + "Bonus to Base Magic Defense:\n    " + Helpers.engineeringRound(currentItemSceneRef.getMagicDefense(),3)
	if (isInRewards && currentlyEquippedItem != null) :
		myText += getImprovementText(currentItemSceneRef.getMagicDefense(), current.MAGDEF, false,false)
	myText += "\n"
	myText = myText + "\n"
	return myText
	
func updateText_accessory(myText) :
	$VBoxContainer/Text/VBoxContainer/Extra.text = ""
	myText += ""
	return myText
	
func updateText_currency(myText) :
	$VBoxContainer/Text/VBoxContainer/Extra.setText("")
	myText += ""
	return myText
	
func updateText_modifiers(myText) :
	var modifiers : ModifierPacket = currentItemSceneRef.core.getModifierPacket()
	var addEndl : bool = false
	for key in Definitions.attributeDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var modString = modifiers.getModStringOrNull_attr(key, subkey)
			if (modString != null) :
				if (isInIGOptions) :
					myText += "Typical "
				myText += modString + "\n"
				addEndl = true
	for key in Definitions.baseStatDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var modString = modifiers.getModStringOrNull_combatStat(key,subkey)
			if (modString != null) :
				myText += modString + "\n"
				addEndl = true
	for key in Definitions.otherStatDictionary.keys() :
		if (Definitions.otherStatEnum.routineSpeed_0 <= key && key <= Definitions.otherStatEnum.routineSpeed_4) :
			continue
		for subkey in ModifierPacket.StandardModifier.keys() :
			var modString = modifiers.getModStringOrNull_otherStat(key,subkey)
			if (modString != null) :
				myText += modString + "\n"
				addEndl = true
	if (addEndl) :
		myText += "\n"
	return myText

func getNewTempstr(subkey) :
	if (subkey == "Premultiplier") :
		return " Multiplier"
	elif (subkey == "Postmultiplier") :
		return " Standard Multiplier Bonus"
	elif (subkey == "Prebonus") :
		return " Base Bonus"
	else :
		return ""
	
func updateText_modifiers_REWARDS(myText) :
	var modifiers : ModifierPacket = currentItemSceneRef.core.getModifierPacket().duplicate(true)
	var currentMods = currentlyEquippedItem.getModifierPacket().duplicate(true)
	var addEndl : bool = false
	var stringList : Dictionary = {
		0 : [],
		1 : [],
		2 : [],
		3 : []
	}
	for key in Definitions.attributeDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var tempStr : String = Definitions.attributeDictionaryShort[key] + getNewTempstr(subkey) + "   "
			var newVal = modifiers.getMod("attribute",key,subkey)
			var oldVal = currentMods.getMod("attribute",key,subkey)
			if (((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0))) && !((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr += "--"
				if (subkey == "Premultiplier") :
					tempStr += getImprovementText(1,(oldVal),false,true)
				else :
					tempStr += getImprovementText(0,oldVal,false,true)
				stringList[0].append(tempStr+"\n")
			elif (((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0)))&&((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr = ""
			elif (!((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0)))&&!((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr = modifiers.getModStringOrNull_attr(key,subkey)
				tempStr += getImprovementText(newVal,oldVal,false,true)
				stringList[2].append(tempStr+"\n")
			else :
				tempStr = modifiers.getModStringOrNull_attr(key,subkey)
				tempStr += getImprovementText(newVal,oldVal,false,true)
				stringList[3].append(tempStr+"\n")
			if (tempStr != "") :
				addEndl = true
	for key in Definitions.baseStatDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var tempStr : String = Definitions.baseStatDictionaryShort[key] + getNewTempstr(subkey) + " \t"
			var newVal = modifiers.getMod("stat",key,subkey)
			var oldVal = currentMods.getMod("stat",key,subkey)
			if (((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0))) && !((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr += "--"
				if (subkey == "Premultiplier") :
					tempStr += getImprovementText(1,(oldVal),false,true)
				else :
					tempStr += getImprovementText(0,oldVal,false,true)
				stringList[0].append(tempStr+"\n")
			elif (((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0)))&&((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr = ""
			elif (!((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0)))&&!((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr = modifiers.getModStringOrNull_combatStat(key,subkey)
				tempStr += getImprovementText(newVal,oldVal,false,true)
				stringList[2].append(tempStr+"\n")
			else :
				tempStr = modifiers.getModStringOrNull_combatStat(key,subkey)
				tempStr += getImprovementText(newVal,oldVal,false,true)
				stringList[3].append(tempStr+"\n")
			if (tempStr != "") :
				addEndl = true
	for key in Definitions.otherStatDictionary.keys() :
		if (Definitions.otherStatEnum.routineSpeed_0 <= key && key <= Definitions.otherStatEnum.routineSpeed_4) :
			continue
		var localInverted : bool = false
		if (key == Definitions.otherStatEnum.physicalDamageTaken || key == Definitions.otherStatEnum.magicDamageTaken) :
			localInverted = true
		for subkey in ModifierPacket.StandardModifier.keys() :
			var tempStr : String = Definitions.otherStatDictionary[key] + getNewTempstr(subkey) + " \t"
			var newVal = modifiers.getMod("otherStat",key,subkey)
			var oldVal = currentMods.getMod("otherStat",key,subkey)
			if (((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0))) && !((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr += "--"
				if (subkey == "Premultiplier") :
					tempStr += getImprovementText(1,(oldVal),localInverted,true)
				else :
					tempStr += getImprovementText(0,oldVal,localInverted,true)
				stringList[0].append(tempStr+"\n")
			elif (((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0)))&&((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr = ""
			elif (!((subkey == "Premultiplier" && is_equal_approx(newVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(newVal,0.0)))&&!((subkey == "Premultiplier" && is_equal_approx(oldVal,1.0)) || (subkey != "Premultiplier" && is_equal_approx(oldVal,0.0)))) :
				tempStr = modifiers.getModStringOrNull_otherStat(key,subkey)
				tempStr += getImprovementText(newVal,oldVal,localInverted,true)
				stringList[2].append(tempStr+"\n")
			else :
				tempStr = modifiers.getModStringOrNull_otherStat(key,subkey)
				tempStr += getImprovementText(newVal,oldVal,localInverted,true)
				stringList[3].append(tempStr+"\n")
			if (tempStr != "") :
				addEndl = true
	for str in stringList[2] :
		myText += str
	for str in stringList[3] :
		myText += str
	for str in stringList[0] :
		myText += str

	if (addEndl) :
		myText += "\n"
	return myText
	
func updateText() :
	if (currentItemSceneRef == null) :
		return
	if (!myTooltips.is_empty()) :
		for child in myTooltips :
			if (child == null || !(child.has_method("queue_free"))) :
				continue
			child.queue_free()
		myTooltips.clear()
	updateText_sprite()
	$VBoxContainer/Text/VBoxContainer/Title.text = currentItemSceneRef.getTitle()
	var myText : String = ""
	myText = EquipmentGroups.getColouredQualityString(currentItemSceneRef.getQuality(), false) + " " + Definitions.equipmentTypeDictionary[currentItemSceneRef.getType()]
	if (isInRewards && currentlyEquippedItem != null) :
		myText += "\t\t[vs Equipped]"
	myText += "\n"
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
		$VBoxContainer/Text/VBoxContainer/Extra.text = ""
	if (isInRewards && currentlyEquippedItem != null) :
		myText = updateText_modifiers_REWARDS(myText)
	else :
		myText = updateText_modifiers(myText)
	if (!isInIGOptions && (currentItemSceneRef.getType() == Definitions.equipmentTypeEnum.weapon || currentItemSceneRef.getType() == Definitions.equipmentTypeEnum.armor)) :
		myText += "Times reforged: " + str(currentItemSceneRef.getReforges()) + "\n\n"
	var description : String = currentItemSceneRef.getDesc()
	var usedDescription
	var iname = currentItemSceneRef.getItemName()
	if (iname == "coating_divine" || iname == "lightning_arrows_1" || iname == "lightning_arrows_2" || iname == "ring_authority") :
		var endPos = description.find("\n\n")
		myText += description.substr(0, endPos)
		usedDescription = description.substr(endPos+1)
	else :
		usedDescription = description
	$VBoxContainer/Text/VBoxContainer/Panel/ScrollContainer/VBoxContainer/Description2.text = usedDescription
	$VBoxContainer/Text/VBoxContainer.visible = true
	$VBoxContainer/Picture/CenterContainer.visible = true
	$VBoxContainer/Text/VBoxContainer/Extra.visible = $VBoxContainer/Text/VBoxContainer/Extra.text != ""
	await $VBoxContainer/Text/VBoxContainer/Panel/ScrollContainer/VBoxContainer/Description.setText(myText)
	if (currentItemSceneRef == null) :
		return
	if (currentItemSceneRef.core is Weapon) :
		addWeaponScalingTooltips()

func resetDetails() :
	$VBoxContainer/Text/VBoxContainer.visible = false
	$VBoxContainer/Picture/CenterContainer.visible = false
	
func setOptionButton(itemSceneRef : Node) :
	return
	#var optionButton = $VBoxContainer/Text/VBoxContainer/HBoxContainer/OptionButton
	#var chosenItem = IGOptions.getIGOptionsCopy()["individualEquipmentTake"].get(itemSceneRef.getItemName())
	#if (chosenItem == null) :
		#optionButton.select(0)
	#else :
		#optionButton.select(chosenItem)

signal individualEquipmentTakeChanged
func _on_option_button_item_selected(index: int) -> void:
	return
	#if (isInIGOptions) :
		#emit_signal("individualEquipmentTakeChanged", currentItemSceneRef, index)
	#else :
		#var optionsCopy = IGOptions.getIGOptionsCopy()
		#optionsCopy["individualEquipmentTake"][currentItemSceneRef.getItemName()] = index
		#IGOptions.saveAndUpdateIGOptions(optionsCopy)

func updateFromOptions(optionDict : Dictionary) :
	return
	#if (currentItemSceneRef == null) :
		#return
	#var currentEquipSetting = optionDict["individualEquipmentTake"].get(currentItemSceneRef.getItemName())
	#if (currentEquipSetting == null) :
		#currentEquipSetting = 0
	#$VBoxContainer/Text/VBoxContainer/HBoxContainer/OptionButton.select(currentEquipSetting)

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
	var myText : RichTextLabel = $VBoxContainer/Text/VBoxContainer/Panel/ScrollContainer/VBoxContainer/Description
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
	var lineNumber = 6
	
	if (currentItemSceneRef == null) :
		return
	for key in Definitions.attributeDictionary.keys() :
		if (key == Definitions.attributeEnum.DUR || key == Definitions.attributeEnum.SKI) :
			continue
		#if (currentItemSceneRef.core.getScaling(key) == 0) :
			#continue
		var lineHeight = myText.get_theme_font("normal_font").get_height(fontSize)
		var yCoord1 = (lineNumber-1) * lineHeight# + lineHeight/4.0
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
		#var offset = myText.global_position-global_position
		#xCoord1 += offset.x
		#xCoord2 += offset.x
		#yCoord1 += offset.y
		#yCoord2 += offset.y
		var desc = $VBoxContainer/Text/VBoxContainer/Panel/ScrollContainer/VBoxContainer/Description
		var newTooltip = tooltipLoader.instantiate()
		desc.add_child(newTooltip)
		newTooltip.currentLayer = currentLayer
		myTooltips.append(newTooltip)
		#newTooltip.setTitle(Definitions.attributeDictionaryShort[key] + " Scaling: " + letterCache[key])
		#newTooltip.setDesc(str(currentItemSceneRef.core.getScaling(key)))
		var tempTitle = Helpers.engineeringRound(currentItemSceneRef.core.getScaling(key),3)
		if (isInRewards && currentlyEquippedItem != null) :
			tempTitle += getImprovementText(currentItemSceneRef.core.getScaling(key),currentlyEquippedItem.getScaling(key), false,false)
		newTooltip.setTitle(tempTitle)
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


func _on_visibility_changed() -> void:
	updateText()
