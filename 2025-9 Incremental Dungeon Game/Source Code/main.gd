extends Node

var currentScreen : Node = null
const gameScreenLoader = preload("res://Screens/GameScreen/game_screen.tscn")
const mainMenuLoader = preload("res://Screens/main_menu.tscn")
const optionsMenuLoader = preload("res://Screens/MainOptions/main_menu_options.tscn")
const introScreenLoader = preload("res://Screens/IntroScreen/intro_screen.tscn")

####Screen Swap Methods
func swapScreen(screenLoader) -> void :
	SaveManager.pausePlaytime()
	if currentScreen :
		currentScreen.process_mode = Node.PROCESS_MODE_DISABLED
		await(get_tree().process_frame)
		#await RenderingServer.frame_post_draw
		currentScreen.queue_free()
		await currentScreen.tree_exited
		currentScreen = null
		#await(get_tree().process_frame)
	currentScreen = screenLoader.instantiate()
	add_child(currentScreen)

signal mainMenuLoaded
func swapToMenu() -> void :
	await swapScreen(mainMenuLoader)
	currentScreen.connect("newGame", _onNewGame)
	currentScreen.connect("loadGame", _onLoadGame)
	currentScreen.connect("swapToMainMenuOptions", _onSwapToMainMenuOptions)
	emit_signal("mainMenuLoaded")
	
signal gameLoaded
func swapToGame() -> void :
	await swapScreen(gameScreenLoader)
	currentScreen.connect("exitToMenu", swapToMenu)
	currentScreen.connect("loadGameNow", _onLoadGame)
	emit_signal("gameLoaded")

func _onNewGame() :
	await swapScreen(introScreenLoader)
	currentScreen.connect("characterDone", _onIntroEnd)
	currentScreen.connect("cancel", _on_intro_cancel)
	
func _on_intro_cancel() :
	await swapToMenu()
	
func _onIntroEnd(character : CharacterPacket, hyperMode : bool) :
	await swapToGame()
	while(!currentScreen.myReady) : 
		await get_tree().process_frame
	#Gives ownership of class struct to Player
	SaveManager.newGame(currentScreen, character, hyperMode)
	
func _onLoadGame() :
	await swapToGame()
	SaveManager.loadGame(Definitions.saveSlots.current)
	
func _onSwapToMainMenuOptions() :
	await swapScreen(optionsMenuLoader)
	currentScreen.connect("swapToMainMenu", swapToMenu)
	
func mainSettingsInit() :
	var settings
	if (!FileAccess.file_exists(Definitions.mainSettingsPath)) :
		var saveFile = FileAccess.open(Definitions.mainSettingsPath, FileAccess.WRITE)
		settings = MainOptionsHelpers.getDefaultSettings()
		saveFile.store_string(JSON.stringify(settings))
		saveFile.close()
	else :
		settings = MainOptionsHelpers.loadSettings()
	MainOptionsHelpers.applyWindowMode(settings["Window Mode"])
##############################

func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var key : String = "Version"
	tempDict[key] = Definitions.currentVersion
	return tempDict

#const cursor = preload("res://Images/mouse_2.png")
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() : 
	Input.set_use_accumulated_input(false)
	#Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
	#Input.set_custom_mouse_cursor(cursor, Input.CURSOR_ARROW)
	#Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	mainSettingsInit()
	randomize()
	swapToMenu()
	connect("mainMenuLoaded", AudioHandler._on_main_menu_loaded)
	connect("gameLoaded", AudioHandler._on_game_loaded)
	myReady = true
	emit_signal("myReadySignal")

func beforeLoad(newSave) :
	myReady = true
	emit_signal("myReadySignal")
	if (newSave) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
func onLoad(_loadDict) :
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")
