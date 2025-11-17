extends Panel

signal finished

const entryLoader = preload("res://Screens/GameScreen/Tabs/Combat/CombatRewards/combat_reward_entry.tscn")

func _ready() :
	$Content/Details.setOption0("Take")
	$Content/Details.setOption1("Discard")

func initialise(rewards : Array[Equipment]) :
	for index in range(0,rewards.size()) :
		var entry = entryLoader.instantiate()
		$Content/InventoryPanel/VBoxContainer.add_child(entry)
		entry.initialise(rewards[index].resource_path.get_file().get_basename())
		entry.connect("wasSelected", _on_entry_selected)
	var firstEntry = $Content/InventoryPanel/VBoxContainer.get_child(0)
	firstEntry.getItemSceneRef().select()
	$Content/Details.setItemSceneRefBase(firstEntry.getItemSceneRef())
		
func _on_entry_selected(itemSceneRef) :
	$Content/Details.setItemSceneRefBase(itemSceneRef)
	for child in $Content/InventoryPanel/VBoxContainer.get_children() :
		if (child.getItemSceneRef() != itemSceneRef) :
			child.getItemSceneRef().deselect()

signal addToInventoryRequested
func _on_details_option_pressed(itemSceneRef, val : int) -> void:
	if (val == 0) :
		emit_signal("addToInventoryRequested", itemSceneRef)
	else :
		removeItemFromList(itemSceneRef)
	
#func findItem(item : Equipment) :
	#for child in $Content/InventoryPanel/VBoxContainer.get_children() :
		#if (child.getRewardRef() == item) :
			#return child
	#return null

func removeItemFromList(itemSceneRef) :
	var itemList = $Content/InventoryPanel/VBoxContainer.get_children()
	var killableIndex
	var killableScene
	for index in range(0,itemList.size()) :
		if (itemList[index].getItemSceneRef() == itemSceneRef) :
			killableIndex = index
			killableScene = itemList[index]
			break
	#Explicitly remove child because sometimes it seems to take more than 1 frame
	$Content/InventoryPanel/VBoxContainer.remove_child(killableScene)
	killableScene.queue_free()
	await get_tree().process_frame
	var newItemList = $Content/InventoryPanel/VBoxContainer.get_children()
	if (newItemList.is_empty()) :
		emit_signal("finished")
		queue_free()
		return
	var newIndex
	if (killableIndex == newItemList.size()) :
		newIndex = newItemList.size()-1
	else :
		newIndex = killableIndex
	$Content/Details.setItemSceneRefBase(newItemList[newIndex].getItemSceneRef())
	newItemList[newIndex].getItemSceneRef().select()
