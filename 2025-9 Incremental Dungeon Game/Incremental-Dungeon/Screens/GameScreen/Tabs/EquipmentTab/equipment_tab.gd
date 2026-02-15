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
		itemSceneRef.core.getItemName()
		itemSceneRef.core.basicAttack.getResourceName()
		var temp = itemSceneRef.core.duplicate()
		return temp
func getCurrentArmor() -> Armor :
	var itemSceneRef = $Inventory.getEquippedItem(Definitions.equipmentTypeEnum.armor)
	if (itemSceneRef == null) :
		return null
	else :
		itemSceneRef.core.getItemName()
		var temp = itemSceneRef.core.duplicate()
		#temp.resourceName = itemSceneRef.core.resourceName
		return temp
func getCurrentAccessory() -> Accessory :
	var itemSceneRef = $Inventory.getEquippedItem(Definitions.equipmentTypeEnum.accessory)
	if (itemSceneRef == null) :
		return null
	else :
		itemSceneRef.core.getItemName()
		var temp = itemSceneRef.core.duplicate()
		#temp.resourceName = itemSceneRef.core.resourceName
		return temp
func getEquippedItem(type) :
	return $Inventory.getEquippedItem(type)
func getDirectModifiers() -> ModifierPacket :
	return $Inventory.getModifierPacket()
func getItemCount(item : Equipment) :
	return $Inventory.getItemCount(item)
func getElementalDirectModifiers(subclass) -> ModifierPacket :
	return $Inventory.getElementalModifierPacket(subclass)
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
	#AudioHandler.playMenuSfx(AudioHandler.menuSfx.select)
func _on_unequip_requested_from_details(itemSceneRef) -> void:
	$Inventory.unequipItem(itemSceneRef.getType())
func _on_current_equips_select_requested(itemSceneRef, type : Definitions.equipmentTypeEnum) -> void:
	$PagedEquipmentDetails.setItemSceneRefBase(itemSceneRef)
	#$Inventory.sortByType(type)
	if (itemSceneRef != null) :
		$Inventory.selectItem(itemSceneRef)
func _on_discard_requested_from_details(itemSceneRef) -> void:
	$Inventory.discardItem(itemSceneRef)
	#AudioHandler.playMenuSfx(AudioHandler.menuSfx.cancel)
signal unequippedItem
func _on_inventory_unequipped_item(itemSceneRef) -> void :
	$CurrentEquips.setItemSceneRef(null, itemSceneRef.getType())
	emit_signal("unequippedItem", itemSceneRef)
	#$PagedEquipmentDetails.setItemSceneRef(null, itemSceneRef.getType())
signal equippedItem
func _on_inventory_equipped_item(itemSceneRef) -> void:
	$CurrentEquips.setItemSceneRef(itemSceneRef, itemSceneRef.getType())
	emit_signal("equippedItem", itemSceneRef)
	#$PagedEquipmentDetails.setItemSceneRef(itemSceneRef, itemSceneRef.getType())
func _on_item_deselected(_itemSceneRef) -> void:
	$PagedEquipmentDetails.setItemSceneRefBase(null)

#####################################

signal isReforgeHoveredRequested
func _on_inventory_is_reforge_hovered_requested(emitter) -> void:
	emit_signal("isReforgeHoveredRequested", emitter)
func reforge(type, newScalingVal, rows) :
	return $Inventory.reforge(type, newScalingVal, rows)
	
	
func hasDirectUpgrade(oldItem : Equipment) -> bool:
	for item in $Inventory.getInventoryList() :
		if (item.get_child_count() != 1) :
			continue
		if (Helpers.equipmentIsDirectUpgrade(item.get_child(0).core,oldItem)) :
			#print("Deleting (probably) a combat reward: " + oldItem.getName() + " because you already have " + item.get_child(0).core.getName())
			return true
	return false

func replaceDirectDowngrades(newItem : Equipment, replaceInInventory : bool, replaceInEquipped : bool, replaceLegendaries : bool, replaceSignatures : bool) -> bool :
	var replaced : bool = false
	var equippedDiscarded : bool = false
	for item in $Inventory.getInventoryList() :
		if (item.get_child_count() != 1) :
			continue
		var groups : EquipmentGroups = item.get_child(0).core.equipmentGroups
		if ((groups.quality == EquipmentGroups.qualityEnum.legendary&&!replaceLegendaries) || (groups.isSignature && !replaceSignatures)) :
			continue
		if (Helpers.equipmentIsDirectUpgrade(newItem, item.get_child(0).core)) :
			var isEquipped = $Inventory.getEquippedItem(item.get_child(0).getType()) == item.get_child(0)
			if (isEquipped && replaceInEquipped || !isEquipped && replaceInInventory) :
				#if (item.get_child(0) == $Inventory.selectedEntry) :
					#$PagedEquipmentDetails.setItemSceneRefBase(null)
				var equippedString
				if (isEquipped) :
					equippedString = " (equipped) "
				else :
					equippedString = ""
				#print("replaced " + item.get_child(0).core.getName() + equippedString + " with " + newItem.getName())
				$Inventory.discardItem(item.get_child(0))
				replaced = true
				if (isEquipped) :
					equippedDiscarded = true
	var itemSceneRef = null
	if (replaced) :
		itemSceneRef = SceneLoader.createEquipmentScene(newItem.getItemName())
		itemSceneRef.core = newItem
		$Inventory.addToInventory(itemSceneRef)
	if (equippedDiscarded && itemSceneRef != null) :
		$Inventory.equipItem(itemSceneRef)
	return replaced


func _on_paged_equipment_details_currently_equipped_item_requested(emitter, type) -> void:
	var item = getEquippedItem(type)
	if (item == null) :
		emitter.provideCurrentlyEquippedItem(null)
	else :
		emitter.provideCurrentlyEquippedItem(item.core)
