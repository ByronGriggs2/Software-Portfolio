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

func _onNewGame() :
	swapScreen("Screens/intro_screen.tscn")
	currentScreen.connect("characterDone", _onIntroEnd)
func _onIntroEnd(character : CharacterClass) :
	swapScreen("Screens/game_screen.tscn")
	currentScreen.initialStartup(character)
func _onLoadGame() :
	pass
func _onSwapToMainMenuOptions() :
	swapScreen("Screens/main_menu_options.tscn")
	currentScreen.connect("swapToMainMenu", swapToMenu)
##############################
