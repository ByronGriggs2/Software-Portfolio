extends Control

var currentRoom = null
var friendlyParty : Array[ActorPreset]

func _ready() :
	friendlyParty.resize(1)

func _on_level_chosen(emitter, encounter) -> void:
	currentRoom = emitter
	var copy : Array[ActorPreset]
	for elem in friendlyParty :
		copy.append(elem.duplicate())
	$CombatPanel.resetCombat(copy, encounter.enemies)
	$CombatMap.visible = false
	$CombatPanel.visible = true

func _on_combat_panel_victory() -> void:
	$CombatMap.completeLevel(currentRoom)
	currentRoom = null
	$CombatPanel.visible = false
	$CombatMap.visible = true

func _on_combat_panel_defeat() -> void:
	startDefeatCoroutine()
	
func _on_combat_panel_retreat() -> void:
	interruptDefeatCoroutine()
	currentRoom = null
	$CombatPanel.visible = false
	$CombatMap.visible = true

func addPlayerReference(player) :
	friendlyParty[0] = player.core

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
	$CombatPanel.resetCombat(copy, currentRoom.encounter.enemies)
	defeatCoroutineRunning = false
	
func interruptDefeatCoroutine() :
	defeatCoroutineInterrupted = true
