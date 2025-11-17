extends "res://Screens/GameScreen/Tabs/EquipmentTab/equipment_details.gd"

#var currentPage : Definitions.equipmentTypeEnum = Definitions.equipmentTypeEnum.weapon
signal equipRequested
signal unequipRequested

#func _ready() :
	#for key in Definitions.equipmentTypeDictionary.keys() :
		#currentItemSceneRefs[key] = null
		
func _process(_delta) : 
	if (currentItemSceneRef != null) :
		if (currentItemSceneRef.isEquipped()) :
			$Text/VBoxContainer/HBoxContainer/CenterContainer/Button.text = " Unequip "
		else :
			$Text/VBoxContainer/HBoxContainer/CenterContainer/Button.text = " Equip "
		super(_delta)
	
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
	
