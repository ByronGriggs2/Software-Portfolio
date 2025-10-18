extends "res://Screens/GameScreen/Tabs/EquipmentTab/equipment_details.gd"

var selectedItems : Dictionary = {}
var currentPage : Definitions.equipmentTypeEnum = Definitions.equipmentTypeEnum.weapon
signal equipRequested
signal unequipRequested

func _ready() :
	for key in Definitions.equipmentTypeDictionary.keys() :
		selectedItems[key] = null
		
func _process(_delta) : 
	if (selectedItems[currentPage] != null) :
		if (selectedItems[currentPage].isEquipped()) :
			$Text/VBoxContainer/HBoxContainer/CenterContainer/Button.text = "Unequip"
		else :
			$Text/VBoxContainer/HBoxContainer/CenterContainer/Button.text = "Equip"
		super(_delta)
	
func switchPage(newPage : Definitions.equipmentTypeEnum) :
	currentPage = newPage
	var item = selectedItems.get(newPage)
	setItemSceneRef(item, newPage)

func _on_button_pressed() -> void:
	if (!selectedItems[currentPage].isEquipped()) :
		emit_signal("equipRequested", selectedItems[currentPage])
	else :
		emit_signal("unequipRequested", selectedItems[currentPage])

func setItemSceneRef(itemSceneRef, type : Definitions.equipmentTypeEnum) :
	selectedItems[type] = itemSceneRef
	currentPage = type
	setItemSceneRefBase(itemSceneRef)

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
		emit_signal("discardItemRequested", selectedItems[currentPage])
	
