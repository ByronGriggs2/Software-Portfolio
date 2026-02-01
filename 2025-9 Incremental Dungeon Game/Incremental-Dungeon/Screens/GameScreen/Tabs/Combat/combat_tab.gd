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
signal playerPortraitRequested
func _on_player_portrait_requested(emitter) :
	emit_signal("playerPortraitRequested", emitter)
##########################
func getScalingRows() :
	return $ProceduralGenerationLogic.getScalingRows_mapFinished(maxFloor, getFurthestProgression())
func getMostRecentEquipmentScaling() -> float :
	var currentRow = getFurthestProgression()
	var scalingRows = $ProceduralGenerationLogic.getScalingRows_mapFinished(maxFloor,currentRow)
	return $ProceduralGenerationLogic.getEquipmentScaling(scalingRows)
func getMostRecentCurrencyScaling(type) -> float :
	var row = getFurthestProgression()
	return getCurrencyScaling(type, maxFloor, row)
func getCurrencyScalingInCurrentRoom(type) -> float :
	return getCurrencyScaling(type, getCurrentFloorIndex(), currentFloor.getRoomRow(currentRoom))
func getCurrencyScaling(type : String, myFloor : int, row : int) -> float :
	var scalingRows = $ProceduralGenerationLogic.getScalingRows_mapFinished(myFloor,row)
	if (type == "routine") :
		return $ProceduralGenerationLogic.getGoldScaling(scalingRows)
	elif (type == "armor" || type == "weapon") :
		return $ProceduralGenerationLogic.getOreScaling(scalingRows)
	elif (type == "soul") :
		return $ProceduralGenerationLogic.getSoulScaling(scalingRows)
	else :
		return 0
func getFurthestProgression() :
	if ($MapContainer.get_child_count() == 1) :
		return 0
	return $MapContainer.get_children().back().getFurthestProgression()
func createRandomShopItem(type : Definitions.equipmentTypeEnum) -> Equipment :
	return $ProceduralGenerationLogic.createRandomShopItem(type, maxFloor, getFurthestProgression())
func _on_level_chosen(emitter, encounter) -> void:
	friendlyParty[0] = await getPlayerCore()
	currentRoom = emitter
	if (currentRoom.isFirstEntry()) :
		currentRoom.enter()
		if (encounter.introText != "") :
			await launchNarrative(encounter.introTitle, encounter.introText, "Continue", true, false)
	if (encounter.enemies.is_empty()) :
		_on_combat_panel_victory(false)
		return
	var copy : Array[ActorPreset]
	for elem in friendlyParty :
		copy.append(elem.duplicate())
	$CombatPanel.resetCombat(copy, encounter.enemies)
	hideMapAndUI()
	$CombatPanel.visible = true
	
signal tutorialRequested
func _on_tutorial_requested(tutorialName, tutorialPos) :
	emit_signal("tutorialRequested", tutorialName, tutorialPos)

func _on_combat_panel_victory(automaticReset : bool) -> void:
	$CombatPanel.pauseCombat()
	if (currentRoom.getEncounterRef().victoryText != "" && !currentRoom.isCompleted()) :
		await launchNarrative(currentRoom.getEncounterRef().victoryTitle, currentRoom.getEncounterRef().victoryText, "Continue", true, false)
	var magicFind = await getMagicFind()
	var rewards
	## New
	if (currentFloor.has_method("getEnvironment")) :
		rewards = $ProceduralGenerationLogic.generateDrops(getCurrentFloorIndex(), currentRoom, currentFloor.getEnvironment(), magicFind)
	else :
		var rewardsTutorial = currentRoom.getEncounterRef().getRewards(magicFind)
		rewards = {
			"equipment" : rewardsTutorial,
			"currency" : [0]
		}
	hideMapAndUI()
	if (Definitions.hasDLC && currentRoom.has_method("getEncounterRef") && currentRoom.getEncounterRef().enemies.size() != 0 && currentRoom.getEncounterRef().enemies[0].getResourceName() == "athena") :
		var oldSettings = IGOptions.getIGOptionsCopy()
		var newSettings = oldSettings.duplicate(true)
		newSettings["filter"] = IGOptions.getDefaultOptionDict().duplicate()["filter"]
		IGOptions.saveIGOptionsNoUpdate(newSettings)
		await handleCombatRewards(rewards)
		IGOptions.saveIGOptionsNoUpdate(oldSettings)
	else :
		await handleCombatRewards(rewards)
	#showMapAndUI()
	if (currentRoom == null) :
		return
	if ($MapContainer.get_child_count() >= 2 && currentFloor == $MapContainer.get_child(1) && (currentRoom.name as String) == "N0") :
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.row1, Vector2(0,0))
	if (currentRoom != null) :
		currentFloor.completeRoom(currentRoom)
	if (automaticReset && currentRoom != null) :
		friendlyParty[0] = await getPlayerCore()
		var copy : Array[ActorPreset]
		for elem in friendlyParty :
			copy.append(elem.duplicate())
		$CombatPanel.resetCombat(copy, currentRoom.getEncounterRef().enemies)
		$CombatPanel.resumeCombat()
	else :
		currentRoom = null
		$CombatPanel.visible = false
		if (!narrativeWorking) :
			showMapAndUI()
			
func getCurrentFloorIndex() :
	var index = 0
	for child in $MapContainer.get_children() :
		if (child == currentFloor) :
			break
		index += 1
	return index

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
	$CombatPanel.pauseCombat()
	currentFloor.onCombatRetreat(currentRoom)
	currentRoom = null
	$CombatPanel.visible = false
	showMapAndUI()
	
signal playerClassRequested
func _on_player_class_requested(emitter) :
	emit_signal("playerClassRequested", emitter)
	
func getTypicalEnemyDefense(myFloor : int) :
	return $MapContainer.get_child(myFloor).getTypicalEnemyDefense()
	
signal newFloorCompleted
func _on_map_completed(emitter) :
	currentRoom = null
	$CombatPanel.pauseCombat()
	$CombatPanel.visible = false
	showMapAndUI()
	if (emitter.has_method("getEnvironment")) :
		Helpers.handleBiomeAchievement(emitter.getEnvironment())
	var completedIndex = Helpers.findIndexInContainer($MapContainer, emitter)
	if (completedIndex == maxFloor) :
		## The only boss that starts with Hell Knight is the final boss
		if (currentFloor.has_method("getBossName") && currentFloor.getBossName() != "Hell Knight") :
			var outroText = "As the " + currentFloor.getBossName() + " falls, the path forward opens."
			var title = "Victory!"
			var button = "Descend"
			launchNarrative(title, outroText, button, false, false)
		maxFloor += 1
		$FloorDisplay.setMaxFloor(maxFloor)
		var typicalEnemyDefense = emitter.getTypicalEnemyDefense()
		createNewFloor()
		if (narrativeWorking) :
			await $NarrativePanel.continueSignal
		_on_floor_display_floor_down()
		if (currentFloor.has_method("getEnvironment")) :
			var environment : MyEnvironment = currentFloor.getEnvironment()
			var title = environment.getName()
			var myText = environment.introText
			var button = "Continue"
			await launchNarrative(title, myText, button, true, true)
		emit_signal("newFloorCompleted", typicalEnemyDefense)
	if ($MapContainer.get_child_count() >= 2 && emitter == $MapContainer.get_child(1)) :
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.floor1, Vector2(0,0))
	elif ($MapContainer.get_child_count() >= 3 && emitter == $MapContainer.get_child(2)) :
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.floor2, Vector2(0,0))

var narrativeWorking : bool = false
func launchNarrative(title : String, myText : String, buttonText : String, waitToFinish : bool, isEnvironmentIntro : bool) :
	narrativeWorking = true
	$NarrativePanel.setTitle(title)
	$NarrativePanel.setText(myText)
	$NarrativePanel.setButtonText(buttonText)
	hideMapAndUI()
	$NarrativePanel.visible = true
	var factions = $NarrativePanel/VBoxContainer/VBoxContainer/FactionSymbol
	var elements = $NarrativePanel/VBoxContainer/VBoxContainer/Elements
	if (isEnvironmentIntro) :
		factions.visible = true
		elements.visible = true
		factions.setEnvironment(currentFloor.getEnvironment())
		elements.setEnvironment(currentFloor.getEnvironment())
	else :
		factions.visible = false
		elements.visible = false
	if (waitToFinish) :
		await $NarrativePanel.continueSignal
	
func _on_narrative_panel_complete() :
	$NarrativePanel.visible = false
	showMapAndUI()
	narrativeWorking = false

const mapLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/Map_Runtime/combat_map_template_runtime.tscn")
func createNewFloor() :
	var mapData : MapData = $ProceduralGenerationLogic.createMap()
	var newFloor = mapLoader.instantiate()
	$MapContainer.add_child(newFloor)
	if (!newFloor.myReady) :
		await newFloor.myReadySignal
	await newFloor.initialise(mapData)
	newFloor.beforeLoad()
	newFloor.name = "Floor" + str($MapContainer.get_child_count()-1)
	connectToMapSignals(newFloor)
	
func _on_floor_display_floor_up() -> void:
	var currentFloorIndex = Helpers.findIndexInContainer($MapContainer, currentFloor)
	if (currentFloorIndex != null && currentFloorIndex != 0) :
		currentFloor.visible = false
		currentFloor = $MapContainer.get_child(currentFloorIndex-1)
		currentFloor.visible = true
		$FloorDisplay.setFloor(currentFloorIndex-1)
	if (currentFloor.has_method("getEnvironment")) :
		$FloorDisplay.setEnvironment(currentFloor.getEnvironment())
	else :
		$FloorDisplay.setTutorialBiome()

func _on_floor_display_floor_down() -> void:
	var currentFloorIndex = Helpers.findIndexInContainer($MapContainer, currentFloor)
	if (currentFloorIndex != null && currentFloorIndex != maxFloor) :
		currentFloor.visible = false
		currentFloor = $MapContainer.get_child(currentFloorIndex+1)
		currentFloor.visible = true
		$FloorDisplay.setFloor(currentFloorIndex+1)
	if (currentFloor.has_method("getEnvironment")) :
		$FloorDisplay.setEnvironment(currentFloor.getEnvironment())
	else :
		$FloorDisplay.setTutorialBiome()
		
signal addChildRequested
func _on_add_child_requested(emitter, node : Node) :
	emit_signal("addChildRequested",emitter, node)
		
signal routineUnlockRequested
func _on_routine_unlock_requested(emitter, routine : AttributeTraining) :
	emit_signal("routineUnlockRequested", emitter, routine)
	
signal shopRequested
func _on_shop_requested(details) :
	emit_signal("shopRequested", details)
	
func showMapAndUI() :
	$MapContainer.visible = true
	enableUI()
func hideMapAndUI() :
	$MapContainer.visible = false
	disableUI()
func disableUI() :
	if (currentFloor != null) :
		currentFloor.disableUI()
	$FloorDisplay.visible = false
func enableUI() :
	currentFloor.enableUI()
	$FloorDisplay.visible = true
func enableUIIfPaused() :
	if ($CombatPanel.paused) :
		enableUI()

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
	
func connectToMapSignals(map : Node) :
	if (map.has_signal("levelChosen")) :
		map.connect("levelChosen", _on_level_chosen)
	if (map.has_signal("playerClassRequested")) :
		map.connect("playerClassRequested", _on_player_class_requested)
	if (map.has_signal("tutorialRequested")) :
		map.connect("tutorialRequested", _on_tutorial_requested)
	if (map.has_signal("routineUnlockRequested")) :
		map.connect("routineUnlockRequested", _on_routine_unlock_requested)
	if (map.has_signal("shopRequested")) :
		map.connect("shopRequested", _on_shop_requested)
	if (map.has_signal("addChildRequested")) :
		map.connect("addChildRequested", _on_add_child_requested)
	if (map.has_signal("apophisKilled")) :
		map.connect("apophisKilled", _on_apophis_killed)
	map.connect("mapCompleted", _on_map_completed)
	
signal apophisKilled
func _on_apophis_killed() :
	emit_signal("apophisKilled")

const combatRewardsLoader = preload("res://Screens/GameScreen/Tabs/Combat/CombatRewards/combat_rewards.tscn")
var firstReward : bool = true
func handleCombatRewards(rewards : Dictionary) :
	var nullify : bool = rewards["equipment"].is_empty()
	for index in range(0, rewards["currency"].size()) :
		nullify = nullify && rewards["currency"][index] == 0
	if (nullify) :
		return
	if (firstReward) :
		firstReward = false
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.equipment, Vector2(0,0))
	var rewardHandler = combatRewardsLoader.instantiate()
	add_child(rewardHandler)
	rewardHandler.connect("addToInventoryRequested", _on_add_to_inventory_request)
	rewardHandler.connect("waitingForUser", _on_reward_pending)
	rewardHandler.connect("itemListForYourInspectionGoodSir", _item_list_inspection)
	rewardHandler.connect("currentlyEquippedItemRequested", _on_currently_equipped_item_requested)
	rewardHandler.initialise(rewards)
	#if (rewardHandler.initialisationPending) :
		#await rewardHandler.initialisationComplete
	if (!rewardHandler.isFinished()) :
		await rewardHandler.finished
		
signal itemListForYourInspectionGoodSir
func _item_list_inspection(val, val2) :
	emit_signal("itemListForYourInspectionGoodSir", val, val2)
	
signal currentlyEquippedItemRequested
func _on_currently_equipped_item_requested(emitter, type) :
	emit_signal("currentlyEquippedItemRequested", emitter, type)
		
signal rewardPending
func _on_reward_pending() :
	emit_signal("rewardPending")

signal addToInventoryRequested
func _on_add_to_inventory_request(itemSceneRef) :
	emit_signal("addToInventoryRequested", itemSceneRef)

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
	tempDict["maps"] = []
	for map in $MapContainer.get_children() :
		tempDict["maps"].append(map.getSaveDictionary())
	return tempDict
	
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	for map in $MapContainer.get_children() :
		if(!map.myReady) :
			await map.myReadySignal
	myReady = true
	emit_signal("myReadySignal")
	
func beforeLoad(newGame) :
	myReady = false
	disableUI()
	$MapContainer.visible = false
	$NarrativePanel.connect("continueSignal", _on_narrative_panel_complete)
	for map in $MapContainer.get_children() :
		if (!map.is_in_group("Saveable")) :
			map.beforeLoad(newGame)
		connectToMapSignals(map)
	friendlyParty.resize(1)
	if (newGame) :
		currentFloor = $MapContainer.get_child(0)
		$FloorDisplay.setFloor(0)
		$FloorDisplay.setTutorialBiome()
		$MapContainer.visible = true
		currentFloor.visible = true
		enableUI()
	myReady = true
	emit_signal("myReadySignal")
	if (newGame) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
		
func onLoad(loadDict : Dictionary) :
	myReady = false
	var hardMapCount = $MapContainer.get_child_count()
	for index in range(0, loadDict["maps"].size()) :
		if (index < hardMapCount) :
			pass
			#$MapContainer.get_child(index).onLoad(loadDict["maps"][index])
		else :
			var newMap = mapLoader.instantiate()
			$MapContainer.add_child(newMap)
			newMap.visible = false
			if (!newMap.myReady) :
				await newMap.myReadySignal
			newMap.initialise(loadDict["maps"][index])
	var maps = $MapContainer.get_children()
	for index in range(0, maps.size()) :
		var child : Control = maps[index]
		if (child.has_signal("fullyInitialisedSignal") && !child.fullyInitialised) :
			await child.fullyInitialisedSignal
		if (child.has_signal("fullyInitialisedSignal")) :
			child.beforeLoad()
			child.onLoad(loadDict["maps"][index])
			connectToMapSignals(child)
		
	firstReward = loadDict["firstReward"]
	maxFloor = loadDict["maxFloor"]
	$FloorDisplay.setMaxFloor(maxFloor)
	var currentFloorIndex = loadDict["currentFloorIndex"]
	if (currentFloorIndex is String && currentFloorIndex == "null") :
		currentFloor = $MapContainer.get_child(0)
		$FloorDisplay.setFloor(0)
		$FloorDisplay.setTutorialBiome()
	else :
		currentFloor = $MapContainer.get_child(currentFloorIndex)
		$FloorDisplay.setFloor(currentFloorIndex)
		if (currentFloorIndex == 0) :
			$FloorDisplay.setTutorialBiome()
		else : 
			$FloorDisplay.setEnvironment(currentFloor.getEnvironment())
	enableUI()
	$MapContainer.visible = true
	currentFloor.visible = true
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")

func onLoad_2() :
	$CanvasLayer.offset = global_position


signal playerModifierDictionaryRequested
func _on_combat_panel_player_modifier_dictionary_requested(emitter) -> void:
	emit_signal("playerModifierDictionaryRequested", emitter)

signal playerSubclassRequested
func _on_combat_panel_player_subclass_requested(emitter) -> void:
	emit_signal("playerSubclassRequested", emitter)
	
signal weaponResourceRequested
func _on_combat_panel_weapon_resource_requested(emitter) -> void:
	emit_signal("weaponResourceRequested", emitter)

signal shopShortcutSelected
func _on_routine_button_was_selected(_emitter) -> void:
	emit_signal("shopShortcutSelected", "routine")
func _on_armor_button_was_selected(_emitter) -> void:
	emit_signal("shopShortcutSelected", "armor")
func _on_weapon_button_was_selected(_emitter) -> void:
	emit_signal("shopShortcutSelected", "weapon")
func _on_soul_button_was_selected(_emitter) -> void:
	emit_signal("shopShortcutSelected", "soul")
func enableShopShortcut(val : String) :
	var buttons = $CanvasLayer/ShopShortcuts.get_children()
	var index
	if (val == "routine") :
		index = 0
	elif (val == "armor") :
		index = 1
	elif (val == "weapon") :
		index = 2
	elif (val == "soul") :
		index = 3
	else :
		return
	buttons[index].visible = true


func _on_visibility_changed() -> void:
	$CanvasLayer.visible = self.is_visible_in_tree()


func _on_resized() -> void:
	onLoad_2()
