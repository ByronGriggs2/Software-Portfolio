extends Panel

# Because a node in Godot cannot be displayed in two locations on the screen at once, item scenes will be children of 
# Inventory, with references to those scenes given to CurrentEquips and PagedEquipmentDetails.
# These references are implemented as readonly, so if they want to change it, they have to ask EquipmentTab to ask Inventory 
# to change it, which effectively decouples $CurrentEquips and $PagedEquipmentDetails from Inventory

func getContainer(type : Definitions.equipmentTypeEnum) :
	return $Inventory/InnerContainer.get_child(type)
	
#func isInInventory(item) :
#	return getContainer(item.getType()).itemIsPresent(item)
	
func getStatBonus() :
	var derivedStats : Dictionary
	for key in Definitions.baseStatDictionary.keys() :
		derivedStats[key] = 0
	for key in Definitions.equipmentTypeDictionary.keys() :
		getContainer(key).addStatBonus(derivedStats)
	return derivedStats
	
func getAttributeBonus() :
	var attributes : Dictionary
	for key in Definitions.attributeDictionary.keys() :
		attributes[key] = 0
	for key in Definitions.equipmentTypeDictionary.keys() :
		getContainer(key).addAttributeBonus(attributes)
	return attributes
	
# To be expanded?
func getCurrentWeapon() :
	return getContainer(Definitions.equipmentTypeEnum.weapon).getEquippedItem()
func getCurrentArmor() :
	return getContainer(Definitions.equipmentTypeEnum.armor).getEquippedItem()

func _on_item_selected_in_inventory(itemSceneRef) -> void:
	$PagedEquipmentDetails.setItemSceneRef(itemSceneRef, itemSceneRef.getType())

func _on_inventory_tab_changed(tab: int) -> void:
	$PagedEquipmentDetails.switchPage(tab)

func _on_equip_requested_from_details(itemSceneRef) -> void:
	equipItemFromDetails(itemSceneRef)

func _on_unequip_requested_from_details(itemSceneRef) -> void:
	unequipItemFromDetails(itemSceneRef)
	
func _on_current_equips_select_requested(itemSceneRef, type : Definitions.equipmentTypeEnum) -> void:
	$PagedEquipmentDetails.setItemSceneRef(itemSceneRef, type)
	$Inventory.setTab(getContainer(type))
	if (itemSceneRef != null) :
		getContainer(type).selectItem(itemSceneRef)

func _on_discard_requested_from_details(itemSceneRef) -> void:
	var type = itemSceneRef.getType()
	if (itemSceneRef.isEquipped()) :
		unequipItemFromDetails(itemSceneRef)
	getContainer(itemSceneRef.getType()).discardItem(itemSceneRef)
	$PagedEquipmentDetails.setItemSceneRef(null, type)
	
func isInventoryFull(type : Definitions.equipmentTypeEnum) -> bool :
	return getContainer(type).isInventoryFull()

func addItemToInventory(itemSceneRef) :
	getContainer(itemSceneRef.getType()).addToInventory(itemSceneRef)
	
func unequipItemFromDetails(itemSceneRef) :
	if (itemSceneRef == null) :
		return
	if (itemSceneRef.getType() == Definitions.equipmentTypeEnum.weapon && itemSceneRef.isUnarmed()) :
		return
	getContainer(itemSceneRef.getType()).unequipItem(itemSceneRef)
	$CurrentEquips.setItemSceneRef(getContainer(itemSceneRef.getType()).getEquippedItem(), itemSceneRef.getType())
	$PagedEquipmentDetails.setItemSceneRef(itemSceneRef, itemSceneRef.getType())
	
func equipItemFromDetails(itemSceneRef) :
	unequipItemFromDetails(getContainer(itemSceneRef.getType()).getEquippedItem())
	getContainer(itemSceneRef.getType()).equipItem(itemSceneRef)
	$CurrentEquips.setItemSceneRef(itemSceneRef, itemSceneRef.getType())
	$PagedEquipmentDetails.setItemSceneRef(itemSceneRef, itemSceneRef.getType())

func _on_equipment_loaded_from_disk(itemSceneRef, emitter) -> void:
	var type
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (Definitions.equipmentTypeDictionary[key] == emitter.name) :
			type = key
	$CurrentEquips.setItemSceneRef(itemSceneRef, type)
