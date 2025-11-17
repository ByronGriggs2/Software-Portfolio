extends Panel

var selectedEntry : Node = null
var equippedEntries : Array[Node] = []
@export var inventorySize : int = 10

#################################
##Setters
func addToInventory(itemSceneRef) :
	getNextEmptySlot().add_child(itemSceneRef)
	itemSceneRef.connect("wasSelected", _on_item_selected)
	itemSceneRef.connect("wasDeselected", _on_item_deselected)
func discardItem(itemSceneRef) :
	if (itemSceneRef.isEquipped()) :
		unequipItem(itemSceneRef.getType())
	itemSceneRef.queue_free()
#################################
##Getters
func getEquippedItem(type : Definitions.equipmentTypeEnum) :
	return equippedEntries[type]
func isInventoryFull() -> bool : 
	return (getNextEmptySlot() == null)
func getModifierPacket() -> ModifierPacket :
	var retVal = ModifierPacket.new()
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (equippedEntries[key] != null) :
			equippedEntries[key].addToModifierPacket(retVal)
	return retVal
################################
##Other
func selectItem(itemSceneRef) :
	itemSceneRef.select()
	for child in getInventoryList() :
		if (child.get_child_count() == 1 && child.get_child(0) != itemSceneRef) :
			child.get_child(0).deselect()

signal itemEquipped
func equipItem(itemSceneRef) :
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
	return $ScrollContainer/CenterContainer/GridContainer.get_children()
func getInventory() -> Node :
	return $ScrollContainer/CenterContainer/GridContainer
##################################
##Internal
func findPanel(itemSceneRef) :
	for child in getInventoryList() :
		if (child.get_child_count() == 1 && child.get_child(0) == itemSceneRef) :
			return child
	return null
func findPanelIndex(itemSceneRef) :
	var children = getInventoryList()
	for index in range(0,children.size()) :
		if (children[index].get_child_count() == 1 && children[index].get_child(0) == itemSceneRef) :
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
		$ScrollContainer/CenterContainer/GridContainer.add_child(newPanel)
##############################################
##Saving
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var children = getInventoryList()
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
func _ready() :
	myReady = true
	
func beforeLoad(newSave) :
	for key in Definitions.equipmentTypeDictionary.keys() :
		equippedEntries.append(null)
	for key in Definitions.equipmentTypeDictionary.keys() :
		var newTab = $PanelContainer/HBoxContainer/ABC.duplicate()
		$PanelContainer/HBoxContainer.add_child(newTab)
		newTab.name = Definitions.equipmentTypeDictionary[key]
		newTab.setText(Definitions.equipmentTypeDictionary[key])
		newTab.connect("myPressed", _on_tab_button_pressed)
	$PanelContainer/HBoxContainer/ABC.connect("myPressed", _on_abc_pressed)
	if (newSave) :
		initialiseContainers()

func onLoad(loadDict) -> void : 
	inventorySize = loadDict["inventorySize"]
	initialiseContainers()
	var inventorySlots = getInventoryList()
	for currentSlot in range(0,inventorySize) :
		if (loadDict["itemName" + str(currentSlot)] != "null") :
			var newEntry = SceneLoader.createEquipmentScene(loadDict["itemName" + str(currentSlot)])
			inventorySlots[currentSlot].add_child(newEntry)
			newEntry.loadSaveDictionary(loadDict["itemDict"+str(currentSlot)])
			newEntry.connect("wasSelected", _on_item_selected)
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (loadDict["equippedIndex" + str(key)]) is String && loadDict["equippedIndex" + str(key)] == "null" :
			equippedEntries[key] = null
		else :
			equipItem(inventorySlots[loadDict["equippedIndex" + str(key)]].get_child(0))
