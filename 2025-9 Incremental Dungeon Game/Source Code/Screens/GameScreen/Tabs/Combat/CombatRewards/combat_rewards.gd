extends Panel

signal finished

const entryLoader = preload("res://Screens/GameScreen/Tabs/Combat/CombatRewards/combat_reward_entry.tscn")

func _ready() :
	$Details.setOption0("Take")
	$Details.setOption1("Discard")

func initialise(rewards : Array[Equipment]) :
	for reward in rewards :
		var entry = entryLoader.instantiate()
		$ItemPanel/VBoxContainer.add_child(entry)
		entry.initialise(reward.resource_path.get_file().get_basename())
		entry.connect("wasSelected", _on_entry_selected)
		
func _on_entry_selected(itemSceneRef) :
	$Details.visible = true
	$Details.setItemSceneRefBase(itemSceneRef)
	for child in $ItemPanel/VBoxContainer.get_children() :
		if (child.getItemSceneRef() != itemSceneRef) :
			child.getItemSceneRef().deselect()

signal addToInventoryRequested
func _on_details_option_pressed(itemSceneRef, val : int) -> void:
	if (val == 0) :
		emit_signal("addToInventoryRequested", itemSceneRef)
	else :
		removeItemFromList(itemSceneRef)
	
#func findItem(item : Equipment) :
	#for child in $ItemPanel/VBoxContainer.get_children() :
		#if (child.getRewardRef() == item) :
			#return child
	#return null

func removeItemFromList(itemSceneRef) :
	var itemList = $ItemPanel/VBoxContainer.get_children()
	var killableIndex
	var killableScene
	for index in range(0,itemList.size()) :
		if (itemList[index].getItemSceneRef() == itemSceneRef) :
			killableIndex = index
			killableScene = itemList[index]
			break
	$ItemPanel/VBoxContainer.remove_child(killableScene)
	killableScene.queue_free()
	var newItemList = $ItemPanel/VBoxContainer.get_children()
	if (newItemList.is_empty()) :
		emit_signal("finished")
		queue_free()
		return
	var newIndex
	if (killableIndex == newItemList.size()) :
		newIndex = killableIndex-1
	else :
		newIndex = killableIndex+1
	$Details.setItem(newItemList[newIndex].getItemSceneRef())
	newItemList[newIndex].getItemSceneRef().select()
