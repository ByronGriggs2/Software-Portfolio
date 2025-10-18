extends Control

signal itemSelected

var selectedEntry = null
var equippedEntry = null
var unarmedSlot = null
@export var inventorySize : int = 2
	
func addStatBonus(statDict : Dictionary) :
	if (equippedEntry != null) :
		equippedEntry.addStatBonus(statDict)
		
func addAttributeBonus(attributeDict : Dictionary) :
	if (equippedEntry != null) :
		equippedEntry.addAttributeBonus(attributeDict)
	
func getEquippedItem() :
	return equippedEntry
	
func selectItem(itemSceneRef) :
	if (itemSceneRef == unarmedSlot) :
		return
	itemSceneRef.select()
	for child in $ScrollContainer/CenterContainer/GridContainer.get_children() :
		if (child.get_child_count() == 1 && child.get_child(0) != itemSceneRef) :
			child.get_child(0).deselect()
	
func discardItem(itemSceneRef) :
	if (itemSceneRef == unarmedSlot) :
		return
	findPanel(itemSceneRef).remove_child(itemSceneRef)
	itemSceneRef.queue_free()
	
func findPanel(itemSceneRef) :
	for child in $ScrollContainer/CenterContainer/GridContainer.get_children() :
		if (child.get_child_count() == 1 && child.get_child(0) == itemSceneRef) :
			return child
	return null
	
func findPanelIndex(itemSceneRef) :
	var children = $ScrollContainer/CenterContainer/GridContainer.get_children()
	for index in range(0,children.size()) :
		if (children[index].get_child_count() == 1 && children[index].get_child(0) == itemSceneRef) :
			return index
	return null
	
func equipItem(itemSceneRef) :
	equippedEntry = itemSceneRef
	equippedEntry.equip()
func unequipItem(itemSceneRef) :
	equippedEntry.unequip()
	if (itemSceneRef.getType() == Definitions.equipmentTypeEnum.weapon) :
		equippedEntry = unarmedSlot
	else :
		equippedEntry = null
	
	
func addToInventory(itemSceneRef) :
	getNextEmptySlot().add_child(itemSceneRef)
	itemSceneRef.connect("wasSelected", _on_item_selected)

func getNextEmptySlot() :
	var children = $ScrollContainer/CenterContainer/GridContainer.get_children()
	if (children.is_empty()) :
		return null
	for child in children :
		if (child.get_child_count() == 0) :
			return child
	return null
	
func isInventoryFull() -> bool : 
	return (getNextEmptySlot() == null)
	
func _on_item_selected(itemSceneRef) :
	selectItem(itemSceneRef)
	emit_signal("itemSelected", itemSceneRef)
		
#func hasItem(item) :
	#for child in $ScrollContainer/CenterContainer/GridContainer.get_children() :
		#var storedItem = child.get_child(0)
		#if (storedItem != null) :
			#if (storedItem.isItem(item)) :
				#return true
	#return false
	
func initialiseContainers() :
	for index in range(0,inventorySize-1) :
		var newPanel = $ScrollContainer/CenterContainer/GridContainer/Panel.duplicate()
		$ScrollContainer/CenterContainer/GridContainer.add_child(newPanel)
			
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var children = $ScrollContainer/CenterContainer/GridContainer.get_children()
	if (children.is_empty()) :
		return tempDict
	for index in range(0,children.size()) :
		if (children[index].get_child_count() == 1) :
			tempDict["itemName"+str(index)] = children[index].get_child(0).getItemName()
			tempDict["itemDict"+str(index)] = children[index].get_child(0).getSaveDictionary()
		else :
			tempDict["itemName"+str(index)] = "null"
	tempDict["inventorySize"] = inventorySize
	if (equippedEntry == null) :
		tempDict["equippedIndex"] = "null"
	else :
		tempDict["equippedIndex"] = findPanelIndex(equippedEntry)
	return tempDict
	
var myReady : bool = false
func _ready() :
	myReady = true

func beforeLoad(newSave) :
	if (newSave) :
		initialiseContainers()

func onLoad(loadDict) -> void :
	inventorySize = loadDict["inventorySize"]
	initialiseContainers()
	var inventorySlots = $ScrollContainer/CenterContainer/GridContainer.get_children()
	for currentSlot in range(0,inventorySlots.size()) :
		if (loadDict["itemName" + str(currentSlot)] != "null") :
			var newEntry = SceneLoader.createEquipmentScene(loadDict["itemName" + str(currentSlot)])
			newEntry.loadSaveDictionary(loadDict["itemDict"+str(currentSlot)])
			inventorySlots[currentSlot].add_child(newEntry)
			newEntry.connect("wasSelected", _on_item_selected)
	if (loadDict["equippedIndex"] is String && loadDict["equippedIndex"] == "null") :
		return
	equipItem(inventorySlots[loadDict["equippedIndex"]].get_child(0))
	
signal equipmentLoadedFromDisk
func afterLoad() :
	if (name == Definitions.equipmentTypeDictionary[Definitions.equipmentTypeEnum.weapon]) :
		var unarmedName = "unarmed_" + Definitions.classDictionary[Definitions.getPlayerClass()]
		unarmedSlot = SceneLoader.createEquipmentScene(unarmedName)
		add_child(unarmedSlot)
		unarmedSlot.visible = false
		if (equippedEntry == null) :
			equipItem(unarmedSlot)
	emit_signal("equipmentLoadedFromDisk", equippedEntry, self)
