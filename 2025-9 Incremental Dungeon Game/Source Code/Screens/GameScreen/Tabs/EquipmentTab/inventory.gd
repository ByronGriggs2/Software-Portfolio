extends Panel

## Currency is no longer handled by this class. It's handled by GameScreen.

var selectedEntry : Node = null
var equippedEntries : Array[Node] = []
@export var inventorySize : int = 8

#################################
##Setters
func addToInventory(itemSceneRef) :
	if (itemSceneRef.core is Currency) :
		return
	getNextEmptySlot().add_child(itemSceneRef)
	itemSceneRef.setMouseFilter(Control.MOUSE_FILTER_PASS)
	itemSceneRef.get_node("SuperButton").contentMargin = 6
	itemSceneRef.get_node("SuperButton").updatePanel()
	itemSceneRef.connect("wasSelected", _on_item_selected)
	itemSceneRef.connect("wasDeselected", _on_item_deselected)
	
func discardItem(itemSceneRef) :
	var itemIndex = findPanelIndex(itemSceneRef)
	getInventoryList()[itemIndex].remove_child(itemSceneRef)
	if (itemIndex == null) :
		return
	if (itemSceneRef.isEquipped()) :
		unequipItem(itemSceneRef.getType())
	if (itemIndex != inventorySize-1 && getInventory().get_child(itemIndex+1).get_child_count() == 1) :
		itemIndex += 1
	elif (itemIndex != 0 && getInventory().get_child(itemIndex-1).get_child_count() == 1) :
		itemIndex -= 1
	else :
		itemIndex = -1
	emit_signal("itemDeselected", itemSceneRef)
	if (itemIndex != -1) :
		var newSelection : Node = getInventory().get_child(itemIndex).get_child(0)
		selectItem(newSelection)
		emit_signal("itemSelected", newSelection)
	itemSceneRef.queue_free()
	
	
func setItemCount(item : Equipment, val : int) :
	if (item is Currency) :
		return
	var currentAmount = getItemCount(item)
	for child in getInventoryList() :
		if (currentAmount > val && child.get_child_count() == 1&&child.get_child(0).core == item) :
			discardItem(child.get_child(0))
			currentAmount -= 1
		if (currentAmount <= val) :
			break
const draggablePanelLoader = preload("res://draggable_panel.tscn")
func expand(increase : int) :
	inventorySize += increase
	for index in range(0,increase) :
		var newPanel = draggablePanelLoader.instantiate()
		$ScrollContainer/CenterContainer/GridContainer.add_child(newPanel)
		newPanel.connect("dragStart", _on_draggable_drag)
		newPanel.connect("dragStop", _on_draggable_release)
#################################
##Getters
func getEquippedItem(type : Definitions.equipmentTypeEnum) :
	return equippedEntries[type]
func isInventoryFull() -> bool : 
	return (getNextEmptySlot() == null)
func getModifierPacket() -> ModifierPacket :
	#var retVal = ModifierPacket.new()
	var modPacketRefs : Array[ModifierPacket] = []
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (equippedEntries[key] != null) :
			modPacketRefs.append(equippedEntries[key].getModPacketRef())
	var retVal = ModifierPacket.add_array(modPacketRefs)
	if (equippedEntries[2] != null && equippedEntries[2].getItemName() == "coating_divine" && equippedEntries[0] != null && equippedEntries[0].core.equipmentGroups.weaponClass != EquipmentGroups.weaponClassEnum.ranged) :
		retVal.statMods[Definitions.baseStatDictionary[Definitions.baseStatEnum.DR]]["Premultiplier"] *= 1.11
		retVal.otherMods[Definitions.otherStatDictionary[Definitions.otherStatEnum.physicalConversion]]["Prebonus"] += 0.11
	elif (equippedEntries[2] != null && equippedEntries[2].getItemName() == "lightning_arrows_1" && equippedEntries[0] != null && equippedEntries[0].core.equipmentGroups.weaponClass != EquipmentGroups.weaponClassEnum.melee) :
		retVal.statMods[Definitions.baseStatDictionary[Definitions.baseStatEnum.DR]]["Premultiplier"] *= 1.11
		retVal.otherMods[Definitions.otherStatDictionary[Definitions.otherStatEnum.physicalConversion]]["Prebonus"] += 0.11
	elif (equippedEntries[2] != null && equippedEntries[2].getItemName() == "lightning_arrows_2" && equippedEntries[0] != null && equippedEntries[0].core.equipmentGroups.weaponClass != EquipmentGroups.weaponClassEnum.melee) :
		retVal.statMods[Definitions.baseStatDictionary[Definitions.baseStatEnum.DR]]["Premultiplier"] *= 1.3
		retVal.otherMods[Definitions.otherStatDictionary[Definitions.otherStatEnum.physicalConversion]]["Prebonus"] += 0.3
	return retVal
func getElementalModifierPacket(subclass) -> ModifierPacket :
	var mult
	if (subclass == Definitions.subclass.enchant) :
		mult = 1.2625
	else :
		mult = 1.25
	var retVal = ModifierPacket.new()
	var weaponMatches
	if (equippedEntries[0] == null || equippedEntries[2] == null) :
		weaponMatches = 0
	else :
		weaponMatches = EquipmentGroups.getMatchingElementCount(equippedEntries[0].core.equipmentGroups, equippedEntries[2].core.equipmentGroups)
		if (equippedEntries[1] != null && equippedEntries[1].getItemName() == "armor_cephalopod" && equippedEntries[2] != null && equippedEntries[2].core.equipmentGroups.isWater && equippedEntries[0] != null && !equippedEntries[0].core.equipmentGroups.isWater) :
			weaponMatches += 1
	retVal.statMods[Definitions.baseStatDictionary[Definitions.baseStatEnum.DR]]["Premultiplier"] = pow(mult, weaponMatches)
	var armorMatches
	if (equippedEntries[1] == null || equippedEntries[2] == null) :
		armorMatches = 0
	else :
		armorMatches = EquipmentGroups.getMatchingElementCount(equippedEntries[1].core.equipmentGroups, equippedEntries[2].core.equipmentGroups)
	retVal.statMods[Definitions.baseStatDictionary[Definitions.baseStatEnum.PHYSDEF]]["Premultiplier"] = pow(mult,armorMatches)
	retVal.statMods[Definitions.baseStatDictionary[Definitions.baseStatEnum.MAGDEF]]["Premultiplier"] = pow(mult, armorMatches)
	return retVal
	
func getItemCount(item : Equipment) :
	if (item is Currency) :
		return -1
	var count : int = 0
	for myItem in getInventoryList() :
		if (myItem.get_child_count()==1 && myItem.get_child(0).core == item) :
			count += 1
	return count
################################
##Other
func selectItem(itemSceneRef) :
	itemSceneRef.select()
	selectedEntry = itemSceneRef
	for child in getInventoryList() :
		if (child.get_child_count() == 1 && child.get_child(0) != itemSceneRef) :
			child.get_child(0).deselect()

signal itemEquipped
func equipItem(itemSceneRef) :
	if (!Definitions.isEquippable(itemSceneRef)) :
		return
	if (equippedEntries[itemSceneRef.getType()] != null) :
		unequipItem(itemSceneRef.getType())
	equippedEntries[itemSceneRef.getType()] = itemSceneRef
	itemSceneRef.equip()
	emit_signal("itemEquipped", itemSceneRef)
	
signal itemUnequipped
func unequipItem(type : Definitions.equipmentTypeEnum) :
	var entry = equippedEntries[type]
	equippedEntries[type].unequip()
	equippedEntries[type] = null
	emit_signal("itemUnequipped", entry)
	
func sortBasic() :
	resetVisibility()
	var basicSortSwap : Callable = func(a,b) : return a.get_child_count() == 0 && b.get_child_count() == 1
	Helpers.myOwnGoddamnSort_sortChildren(getInventory(), basicSortSwap)
		
func resetVisibility() :
	for item in getInventoryList() :
		if (item.get_child_count() == 1) :
			item.get_child(0).visible = true
			
func sortAlphabetical() :
	sortBasic()
	var alphabeticalSwap : Callable = func(a,b) : 
		if (a.get_child_count() == 0 || b.get_child_count() == 0) :
			return false
		return b.get_child(0).getTitle() < a.get_child(0).getTitle()
	Helpers.myOwnGoddamnSort_sortChildren(getInventory(), alphabeticalSwap)

func sortByType(type : Definitions.equipmentTypeEnum) :
	sortAlphabetical()
	var typeSwap : Callable = func(a,b) : 
		if (a.get_child_count() == 0 || b.get_child_count() == 0) :
			return false
		return b.get_child(0).getType() == type && a.get_child(0).getType() != type
	Helpers.myOwnGoddamnSort_sortChildren(getInventory(), typeSwap)
		
	#for item in getInventoryList() :
		#if (item.get_child_count() == 1 && item.get_child(0).getType() != type) :
			#item.get_child(0).visible = false
			#if (item == selectedEntry) :
				#item.get_child(0).deselect()
	
func getInventoryList() -> Array[Node] : 
	var ret = $ScrollContainer/CenterContainer/GridContainer.get_children()
	var pos = ret.find(draggingPanel)
	if (pos != -1) : 
		ret.remove_at(pos)
	return ret
func getInventory() -> Node :
	return $ScrollContainer/CenterContainer/GridContainer
##################################
##Internal
func reforge(type, newScalingVal, rows) -> bool :
	if (equippedEntries[type] == null) :
		return false
	equippedEntries[type].reforge(newScalingVal, rows)
	return true
	
func findPanel(item) :
	var equipment
	if (item is Equipment) :
		equipment = item
	else :
		equipment = item.core
	for child in getInventoryList() :
		if (child.get_child_count() == 1 && child.get_child(0).core == equipment) :
			return child
	return null
func findPanelIndex(item) :
	var equipment
	if (item is Equipment) :
		equipment = item
	else :
		equipment = item.core
	var children = getInventoryList()
	for index in range(0,children.size()) :
		if (children[index].get_child_count() == 1 && children[index].get_child(0).core == equipment) :
			return index
	return null	
func getNextEmptySlot() :
	var children = getInventoryList()
	if (children.is_empty()) :
		return null
	for child in children :
		if (child.get_child_count() == 0) :
			return child
	return null
func findEquipment(item : Equipment) :
	for child in getInventoryList() :
		if (child.get_child_count()==1 && child.get_child(0).core == item) :
			return child.get_child(0)
	return null
####################################
##Signals
signal itemSelected
func _on_item_selected(itemSceneRef) :
	selectItem(itemSceneRef)
	emit_signal("itemSelected", itemSceneRef)
signal itemDeselected
func _on_item_deselected(itemSceneRef) :
	emit_signal("itemDeselected", itemSceneRef)
func _on_abc_pressed(_emitter) -> void:
	sortAlphabetical()
func _on_tab_button_pressed(emitter) :
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (emitter.name == Definitions.equipmentTypeDictionary[key]) :
			sortByType(key)
			return
####################################
##Initialisation
func initialiseContainers() :
	for index in range(0,inventorySize-1) :
		var newPanel = $ScrollContainer/CenterContainer/GridContainer/Panel.duplicate()
		getInventory().add_child(newPanel)
		newPanel.connect("dragStart", _on_draggable_drag)
		newPanel.connect("dragStop", _on_draggable_release)
	getInventory().get_child(0).connect("dragStart", _on_draggable_drag)
	getInventory().get_child(0).connect("dragStop", _on_draggable_release)
	
#var oldPosPanel : Node = null
var draggingPanel : Node = null
var ghostPanel : Node = null
func _on_draggable_drag(emitter) :
	draggingPanel = emitter
	ghostPanel = draggablePanelLoader.instantiate()
	add_child(ghostPanel)
	draggingPanel.cancelDrag()
	ghostPanel.startDragging()
	ghostPanel.connect("dragStop", _on_draggable_release)
	if (draggingPanel.get_child_count() > 0) :
		draggingPanel.get_child(0).visible = false
		ghostPanel.add_child(draggingPanel.get_child(0).createGhostCopy())
	#await get_tree().process_frame
	#var oldIndex = Helpers.findIndexInContainer(getInventory(), emitter)
	#var oldPos = emitter.global_position
	#getInventory().remove_child(emitter)
	#add_child(emitter)
	#emitter.global_position = oldPos
	#oldPosPanel = draggablePanelLoader.instantiate()
	#getInventory().add_child(oldPosPanel)
	#getInventory().move_child(oldPosPanel, oldIndex)
	
func _on_draggable_release(_emitter) :
	## Inventory actually does not need to know if reforge is hovered. The player drags the item to the reforge window
	#var reforgeHovered = await getReforgeHovered()
	var oldIndex = Helpers.findIndexInContainer(getInventory(), draggingPanel)
	var newPos = ghostPanel.global_position + ghostPanel.size/2.0
	var swapTarget : Node = null
	for child in getInventoryList() :
		var childPos = child.global_position
		if (childPos.x <= newPos.x && childPos.x + child.size.x >= newPos.x && childPos.y <= newPos.y && childPos.y + child.size.y >= newPos.y) :
			swapTarget = child
			break
	if (swapTarget == draggingPanel) :
		swapTarget = null
	var newIndex
	if (swapTarget != null) :
		newIndex = Helpers.findIndexInContainer(getInventory(), swapTarget)
	remove_child(ghostPanel)
	ghostPanel.queue_free()
	ghostPanel = null
	if (swapTarget == null) :
		getInventory().move_child(draggingPanel, oldIndex)
	else :
		if (oldIndex < newIndex) :
			getInventory().move_child(swapTarget, oldIndex)
			getInventory().move_child(draggingPanel, newIndex)
		else :
			getInventory().move_child(draggingPanel, newIndex)
			getInventory().move_child(swapTarget, oldIndex)
	if (draggingPanel.get_child_count() > 0) :
		draggingPanel.get_child(0).visible = true
	draggingPanel = null
	
var waitingForReforgeHovered : bool = false
var reforgeHovered_comm : bool = false
signal isReforgeHoveredRequested
signal isReforgeHoveredReceived
func getReforgeHovered() :
	waitingForReforgeHovered = true
	emit_signal("isReforgeHoveredRequested", self)
	if (waitingForReforgeHovered) :
		await isReforgeHoveredReceived
	return reforgeHovered_comm
func provideIsReforgedHovered(val) :
	reforgeHovered_comm = val
	waitingForReforgeHovered = false
	emit_signal("isReforgeHoveredReceived")
	
	#getInventory().remove_child(ghostPanel)
	#ghostPanel.queue_free()
	#ghostPanel = null
	#remove_child(draggingPanel)
	#getInventory().add_child(draggingPanel)
	#getInventory().move_child(draggingPanel, draggingPanelIndex)
	#draggingPanel = null
	#draggingPanelIndex = -1
	#clearDraggingSiblings()

#var draggingSiblings : Array = []
#func updateDraggingSiblings() :
	#draggingSiblings.clear()
	#var left = null
	#if (draggingPanelIndex >= 1 && (draggingPanelIndex)%6 != 0) :
		#left = getInventory().get_child(draggingPanelIndex-1)
	#draggingSiblings.append(left)
	#var right = null
	#if (draggingPanelIndex <= getInventory().get_child_count() - 2 && (draggingPanelIndex+1)%6 != 0) :
		#right = getInventory().get_child(draggingPanelIndex+1)
	#draggingSiblings.append(right)
	#var top = null
	#if (draggingPanelIndex >= getInventory().columns) :
		#top = getInventory().get_child(draggingPanelIndex-getInventory().columns)
	#draggingSiblings.append(top)
	#var bottom = null
	#if (draggingPanelIndex <= getInventory().get_child_count()-getInventory().columns-1) :
		#bottom = getInventory().get_child(draggingPanelIndex+getInventory().columns)
	#draggingSiblings.append(bottom)
	#
#func clearDraggingSiblings() :
	#draggingSiblings = []
	#
func _process(_delta) :
	if (!doneLoading) :
		return
	if (!myReady) :
		return
	#if (draggingPanel != null) :
		#var changed : bool = true
		#if (draggingSiblings[0] && draggingPanel.global_position.x < draggingSiblings[0].global_position.x) :
			#draggingPanelIndex -= 1
		#elif (draggingSiblings[1] && draggingPanel.global_position.x > draggingSiblings[1].global_position.x) :
			#draggingPanelIndex += 1
		#elif (draggingSiblings[2] && draggingPanel.global_position.y < draggingSiblings[2].global_position.y) :
			#draggingPanelIndex -= getInventory().columns
		#elif (draggingSiblings[3] && draggingPanel.global_position.y > draggingSiblings[3].global_position.y) :
			#draggingPanelIndex += getInventory().columns
		#else :
			#changed = false
		#if (changed) :
			#getInventory().move_child(ghostPanel, draggingPanelIndex)
			#updateDraggingSiblings()

##############################################
##Saving
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var children = getInventoryList()
	if (draggingPanel != null) :
		children.append(draggingPanel)
	if (children.is_empty()) :
		return tempDict
	for index in range(0,children.size()) :
		if (children[index].get_child_count() == 1) :
			tempDict["itemName"+str(index)] = children[index].get_child(0).getItemName()
			tempDict["itemDict"+str(index)] = children[index].get_child(0).getSaveDictionary()
		else :
			tempDict["itemName"+str(index)] = "null"
	tempDict["inventorySize"] = inventorySize
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (equippedEntries[key] == null) :
			tempDict["equippedIndex" + str(key)] = "null"
		else :
			tempDict["equippedIndex" + str(key)] = findPanelIndex(equippedEntries[key])
	return tempDict
	
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
	
func beforeLoad(newSave) :
	myReady = false
	if (!Definitions.DEVMODE) :
		$PanelContainer/HBoxContainer/LineEdit.queue_free()
	else :
		$PanelContainer/HBoxContainer/LineEdit.visible = true
	for key in Definitions.equipmentTypeDictionary.keys() :
		equippedEntries.append(null)
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (key == Definitions.equipmentTypeEnum.currency) :
			continue
		var newTab = $PanelContainer/HBoxContainer/ABC.duplicate()
		$PanelContainer/HBoxContainer.add_child(newTab)
		newTab.name = Definitions.equipmentTypeDictionary[key]
		newTab.setText(Definitions.equipmentTypeDictionary[key])
		newTab.connect("myPressed", _on_tab_button_pressed)
	$PanelContainer/HBoxContainer/ABC.connect("myPressed", _on_abc_pressed)
	if (newSave) :
		initialiseContainers()
	myReady = true
	emit_signal("myReadySignal")
	if (newSave) :
		doneLoading = true
		emit_signal("doneLoadingSignal")

func onLoad(loadDict) -> void :
	myReady = false 
	inventorySize = loadDict["inventorySize"]
	initialiseContainers()
	var inventorySlots = getInventoryList()
	for currentSlot in range(0,inventorySize) :
		if (loadDict["itemName" + str(currentSlot)] != "null") :
			var newEntry = SceneLoader.createEquipmentScene(loadDict["itemName" + str(currentSlot)])
			inventorySlots[currentSlot].add_child(newEntry)
			newEntry.setMouseFilter(Control.MOUSE_FILTER_PASS)
			newEntry.loadSaveDictionary(loadDict["itemDict"+str(currentSlot)])
			newEntry.connect("wasSelected", _on_item_selected)
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (loadDict["equippedIndex" + str(key)]) is String && loadDict["equippedIndex" + str(key)] == "null" :
			equippedEntries[key] = null
		else :
			equipItem(inventorySlots[loadDict["equippedIndex" + str(key)]].get_child(0))
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")

func _on_line_edit_text_submitted(new_text: String) -> void:
	if (EquipmentDatabase.getEquipment(new_text) != null) :
		var newScene = SceneLoader.createEquipmentScene(new_text)
		newScene.core = EquipmentDatabase.getEquipment(new_text).getAdjustedCopy(10)
		addToInventory(newScene)

const popupLoader = preload("res://Graphic Elements/popups/binary_decision.tscn")
func _on_button_pressed() -> void:
	var newPopup = popupLoader.instantiate()
	add_child(newPopup)
	newPopup.setTitle("Discard All")
	newPopup.setText("Discard all non-equipped items?")
	newPopup.setButton0Name("Confirm")
	newPopup.setButton1Name("Cancel")
	var choice = await newPopup.binaryChosen
	if (choice == 0) :
		var inventory = getInventory()
		for index in range(0, inventory.get_children().size()) :
			var panel = inventory.get_child(index)
			if (panel.get_child_count() != 1) :
				continue
			var item = panel.get_child(0)
			if (selectedEntry == item) :
				emit_signal("itemDeselected",item)
			if (equippedEntries.find(item) == -1) :
				panel.remove_child(item)
				item.queue_free()
