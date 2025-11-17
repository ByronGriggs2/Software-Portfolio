extends Panel

var currentItemSceneRef = null

func _ready() :
	resetDetails()
	
func setItemSceneRefBase(itemSceneRef) :
	currentItemSceneRef = itemSceneRef
	if (currentItemSceneRef == null) :
		resetDetails()
	
func _process(_delta) :
	if (currentItemSceneRef == null) :
		return
	var mySprite = $Picture/HBoxContainer/Sprite
	var newSprite = currentItemSceneRef.getSprite()
	mySprite.setTexture(newSprite.getTexture())
	mySprite.setRegionRect(newSprite.getRegionRect())
	var newScale = 3*newSprite.getScale()
	mySprite.setScale(newScale)
	mySprite.updateSize()
	$Text/VBoxContainer/Title.text = currentItemSceneRef.getTitle()
	var myText = $Text/VBoxContainer/Panel/Description
	if (currentItemSceneRef.core is Weapon) :
		myText.text = "Primary attribute:  " + Definitions.attributeDictionary[currentItemSceneRef.getPrimaryAttribute()] + "\n"
		myText.text = myText.text + "Attack bonus:  " + str(currentItemSceneRef.getAttack()) + "\n"
		myText.text = myText.text + "\n"
	elif (currentItemSceneRef.core is Armor) :
		myText.text = "Physical defense:  " + str(currentItemSceneRef.getPhysicalDefense()) + "\n"
		myText.text = myText.text + "Magic defense:  " + str(currentItemSceneRef.getMagicDefense()) + "\n"
		myText.text = myText.text + "\n"
	elif (currentItemSceneRef.core is Accessory) :
		myText.text = ""	
		
	var modifiers : ModifierPacket = currentItemSceneRef.core.getModifierPacket()
	for key in Definitions.attributeDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var modString = modifiers.getModStringOrNull_attr(key, subkey)
			if (modString != null) :
				myText += modString + "\n"
	for key in Definitions.baseStatDictionary.keys() :
		for subkey in ModifierPacket.StandardModifier.keys() :
			var modString = modifiers.getModStringOrNull_combatStat(key,subkey)
			if (modString != null) :
				myText += modString + "\n"
	myText.text = myText.text + currentItemSceneRef.getDesc()
	$Text/VBoxContainer.visible = true
	$Picture/HBoxContainer.visible = true

func resetDetails() :
	$Text/VBoxContainer.visible = false
	$Picture/HBoxContainer.visible = false
