extends Node

var currentScreen : Node = null

####Screen Swap Methods
func swapScreen(scenePath : String) -> void :
	if currentScreen :
		currentScreen.process_mode = Node.PROCESS_MODE_DISABLED
		await(get_tree().process_frame)
		currentScreen.queue_free()
		currentScreen = null
		await(get_tree().process_frame)
	var screenLoader = load(scenePath)
	currentScreen = screenLoader.instantiate()
	add_child(currentScreen)

func swapToMenu() -> void :
	await swapScreen("Screens/main_menu.tscn")
	currentScreen.connect("newGame", _onNewGame)
	currentScreen.connect("loadGame", _onLoadGame)
	currentScreen.connect("swapToMainMenuOptions", _onSwapToMainMenuOptions)
	
func swapToGame() -> void :
	await swapScreen("Screens/GameScreen/game_screen.tscn")
	currentScreen.connect("exitToMenu", swapToMenu)
	currentScreen.connect("loadGameNow", _onLoadGame)

func _onNewGame() :
	await swapScreen("Screens/IntroScreen/intro_screen.tscn")
	currentScreen.connect("characterDone", _onIntroEnd)
	
func _onIntroEnd(character : CharacterClass, characterName : String) :
	await swapToGame()
	#Gives ownership of class struct to Player
	currentScreen.game_screen_fresh_save_init(character, characterName)
	SaveManager.newGame()
	
func _onLoadGame() :
	await swapToGame()
	SaveManager.loadGame(Definitions.saveSlots.current)
	
func _onSwapToMainMenuOptions() :
	await swapScreen("Screens/main_menu_options.tscn")
	currentScreen.connect("swapToMainMenu", swapToMenu)
##############################

func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var key : String = "Version"
	tempDict[key] = Definitions.currentVersion
	return tempDict
	
var myReady : bool = false
func _ready() : 
	randomize()
	swapToMenu()
	myReady = true

func beforeLoad(_newSave) :
	pass
func onLoad(_loadDict) :
	pass
func afterLoad() :
	pass
	
