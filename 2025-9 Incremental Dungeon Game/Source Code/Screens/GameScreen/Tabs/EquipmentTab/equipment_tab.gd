extends Panel

# Because a node in Godot cannot be displayed in two locations on the screen at once, item scenes will be children of 
# Inventory, with references to those scenes given to CurrentEquips and PagedEquipmentDetails.
# These references are implemented as readonly, so if they want to change it, they have to ask EquipmentTab to ask Inventory 
# to change it, which effectively decouples $CurrentEquips and $PagedEquipmentDetails from Inventory

######################################
##Internal
func isInventoryFull() -> bool :
	return $Inventory.isInventoryFull()
#####################################
##Getters
func getCurrentWeapon() -> Weapon :
	var itemSceneRef = $Inventory.getEquippedItem(Definitions.equipmentTypeEnum.weapon)
	if (itemSceneRef == null) :
		return null
	else :
		return itemSceneRef.core.duplicate()
func getCurrentArmor() -> Armor :
	var itemSceneRef = $Inventory.getEquippedItem(Definitions.equipmentTypeEnum.armor)
	if (itemSceneRef == null) :
		return null
	else :
		return itemSceneRef.core.duplicate()
func getDirectModifiers() -> ModifierPacket :
	return $Inventory.getModifierPacket()
func getItemCount(item : Equipment) :
	return $Inventory.getItemCount(item)
#####################################
##Setters
func addItemToInventory(itemSceneRef) :
	$Inventory.addToInventory(itemSceneRef)
func equipExternalItem(itemSceneRef) :
	$Inventory.unequipItem(itemSceneRef.getType())
	$PagedEquipmentDetails.setItemSceneRef(itemSceneRef)
	$CurrentEquips.setItemSceneRef(itemSceneRef)
func setItemCount(item : Equipment, val : int) :
	$Inventory.setItemCount(item, val)
func expandInventory(increase : int) :
	$Inventory.expand(increase)
#####################################
##Signals
func _on_item_selected(itemSceneRef) -> void:
	$PagedEquipmentDetails.setItemSceneRefBase(itemSceneRef)
#func _on_inventory_tab_changed(tab: int) -> void:
	#$PagedEquipmentDetails.switchPage(tab)
func _on_equip_requested_from_details(itemSceneRef) -> void:
	$Inventory.equipItem(itemSceneRef)
func _on_unequip_requested_from_details(itemSceneRef) -> void:
	$Inventory.unequipItem(itemSceneRef.getType())
func _on_current_equips_select_requested(itemSceneRef, type : Definitions.equipmentTypeEnum) -> void:
	$PagedEquipmentDetails.setItemSceneRefBase(itemSceneRef)
	$Inventory.sortByType(type)
	if (itemSceneRef != null) :
		$Inventory.selectItem(itemSceneRef)
func _on_discard_requested_from_details(itemSceneRef) -> void:
	$Inventory.discardItem(itemSceneRef)
func _on_inventory_unequipped_item(itemSceneRef) -> void :
	$CurrentEquips.setItemSceneRef(null, itemSceneRef.getType())
	#$PagedEquipmentDetails.setItemSceneRef(null, itemSceneRef.getType())
func _on_inventory_equipped_item(itemSceneRef) -> void:
	$CurrentEquips.setItemSceneRef(itemSceneRef, itemSceneRef.getType())
	#$PagedEquipmentDetails.setItemSceneRef(itemSceneRef, itemSceneRef.getType())
func _on_item_deselected(_itemSceneRef) -> void:
	$PagedEquipmentDetails.setItemSceneRefBase(null)

#####################################
