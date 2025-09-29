extends Node

var currentScreen : Node = null

func _ready() -> void :
	randomize()
	swapToMenu()
	
####Screen Swap Methods
func swapScreen(scenePath : String) -> void :
	if currentScreen :
		currentScreen.queue_free()
		currentScreen = null
	var screenLoader = load(scenePath)
	currentScreen = screenLoader.instantiate()
	add_child(currentScreen)

func swapToMenu() -> void :
	swapScreen("Screens/main_menu.tscn")
	currentScreen.connect("newGame", _onNewGame)
	currentScreen.connect("loadGame", _onLoadGame)
	currentScreen.connect("swapToMainMenuOptions", _onSwapToMainMenuOptions)
	
func swapToGame() -> void :
	swapScreen("Screens/GameScreen/game_screen.tscn")
	currentScreen.connect("exitToMenu", swapToMenu)

func _onNewGame() :
	swapScreen("Screens/IntroScreen/intro_screen.tscn")
	currentScreen.connect("characterDone", _onIntroEnd)
func _onIntroEnd(character : CharacterClass, characterName : String) :
	swapToGame()
	#Gives ownership of class struct to Player
	currentScreen.setPlayerClass(character)
	currentScreen.setPlayerName(characterName)
func _onLoadGame() :
	swapToGame()
	SaveManager.loadGame()
func _onSwapToMainMenuOptions() :
	swapScreen("Screens/main_menu_options.tscn")
	currentScreen.connect("swapToMainMenu", swapToMenu)
##############################

func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var key : String = "Version"
	tempDict[key] = Definitions.currentVersion
	return tempDict

func setFromSaveDictionary(_loadDict) -> void :
	pass
