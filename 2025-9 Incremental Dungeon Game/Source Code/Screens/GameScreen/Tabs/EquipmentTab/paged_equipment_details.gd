extends "res://Screens/GameScreen/Tabs/EquipmentTab/equipment_details.gd"

#var currentPage : Definitions.equipmentTypeEnum = Definitions.equipmentTypeEnum.weapon
signal equipRequested
signal unequipRequested

#func _ready() :
	#for key in Definitions.equipmentTypeDictionary.keys() :
		#currentItemSceneRefs[key] = null
		
func setItemSceneRefBase(itemSceneRef) :
	await super(itemSceneRef)
	if (itemSceneRef == null) :
		return
	var options : OptionButton = $VBoxContainer/Text/VBoxContainer/HBoxContainer/OptionButton
	var myButton = $VBoxContainer/Text/VBoxContainer/HBoxContainer2/CenterContainer2/Discard
	if (itemSceneRef.getType() == Definitions.equipmentTypeEnum.currency) :
		myButton.text = " Not Discardable "
		myButton.set_disabled(true)
		if (options.get_item_count() == 3) :
			options.remove_item(2)
		elif (options.get_item_count() == 2) :
			pass
		else :
			return
	else :
		myButton.text = " Discard "
		myButton.set_disabled(false)
		if (options.get_item_count() == 3) :
			pass
		elif (options.get_item_count() == 2) :
			options.add_item("Always Discard", 2)
		else :
			return
			
		
func _process(_delta) : 
	if (currentItemSceneRef != null) :
		var myButton = $VBoxContainer/Text/VBoxContainer/HBoxContainer2/CenterContainer/Button
		if (!Definitions.isEquippable(currentItemSceneRef)) :
			myButton.text = " Not equippable "
			myButton.set_disabled(true) 
		elif (currentItemSceneRef.isEquipped()) :
			myButton.text = " Unequip "
			myButton.set_disabled(false)
		else :
			myButton.text = " Equip "
			myButton.set_disabled(false)
	
#func switchPage(newPage : Definitions.equipmentTypeEnum) :
	#currentPage = newPage
	#var item = currentItemSceneRefs.get(newPage)
	#setItemSceneRef(item, newPage)

func _on_button_pressed() -> void:
	if (!currentItemSceneRef.isEquipped()) :
		emit_signal("equipRequested", currentItemSceneRef)
	else :
		emit_signal("unequipRequested", currentItemSceneRef)

const binaryPopupLoader = preload("res://Graphic Elements/popups/binary_decision.tscn")
func _on_discard_pressed() -> void:
	var popup = binaryPopupLoader.instantiate()
	add_child(popup)
	popup.setTitle("Discard an item?")
	popup.setText("Are you sure you want to permanently discard this item?")
	popup.connect("binaryChosen", _on_discard_item_resolved)
	
signal discardItemRequested
func _on_discard_item_resolved(option : int) :
	if (option == 0) :
		emit_signal("discardItemRequested", currentItemSceneRef)
	
