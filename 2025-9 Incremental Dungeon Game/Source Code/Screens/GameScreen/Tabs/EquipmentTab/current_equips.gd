extends Panel

var currentEquipment : Dictionary
var superButtons : Dictionary

func _ready() :
	createEquipmentSlots()
	for key in Definitions.equippableDictionary.keys() :
		currentEquipment[key] = null
	
func _process(_delta) :
	updateSprites()
	
func updateSprites() :
	for key in Definitions.equippableDictionary.keys() :
		if (currentEquipment[key] == null) :
			getSprite(key).setTexture(null)
			getText(key).text = " "
		else :
			var mySprite = getSprite(key)
			var actualSprite = currentEquipment[key].getSprite()
			mySprite.setTexture(actualSprite.getTexture())
			mySprite.setRegionRect(actualSprite.getRegionRect()) 
			mySprite.setScale(actualSprite.getScale() * 4.0/5.0)
			mySprite.setIs32(actualSprite.isIs32())
			getText(key).text = currentEquipment[key].getTitle()
		
func getSprite(type : Definitions.equipmentTypeEnum) :
	return $HBoxContainer.get_child(type).get_node("Panel").get_node("PassiveSuperButton").get_node("HBoxContainer").get_node("Sprite")
func getText(type : Definitions.equipmentTypeEnum) :
	return $HBoxContainer.get_child(type).get_node("RichTextLabel2")
#func getTitle(type : Definitions.equipmentTypeEnum) :
	#return $HBoxContainer.get_child(type).get_node("Title")
	
func setItemSceneRef(itemSceneRef, type : Definitions.equipmentTypeEnum) :
	currentEquipment[type] = itemSceneRef
	updateSprites()
		
func createEquipmentSlots() :
	for key in Definitions.equippableDictionary.keys() :
		if (key == Definitions.equipmentTypeEnum.weapon) :
			superButtons[key] = $HBoxContainer/Weapon/Panel/PassiveSuperButton
		else :
			var newSlot = $HBoxContainer/Weapon.duplicate()
			$HBoxContainer.add_child(newSlot)
			newSlot.name = Definitions.equippableDictionary[key]
			newSlot.get_node("Title").text = Definitions.equippableDictionary[key]
			superButtons[key] = newSlot.get_node("Panel").get_node("PassiveSuperButton")
		
signal selectRequested
func _on_equipment_selected(emitter) :
	for key in superButtons.keys() :
		if (superButtons[key] == emitter) :
			emit_signal("selectRequested", currentEquipment.get(key), key)
			return
