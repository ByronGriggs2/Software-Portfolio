extends Panel

var currentFloor = null
var currentRoom = null
var playerReference = null
var friendlyParty : Array[ActorPreset]

func _ready() :
	friendlyParty.resize(1)
	currentFloor = $MapContainer.get_child(0)
	currentFloor.visible = true

func _on_level_chosen(emitter, encounter) -> void:
	friendlyParty[0] = playerReference.core
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
		_on_combat_panel_victory()
		return
	var copy : Array[ActorPreset]
	for elem in friendlyParty :
		copy.append(elem.duplicate())
	$CombatPanel.resetCombat(copy, encounter.enemies)
	hideMap()
	$CombatPanel.visible = true

func _on_combat_panel_victory() -> void:
	if (currentRoom.getEncounterRef().victoryText != "") :
		$NarrativePanel.text = currentRoom.getEncounterRef().victoryText
		$NarrativePanel.title = currentRoom.getEncounterRef().victoryTitle
		$NarrativePanel.visible = true
		await($NarrativePanel.continueSignal)
		$NarrativePanel.visible = false
	if (!currentRoom.getEncounterRef().rewards.is_empty()) :
		await handleCombatRewards(currentRoom.getEncounterRef().rewards)		
	currentFloor.completeLevel(currentRoom)
	currentRoom = null
	$CombatPanel.visible = false
	showMap()

func _on_combat_panel_defeat() -> void:
	currentFloor.onCombatLoss(currentRoom)
	startDefeatCoroutine()
	
func _on_combat_panel_retreat() -> void:
	interruptDefeatCoroutine()
	currentRoom = null
	showMap()

func addPlayerReference(player) :
	playerReference = player
	
func hideMap() :
	#currentFloor.visible = false
	$MapContainer.visible = false
func showMap() :
	#currentFloor.visible = true
	$MapContainer.visible = true

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
	var copy : Array[ActorPreset]
	for elem in friendlyParty :
		copy.append(elem.duplicate())
	$CombatPanel.resetCombat(copy, currentRoom.getEncounterRef().enemies)
	defeatCoroutineRunning = false
	
func interruptDefeatCoroutine() :
	defeatCoroutineInterrupted = true

#only works if combat is not currently paused
func _on_developer_console_console_opened() -> void:
	if ($CombatPanel.visible) :
		$CombatPanel.pauseCombat()

func _on_developer_console_console_closed() -> void:
	$CombatPanel.restartCombat()

func _on_developer_console_set_player_core(args) -> void:
	var filepath = "res://Screens/GameScreen/Tabs/Combat/Actors/" + args[0]
	if (!FileAccess.file_exists(filepath)) :
		return
	$CombatPanel/FriendlyParty.get_child(0).core = load(filepath)

const combatRewardsLoader = preload("res://Screens/GameScreen/Tabs/Combat/CombatRewards/combat_rewards.tscn")
func handleCombatRewards(rewards : Array[Equipment]) :
	var rewardHandler = combatRewardsLoader.instantiate()
	add_child(rewardHandler)
	rewardHandler.initialise(rewards)
	rewardHandler.connect("addToInventoryRequested", _on_add_to_inventory_request)

signal addToInventoryRequested
func _on_add_to_inventory_request(itemSceneRef) :
	emit_signal("addToInventoryRequested", itemSceneRef)

func removeCombatRewardEntry(itemSceneRef) :
	get_node("CombatRewards").removeItemFromList(itemSceneRef)
