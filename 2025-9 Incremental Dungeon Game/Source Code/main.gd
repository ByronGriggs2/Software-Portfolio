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
		currentScreen.queue_free()
		currentScreen = null
		await(get_tree().process_frame)
	currentScreen = screenLoader.instantiate()
	add_child(currentScreen)

func swapToMenu() -> void :
	await swapScreen(mainMenuLoader)
	currentScreen.connect("newGame", _onNewGame)
	currentScreen.connect("loadGame", _onLoadGame)
	currentScreen.connect("swapToMainMenuOptions", _onSwapToMainMenuOptions)
	
func swapToGame() -> void :
	await swapScreen(gameScreenLoader)
	currentScreen.connect("exitToMenu", swapToMenu)
	currentScreen.connect("loadGameNow", _onLoadGame)

func _onNewGame() :
	await swapScreen(introScreenLoader)
	currentScreen.connect("characterDone", _onIntroEnd)
	currentScreen.connect("cancel", _on_intro_cancel)
	
func _on_intro_cancel() :
	await swapToMenu()
	
func _onIntroEnd(character : CharacterClass, characterName : String) :
	await swapToGame()
	while(!currentScreen.myReady) : 
		await get_tree().process_frame
	#Gives ownership of class struct to Player
	SaveManager.newGame(currentScreen, character, characterName)
	
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
	
var myReady : bool = false
func _ready() : 
	mainSettingsInit()
	randomize()
	swapToMenu()
	myReady = true

func beforeLoad(_newSave) :
	pass
func onLoad(_loadDict) :
	pass
