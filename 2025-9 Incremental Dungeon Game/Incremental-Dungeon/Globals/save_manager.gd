extends Node
var currentSlot : Definitions.saveSlots

var globalSettings : Dictionary = {}
func getGlobalSettings() -> Dictionary :
	return globalSettings.duplicate(true)
func saveGlobalSettings(newVal : Dictionary) -> void :
	globalSettings = newVal.duplicate(true)

##################################
## Internal
var secondsElapsed : int = 0
var playtimeTimer : Timer = null
func createTimer() :
	if (playtimeTimer != null) :
		playtimeTimer.queue_free()
		playtimeTimer = null
	playtimeTimer = Timer.new()
	add_child(playtimeTimer)
	playtimeTimer.connect("timeout",_on_timer_tick)
	playtimeTimer.start()
	
func pausePlaytime() :
	if (playtimeTimer != null) :
		playtimeTimer.stop()

func fixNullRecursive(loadDict : Dictionary) :
	for key in loadDict.keys() :
		if (loadDict[key] is Dictionary) :
			fixNullRecursive(loadDict[key])
		elif (loadDict[key] == null) :
			loadDict[key] = "null"	

func fixEnumRecursive(loadDict : Dictionary) :
	for key in loadDict.keys() :
		if (loadDict[key] is Dictionary) :
			fixEnumRecursive(loadDict[key])
		elif (key.is_valid_int() && key != null) :
			loadDict[int(key)] = loadDict[key]
			loadDict.erase(key)

func saveExists() -> bool :
	for key in Definitions.slotPaths.keys() :
		if (FileAccess.file_exists(Definitions.slotPaths[key])) :
			return true
	return false
#############################################################
## Saving Sequence

## Every saveable Node must have the following property:
## myReady : bool

## Every saveable Node must have the following methods :
## func beforeLoad(newSave : bool)
## func onLoad(loadDict : Dictionary)

## Ensure all node references are solid
func waitForReady(nodePaths : Array[NodePath]) :
	for nodePath in nodePaths :
		var node = get_node_or_null(nodePath)
		if (!node.myReady) :
			await node.myReadySignal
			
## All initialisation that can be done before loading should be done here to minimise dependency
## issues. This includes initialising values to defaults.
func beforeLoadStep(nodePaths : Array[NodePath], freshSave : bool) :
	waitForReady(nodePaths)
	for nodePath in nodePaths :
		var node = get_node_or_null(nodePath)
		node.beforeLoad(freshSave)

## Every Saveable node that has a dependency on another saveable node must have the following properties :
## myLoadDependencyName : Definitions.loadDependencyName
## myLoadDependencies : Array[Definitions.loadDependencyName]

## Every such Node must have the following method :
## func afterDependencyLoaded(dependency : Definitions.loadDependencyName)
## which will be called once for each of its dependencies

## Assuming the above rules are followed (self enforced, since Godot makes you do everything yourself),
## and there are no circular dependencies, the following function should efficiently and safely load
## any web of Nodes regardless of complexity
func loadStep(gameState : Dictionary) :
	## Handle dependencies
	#var loadedDependencies : Dictionary = {}
	#var problemChildArray : Array[String]
	#for key in gameState.keys() :
		#var node = get_node_or_null(NodePath(key))
		#if (node.has_method("afterDependencyLoaded")) :
			#problemChildArray.append(key)
			#
	#while (!problemChildArray.is_empty()) :
		#for key in problemChildArray :
			#var node = get_node_or_null(NodePath(key))
			#var nodeLoadable : bool = true
			#for dependency in node.myLoadDependencies :
				#if (loadedDependencies.get(dependency) == null) :
					#nodeLoadable = false
					#break
			#if (nodeLoadable) :
				#node.onLoad(gameState[key])
				#loadedDependencies[node.myLoadDependencyName] = true
				#problemChildArray.remove_at(problemChildArray.find(key))
				#for subKey in problemChildArray :
					#get_node_or_null(NodePath(subKey)).afterDependencyLoaded(node.myLoadDependencyName)
	## Load nondependent nodes
	for key in gameState.keys() :
		var node = get_node_or_null(NodePath(key))
		if (!node.myReady) :
			await node.myReadySignal
	for key in gameState.keys() :
		var node = get_node_or_null(NodePath(key))
		node.onLoad(gameState[key])
		

###################################
## Signals
func _on_timer_tick() :
	secondsElapsed += 1
func _on_new_game_option_chosen(slot : Definitions.saveSlots) :
	currentSlot = slot
	emit_signal("newGameReady")
##################################
## Saving
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	await waitForDependencies()
	handleSafetySave()
	globalSettings = MainOptionsHelpers.loadSettings()
	myReady = true
	emit_signal("myReadySignal")
func beforeLoad(_newGame) :
	myReady = true
	emit_signal("myReadySignal")
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	tempDict["playtime"] = Helpers.getTimestampString(secondsElapsed)
	return tempDict
	
func onLoad(loadDict) -> void :
	myReady = false
	secondsElapsed = Helpers.getSecondsFromTimestamp(loadDict["playtime"])
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")
	
func handleSafetySave() :
	for slot in Definitions.slotNames.keys() :
		var saveFile = loadSaveDict(slot)
		if (saveFile == null) :
			continue
		var mainSave = saveFile.get("/root/Main")
		if (mainSave == null) :
			continue
		var oldVersion = mainSave.get("Version")
		if (oldVersion == null) :
			oldVersion = "Pre-V1.05 release"
		if (oldVersion != Definitions.currentVersion) :
			var dir = DirAccess.open("user://")
			var tempPath
			if (slot == Definitions.saveSlots.slot0) :
				tempPath = Definitions.tempSlot
			elif (slot == Definitions.saveSlots.slot1) :
				tempPath = Definitions.tempSlot2
			elif (slot == Definitions.saveSlots.slot2) :
				tempPath = Definitions.tempSlot3
			else :
				tempPath = Definitions.tempSlot4
			if (dir.file_exists(tempPath)) :
				dir.remove_absolute(tempPath)
			dir.copy(Definitions.slotPaths[slot], tempPath)
			dir.rename(tempPath, Helpers.removeAffix(Definitions.slotPaths[slot], ".json") + "_" + oldVersion + ".json")
			
			saveFile["/root/Main"]["Version"] = Definitions.currentVersion
			queueSaveGame_safety(saveFile, Definitions.slotPaths[slot])
##################################
## Other
var saveActive : bool = false
var saveMutex = Mutex.new()
func saveGame(slot : Definitions.saveSlots) :
	saveMutex.lock()
	if (saveActive) :
		saveMutex.unlock()
		return
	saveActive = true
	var data = generateSave()
	var filePath = generateFilepath(slot)
	saveGame_noCheck(data, filePath)
	saveMutex.unlock()
	
func generateSave() -> Dictionary :
	var centralisedGameState = {}
	centralisedGameState[self.get_path()] = self.getSaveDictionary()
	for node in get_tree().get_nodes_in_group("Saveable") :
		centralisedGameState[node.get_path()] = node.getSaveDictionary()
	return centralisedGameState

func generateFilepath(slot : Definitions.saveSlots) :
	var filePath : String
	if (slot == Definitions.saveSlots.current) :
		filePath = Definitions.slotPaths[currentSlot]
	else :
		filePath = Definitions.slotPaths[slot]
	return filePath

func saveGame_noCheck(saveData, filePath) :
	taskList.append(WorkerThreadPool.add_task(func():handleDiskAccess(saveData.duplicate(true), filePath)))

var purging : bool = false
var taskList : Array = []
var purgeMutex = Mutex.new()
func _process(_delta) :
	purgeMutex.lock()
	if (purging) :
		purgeMutex.unlock()
		return
	saveMutex.lock()
	if (taskList.size() > 0 && !saveActive) :
		purging = true
		purgeMutex.unlock()
		for task in taskList :
			WorkerThreadPool.wait_for_task_completion(task)
		taskList = []
		saveMutex.unlock()
		purgeMutex.lock()
		purging = false
		purgeMutex.unlock()
		return
	saveMutex.unlock()
	purgeMutex.unlock()

func handleDiskAccess(centralisedGameState, filePath) :
	var tempFile = FileAccess.open(Definitions.tempSlot, FileAccess.WRITE)
	var toStore = JSON.stringify(centralisedGameState)
	tempFile.store_string(toStore)
	tempFile.close()
	## With this pattern there is no point in time where your save file exists only in RAM.

	var dir = DirAccess.open("user://")
	if (FileAccess.file_exists(Definitions.emergencySlot) && FileAccess.file_exists(filePath)) :
		dir.remove(Definitions.emergencySlot)
	if (FileAccess.file_exists(filePath)) :
		dir.copy(filePath,Definitions.emergencySlot)
	if (FileAccess.file_exists(filePath)) :
		dir.remove(filePath)
	dir.rename(Definitions.tempSlot, filePath)
	saveMutex.lock()
	saveActive = false
	saveMutex.unlock()
	
func loadSaveDict(slot : Definitions.saveSlots) :
	if (slot == Definitions.saveSlots.current) :
		slot = currentSlot
	if (!FileAccess.file_exists(Definitions.slotPaths[slot])) : return null
	var text = FileAccess.open(Definitions.slotPaths[slot], FileAccess.READ).get_as_text()
	var centralisedGameState = JSON.parse_string(text)
	JSON.new().parse(text)
	if (centralisedGameState == null) :
		centralisedGameState = {}
	fixEnumRecursive(centralisedGameState)
	fixNullRecursive(centralisedGameState)
	return centralisedGameState
	
func loadGame(slot : Definitions.saveSlots) :
	var centralisedGameState = loadSaveDict(slot)
	if (centralisedGameState == null) :
		return false
	var nodePaths : Array[NodePath]
	for key in centralisedGameState.keys() :
		if (key != (self.get_path() as String)) :
			nodePaths.append(NodePath(key))
	await waitForReady(nodePaths)
	await beforeLoadStep(nodePaths, false)
	await loadStep(centralisedGameState)
	await postLoad(nodePaths)
	createTimer()
	return true
	
func postLoad(scenes) :
	for key in scenes :
		var node = get_node_or_null(key)
		if (!node.myReady) :
			await node.myReadySignal
	for key in scenes :
		var node = get_node_or_null(NodePath(key))
		if (node.has_method("onLoad_2")) :
			node.onLoad_2()
	
func newGame(gameScreenRef : Node, character : CharacterPacket, hyperMode : bool) :
	var saveableScenes : Array[NodePath]
	for node in get_tree().get_nodes_in_group("Saveable") :
		saveableScenes.append(node.get_path())
	waitForReady(saveableScenes)
	beforeLoadStep(saveableScenes, true)
	gameScreenRef.initialisePlayerCharacter(character)
	postLoad(saveableScenes)
	secondsElapsed = 0
	createTimer()
	var dict = IGOptions.getIGOptionsCopy()
	dict["hyperMode"] = hyperMode
	IGOptions.saveIGOptionsNoUpdate(dict)
	
const loadGameMenu = preload("res://SaveManager/load_game_menu.tscn")
signal loadRequested
func loadGameSaveSelection(parent : Node) :
	var menu = loadGameMenu.instantiate()
	parent.add_child(menu)
	if (parent.has_method("nestedPopupInit")) :
		menu.nestedPopupInit(parent)
	menu.connect("loadRequested", _on_load_menu_option_chosen)
	menu.connect("finished", _on_finished)
	return menu
func _on_load_menu_option_chosen(slot : Definitions.saveSlots) :
	currentSlot = slot
	emit_signal("loadRequested")
signal finished
func _on_finished() :
	emit_signal("finished")

const saveGameMenu = preload("res://SaveManager/save_game_menu.tscn")
func saveGameSaveSelection(parent : Node) :
	var menu = saveGameMenu.instantiate()
	parent.add_child(menu)
	menu.connect("optionChosen", _on_save_menu_option_chosen)
	if (parent.has_method("nestedPopupInit")) :
		menu.nestedPopupInit(parent)
func _on_save_menu_option_chosen(slot : Definitions.saveSlots) :
	queueSaveGame(slot)
	
signal newGameReady
const newGameMenu = preload("res://SaveManager/new_game_menu.tscn")
func newGameSaveSelection() :
	var menu = newGameMenu.instantiate()
	add_child(menu)
	menu.connect("optionChosen", _on_new_game_option_chosen)
	return menu

func waitForDependencies() :
	if (!MainOptionsHelpers.myReady) :
		await MainOptionsHelpers.myReadySignal

func queueSaveGame(slot) :
	while (true) :
		saveMutex.lock()
		if (!saveActive) :
			break
		saveMutex.unlock()
		await get_tree().process_frame
	saveActive = true
	var data = generateSave()
	var filePath = generateFilepath(slot)
	saveGame_noCheck(data, filePath)
	saveMutex.unlock()
	print("queued save success")
	
func queueSaveGame_safety(saveData : Dictionary, filePath : String) :
	while (true) :
		saveMutex.lock()
		if (!saveActive) :
			break
		saveMutex.unlock()
		await get_tree().process_frame
	saveActive = true
	saveGame_noCheck(saveData, filePath)
	saveMutex.unlock()
	
func queueSaveGlobalSettings_immediate(settings) :
	MainOptionsHelpers.queueSaveSettings(settings)
