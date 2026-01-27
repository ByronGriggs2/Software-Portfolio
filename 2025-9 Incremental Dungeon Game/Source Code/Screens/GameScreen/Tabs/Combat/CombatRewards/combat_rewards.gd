extends Panel

signal finished
signal waitingForUser

const entryLoader = preload("res://Screens/GameScreen/Tabs/Combat/CombatRewards/combat_reward_entry.tscn")

func _ready() :
	self.visible = false
	$Content/Details.setOption0(" Take ")
	$Content/Details.setOption1(" Discard ")

var initialisationPending : bool = false
signal initialisationComplete
signal itemListForYourInspectionGoodSir
func initialise(rewards : Dictionary) :
	initialisationPending = true
	for index in range(0, rewards["currency"].size()) :
		if (rewards["currency"][index] == 0) :
			continue
		var entry = entryLoader.instantiate()
		$Content/InventoryPanel/ScrollContainer/VBoxContainer.add_child(entry)
		entry.initialise_currency(index, rewards["currency"][index])
		entry.connect("wasSelected", _on_entry_selected)
	for index in range(0,rewards["equipment"].size()) :
		var entry = entryLoader.instantiate()
		$Content/InventoryPanel/ScrollContainer/VBoxContainer.add_child(entry)
		entry.initialise(rewards["equipment"][index])
		entry.connect("wasSelected", _on_entry_selected)
	var changed : bool = false
	var optionsCopy = IGOptions.getIGOptionsCopy()
	var encounteredItems : Array = optionsCopy["encounteredItems"]
	for entry in getItemList() :
		if (encounteredItems.find(entry.getItemSceneRef().getItemName()) == -1) :
			changed = true
			encounteredItems.append(entry.getItemSceneRef().getItemName())
	if (changed) :
		encounteredItems.sort_custom(func(a,b) : a<b)
		optionsCopy["encounteredItems"] = encounteredItems
		IGOptions.saveAndUpdateIGOptions(optionsCopy)
		IGOptions.updateGlobalSettings_items()
		IGOptions.checkEquipmentEncyclopedia()
	
	for entry in getItemList() :
		if (entry.getItemSceneRef().getType() == Definitions.equipmentTypeEnum.currency) :
			waitingForResponse = true
			emit_signal("addToInventoryRequested", entry.getItemSceneRef())
			if (waitingForResponse) :
				await responseReceived
		
	var filter = IGOptions.getIGOptionsCopy()["filter"]
	var itemsToDelete : Array[Node] = []
	for entry in getItemList() :
		if (!Helpers.equipmentIsNew(entry.getItemSceneRef().core)) :
			continue
		var grouping : EquipmentGroups = entry.getItemSceneRef().core.equipmentGroups
		var delete : bool = false
		if (filter["type"] == "whitelist") :
			delete = true
			for element in grouping.getElements() :
				delete = delete && !(filter["element"][element])
			delete = delete || !(filter["quality"][grouping.quality])
			delete = delete || !(filter["equipmentType"][entry.getItemSceneRef().getType()])
			if (grouping.weaponClass != -1) :
				delete = delete || !(filter["weaponType"][grouping.weaponClass])
		else :
			for element in grouping.getElements() :
				delete = delete || filter["element"][element]
			delete = delete || filter["quality"][grouping.quality]
			delete = delete || filter["equipmentType"][entry.getItemSceneRef().getType()]
			if (grouping.weaponClass != -1) :
				delete = delete || filter["weaponType"][grouping.weaponClass]
		if (delete) :
			itemsToDelete.append(entry.getItemSceneRef())
			continue
	removeItemsFromList_internal(itemsToDelete)
	await get_tree().process_frame
	if (getItemList().is_empty()) :
		suicide()
		return
		
	emit_signal("itemListForYourInspectionGoodSir", getItemList(), self)
	if (waitingForResponse) :
		await responseReceived
		
	await get_tree().process_frame
	if (getItemList().is_empty()) :
		suicide()
	else :
		var firstEntry = $Content/InventoryPanel/ScrollContainer/VBoxContainer.get_child(0)
		firstEntry.getItemSceneRef().select()
		$Content/Details.setItemSceneRefBase(firstEntry.getItemSceneRef())
		self.visible = true
		initialisationPending = false
		emit_signal("initialisationComplete")
		emit_signal("waitingForUser")
	var selectedCount = 0
	for entry in getItemList() :
		if (entry.isSelected()) :
			selectedCount += 1
	if (selectedCount > 1) :
		for entry in getItemList() :
			entry.deselect()
		getItemList()[0].select()
		
func _on_entry_selected(itemSceneRef) :
	$Content/Details.setItemSceneRefBase(itemSceneRef)
	for child in getItemList() :
		if (child.getItemSceneRef() != itemSceneRef) :
			child.getItemSceneRef().deselect()

signal addToInventoryRequested
signal newItemDropped
func _on_details_option_pressed(itemSceneRef, val : int) -> void:
	if (val == 0) :
		waitingForResponse = true
		emit_signal("addToInventoryRequested", itemSceneRef)
	else :
		removeItemFromList(itemSceneRef)
	
func findItem(item : Equipment) :
	for child in $Content/InventoryPanel/ScrollContainer/VBoxContainer.get_children() :
		if (child.getItemSceneRef() == item) :
			return child
	return null
	
func getItemList() :
	return $Content/InventoryPanel/ScrollContainer/VBoxContainer.get_children()
	
func removeItemsFromList_internal(items : Array[Node]) :
	if (items.size() == 0) :
		return
	var killableIndex
	var killableScene
	for itemSceneRef in items :
		var itemList = $Content/InventoryPanel/ScrollContainer/VBoxContainer.get_children()
		for index in range(0,itemList.size()) :
			if (itemList[index].getItemSceneRef() == itemSceneRef) :
				killableIndex = index
				killableScene = itemList[index]
				break
		#Explicitly remove child because sometimes it seems to take more than 1 frame
		$Content/InventoryPanel/ScrollContainer/VBoxContainer.remove_child(killableScene)
		killableScene.queue_free()
	await get_tree().process_frame
	var newItemList = $Content/InventoryPanel/ScrollContainer/VBoxContainer.get_children()
	if (newItemList.is_empty()) :
		suicide()
		return
	var newIndex
	if (killableIndex == newItemList.size()) :
		newIndex = newItemList.size()-1
	else :
		newIndex = killableIndex
	if (newItemList.size()-1>=newIndex && newItemList[newIndex] != null) :
		$Content/Details.setItemSceneRefBase(newItemList[newIndex].getItemSceneRef())
	newItemList[newIndex].getItemSceneRef().select()
	_on_entry_selected(newItemList[newIndex].getItemSceneRef())
		
func removeItemFromList(itemSceneRef) :
	removeItemsFromList_internal([itemSceneRef])
	waitingForResponse = false
	addDenied = false
	emit_signal("responseReceived")
	
func denyAddToInventory() :
	waitingForResponse = false
	addDenied = true
	emit_signal("responseReceived")

var waitingForResponse : bool = false
var addDenied : bool = false
signal responseReceived
func _on_take_all_button_pressed() -> void:
	for item in getItemList() :
		if (waitingForResponse) :
			await responseReceived
		if (addDenied) :
			addDenied = false
			return
		waitingForResponse = true
		emit_signal("addToInventoryRequested", item.getItemSceneRef())
	if (waitingForResponse) :
		await responseReceived
	if (addDenied) :
		addDenied = false
		return
	suicide()

func suicide() :
	if (initialisationPending) :
		initialisationPending = false
		emit_signal("initialisationComplete")
	finishedBool = true
	emit_signal("finished")
	queue_free()

var finishedBool : bool = false
func isFinished() -> bool :
	return finishedBool
	

signal currentlyEquippedItemRequested
func _on_details_currently_equipped_item_requested(emitter, type) -> void:
	emit_signal("currentlyEquippedItemRequested", emitter, type)
