extends Node
var currentSlot : Definitions.saveSlots

func saveGame(slot : Definitions.saveSlots) :
	var filePath : String
	if (slot == Definitions.saveSlots.current) :
		filePath = Definitions.slotPaths[currentSlot]
	else :
		filePath = Definitions.slotPaths[slot]
		
	var centralisedGameState = {}
	for node in get_tree().get_nodes_in_group("Saveable") :
		centralisedGameState[node.get_path()] = node.getSaveDictionary()
	var saveFile = FileAccess.open(filePath, FileAccess.WRITE)
	saveFile.store_string(JSON.stringify(centralisedGameState))
	saveFile.close()
	
func loadGame(slot : Definitions.saveSlots) :
	if (slot == Definitions.saveSlots.current) :
		slot = currentSlot
	if (!FileAccess.file_exists(Definitions.slotPaths[slot])) : return false
	var text = FileAccess.open(Definitions.slotPaths[slot], FileAccess.READ).get_as_text()
	var centralisedGameState = JSON.parse_string(text)
	fixNullRecursive(centralisedGameState)
	var nodePaths : Array[NodePath]
	for key in centralisedGameState.keys() :
		nodePaths.append(NodePath(key))
	waitForReady(nodePaths)
	beforeLoadStep(nodePaths, false)
	loadStep(centralisedGameState)
	afterLoadStep(nodePaths)
	
func fixNullRecursive(loadDict : Dictionary) :
	for key in loadDict.keys() :
		if (loadDict[key] is Dictionary) :
			fixNullRecursive(loadDict[key])
		elif (loadDict[key] == null) :
			loadDict[key] = "null"	

func newGame() :
	var saveableScenes : Array[NodePath]
	for node in get_tree().get_nodes_in_group("Saveable") :
		saveableScenes.append(node.get_path())
	waitForReady(saveableScenes)
	beforeLoadStep(saveableScenes, true)
	afterLoadStep(saveableScenes)
	
func waitForReady(nodePaths : Array[NodePath]) :
	for nodePath in nodePaths :
		var node = get_node_or_null(nodePath)
		while(node == null) :
			await get_tree().process_frame
			node = get_node_or_null(nodePath)
		while(!node.myReady) :
			await get_tree().process_frame
			
func beforeLoadStep(nodePaths : Array[NodePath], freshSave : bool) :
	for nodePath in nodePaths :
		var node = get_node_or_null(nodePath)
		node.beforeLoad(freshSave)
		
func loadStep(gameState : Dictionary) :
	var nodePaths : Array[NodePath]
	for key in gameState.keys() :
		nodePaths.append(NodePath(key))
	for nodePath in nodePaths :
		var node = get_node_or_null(nodePath)
		node.onLoad(gameState[nodePath as String])
		
func afterLoadStep(nodePaths : Array[NodePath]) :
	for nodePath in nodePaths :
		var node = get_node_or_null(nodePath)
		node.afterLoad()

const loadGameMenu = preload("res://SaveManager/load_game_menu.tscn")
signal loadGameReady
func loadGameSaveSelection() :
	var menu = loadGameMenu.instantiate()
	add_child(menu)
	menu.connect("optionChosen", _on_load_menu_option_chosen)
func _on_load_menu_option_chosen(slot : Definitions.saveSlots) :
	currentSlot = slot
	emit_signal("loadGameReady")

const saveGameMenu = preload("res://SaveManager/save_game_menu.tscn")
func saveGameSaveSelection() :
	var menu = saveGameMenu.instantiate()
	add_child(menu)
	menu.connect("optionChosen", _on_save_menu_option_chosen)
func _on_save_menu_option_chosen(slot : Definitions.saveSlots) :
	saveGame(slot)
	
signal newGameReady
const newGameMenu = preload("res://SaveManager/new_game_menu.tscn")
func newGameSaveSelection() :
	var menu = newGameMenu.instantiate()
	add_child(menu)
	menu.connect("optionChosen", _on_new_game_option_chosen)
func _on_new_game_option_chosen(slot : Definitions.saveSlots) :
	currentSlot = slot
	emit_signal("newGameReady")
	
func saveExists() -> bool :
	for key in Definitions.slotPaths.keys() :
		if (FileAccess.file_exists(Definitions.slotPaths[key])) :
			return true
	return false
