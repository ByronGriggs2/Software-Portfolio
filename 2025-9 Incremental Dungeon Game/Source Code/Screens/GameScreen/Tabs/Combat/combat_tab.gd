extends Panel

var currentFloor = null
var maxFloor : int = 0
var currentRoom = null
var friendlyParty : Array[ActorPreset]

#########################
var playerCore_comm : ActorPreset = null
var waitingForPlayerCore : bool = false
signal playerCoreRequested
signal playerCoreReceived

func getPlayerCore() -> ActorPreset:
	waitingForPlayerCore = true
	emit_signal("playerCoreRequested", self)
	if (waitingForPlayerCore) :
		await playerCoreReceived
	return playerCore_comm
	
func providePlayerCore(val : ActorPreset) :
	playerCore_comm = val
	waitingForPlayerCore = false
	emit_signal("playerCoreReceived")
##########################

func _on_level_chosen(emitter, encounter) -> void:
	friendlyParty[0] = await getPlayerCore()
	currentRoom = emitter
	if (currentRoom.isFirstEntry()) :
		currentRoom.enter()
		if (encounter.introText != "") :
			$NarrativePanel.text = encounter.introText
			$NarrativePanel.title = encounter.introTitle
			$NarrativePanel.visible = true
			await($NarrativePanel.continueSignal)
			$NarrativePanel.visible = false
	if (encounter.enemies.is_empty()) :
		_on_combat_panel_victory(false)
		return
	var copy : Array[ActorPreset]
	for elem in friendlyParty :
		copy.append(elem.duplicate())
	$CombatPanel.resetCombat(copy, encounter.enemies)
	hideMap()
	$CombatPanel.visible = true
	
signal tutorialRequested
func _on_tutorial_requested(tutorialName, tutorialPos) :
	emit_signal("tutorialRequested", tutorialName, tutorialPos)

func _on_combat_panel_victory(automaticReset : bool) -> void:
	if (currentRoom.getEncounterRef().victoryText != "") :
		$NarrativePanel.text = currentRoom.getEncounterRef().victoryText
		$NarrativePanel.title = currentRoom.getEncounterRef().victoryTitle
		$NarrativePanel.visible = true
		await($NarrativePanel.continueSignal)
		$NarrativePanel.visible = false	
	var magicFind = await getMagicFind()
	await handleCombatRewards(currentRoom.getEncounterRef().getRewards(magicFind))		
	currentFloor.completeRoom(currentRoom)
	if (automaticReset) :
		friendlyParty[0] = await getPlayerCore()
		var copy : Array[ActorPreset]
		for elem in friendlyParty :
			copy.append(elem.duplicate())
		$CombatPanel.resetCombat(copy, currentRoom.getEncounterRef().enemies)
	else :
		currentRoom = null
		$CombatPanel.visible = false
		showMap()

var waitingForMagicFind : bool = false
signal magicFindDone
signal magicFindRequested
var magicFind_comm
func getMagicFind() :
	waitingForMagicFind = true
	emit_signal("magicFindRequested", self)
	if (waitingForMagicFind) :
		await magicFindDone
	return magicFind_comm
func provideMagicFind(val) :
	magicFind_comm = val
	waitingForMagicFind = false
	emit_signal("magicFindDone")

func _on_combat_panel_defeat() -> void:
	currentFloor.onCombatLoss(currentRoom)
	startDefeatCoroutine()
	
func _on_combat_panel_retreat() -> void:
	interruptDefeatCoroutine()
	currentFloor.onCombatRetreat(currentRoom)
	currentRoom = null
	showMap()
	
func _on_map_container_visibility_changed() -> void:
	$FloorDisplay.visible = $MapContainer.visible
	
signal playerClassRequested
func _on_player_class_requested(emitter) :
	emit_signal("playerClassRequested", emitter)
	
signal newFloorCompleted
func _on_map_completed(emitter) :
	var completedIndex = Helpers.findIndexInContainer($MapContainer, emitter)
	if (completedIndex == maxFloor && $MapContainer.get_child_count() > maxFloor+1) :
		maxFloor += 1
		$FloorDisplay.setMaxFloor(maxFloor)
		var typicalEnemyDefense = emitter.getTypicalEnemyDefense()
		emit_signal("newFloorCompleted", typicalEnemyDefense)

func _on_floor_display_floor_up() -> void:
	var currentFloorIndex = Helpers.findIndexInContainer($MapContainer, currentFloor)
	if (currentFloorIndex != null && currentFloorIndex != 0) :
		currentFloor.visible = false
		currentFloor = $MapContainer.get_child(currentFloorIndex-1)
		currentFloor.visible = true
		$FloorDisplay.setFloor(currentFloorIndex-1)

func _on_floor_display_floor_down() -> void:
	var currentFloorIndex = Helpers.findIndexInContainer($MapContainer, currentFloor)
	if (currentFloorIndex != null && currentFloorIndex != maxFloor) :
		currentFloor.visible = false
		currentFloor = $MapContainer.get_child(currentFloorIndex+1)
		currentFloor.visible = true
		$FloorDisplay.setFloor(currentFloorIndex+1)
		
signal routineUnlockRequested
func _on_routine_unlock_requested(routine : AttributeTraining) :
	emit_signal("routineUnlockRequested", routine)
	
signal shopRequested
func _on_shop_requested(details) :
	emit_signal("shopRequested", details)
	
func hideMap() :
	#currentFloor.visible = false
	$MapContainer.visible = false
func showMap() :
	#currentFloor.visible = true
	$MapContainer.visible = true
func disableUI() :
	currentFloor.disableUI()
	$FloorDisplay.visible = false
func enableUI() :
	currentFloor.enableUI()
	$FloorDisplay.visible = true

#######################################
var defeatCoroutineRunning : bool = false
var defeatCoroutineInterrupted : bool = false

func startDefeatCoroutine() :
	if (defeatCoroutineRunning) :
		return
	defeatCoroutineRunning = true
	defeatCoroutineInterrupted = false
	await get_tree().create_timer(3.0).timeout
	if (defeatCoroutineInterrupted) :
		defeatCoroutineRunning = false
		return
	friendlyParty[0] = await getPlayerCore()
	var copy : Array[ActorPreset]
	for elem in friendlyParty :
		copy.append(elem.duplicate())
	$CombatPanel.resetCombat(copy, currentRoom.getEncounterRef().enemies)
	defeatCoroutineRunning = false
	
func interruptDefeatCoroutine() :
	defeatCoroutineInterrupted = true

const combatRewardsLoader = preload("res://Screens/GameScreen/Tabs/Combat/CombatRewards/combat_rewards.tscn")
var firstReward : bool = true
func handleCombatRewards(rewards : Array[Equipment]) :
	if (rewards.is_empty()) :
		return
	if (firstReward) :
		firstReward = false
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.equipment, Vector2(0,0))
	var rewardHandler = combatRewardsLoader.instantiate()
	add_child(rewardHandler)
	rewardHandler.connect("addToInventoryRequested", _on_add_to_inventory_request)
	rewardHandler.initialise(rewards)
	#if (rewardHandler.initialisationPending) :
		#await rewardHandler.initialisationComplete
	if (!rewardHandler.isFinished()) :
		await rewardHandler.finished

signal addToInventoryRequested
func _on_add_to_inventory_request(itemSceneRef, isAutomatic : bool) :
	emit_signal("addToInventoryRequested", itemSceneRef, isAutomatic)

func removeCombatRewardEntry(itemSceneRef) :
	get_node("CombatRewards").removeItemFromList(itemSceneRef)
	
func denyAddToInventory() :
	get_node("CombatRewards").denyAddToInventory()
	
func getSaveDictionary() -> Dictionary :
	var tempDict : Dictionary = {}
	tempDict["firstReward"] = firstReward
	tempDict["maxFloor"] = maxFloor
	var currentFloorIndex = Helpers.findIndexInContainer($MapContainer, currentFloor)
	if (currentFloorIndex == null) :
		currentFloorIndex = "null"
	tempDict["currentFloorIndex"] = currentFloorIndex
	return tempDict
var myReady : bool = false
func _ready() :
	myReady = true
func beforeLoad(newGame) :
	friendlyParty.resize(1)
	if (newGame) :
		currentFloor = $MapContainer.get_child(0)
		$FloorDisplay.setFloor(0)
		currentFloor.visible = true
	for map in $MapContainer.get_children() :
		if (map.has_signal("levelChosen2")) :
			map.connect("levelChosen2", _on_level_chosen)
		if (map.has_signal("playerClassRequested")) :
			map.connect("playerClassRequested", _on_player_class_requested)
		if (map.has_signal("tutorialRequested")) :
			map.connect("tutorialRequested", _on_tutorial_requested)
		if (map.has_signal("routineUnlockRequested")) :
			map.connect("routineUnlockRequested", _on_routine_unlock_requested)
		if (map.has_signal("shopRequested")) :
			map.connect("shopRequested", _on_shop_requested)
		map.connect("mapCompleted", _on_map_completed)
func onLoad(loadDict : Dictionary) :
	firstReward = loadDict["firstReward"]
	maxFloor = loadDict["maxFloor"]
	$FloorDisplay.setMaxFloor(maxFloor)
	var currentFloorIndex = loadDict["currentFloorIndex"]
	if (currentFloorIndex is String && currentFloorIndex == "null") :
		currentFloor = $MapContainer.get_child(0)
		$FloorDisplay.setFloor(0)
	else :
		currentFloor = $MapContainer.get_child(currentFloorIndex)
		$FloorDisplay.setFloor(currentFloorIndex)
	currentFloor.visible = true
