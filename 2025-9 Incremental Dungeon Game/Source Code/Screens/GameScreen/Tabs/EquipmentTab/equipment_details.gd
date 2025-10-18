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
	for key in currentItemSceneRef.core.attributeBonus.keys() :
		if (currentItemSceneRef.core.attributeBonus[key] != 0) :
			if (currentItemSceneRef.core.attributeBonus[key] > 0) :
				myText.text = myText.text + "+"
			myText.text = myText.text + str(currentItemSceneRef.core.attributeBonus[key]) + " " + key + "\n"
	for key in currentItemSceneRef.core.statBonus.keys() :
		if (currentItemSceneRef.core.statBonus[key] != 0) :
			if (currentItemSceneRef.core.statBonus[key] > 0) :
				myText.text = myText.text + "+"
			myText.text = myText.text + str(currentItemSceneRef.core.statBonus[key]) + " " + key + "\n"
	myText.text = myText.text + currentItemSceneRef.getDesc()
	$Text/VBoxContainer.visible = true
	$Picture/HBoxContainer.visible = true

func resetDetails() :
	$Text/VBoxContainer.visible = false
	$Picture/HBoxContainer.visible = false
