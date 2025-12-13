extends Panel

signal newGame
signal loadGame
signal swapToMainMenuOptions

func _ready() :
	checkLoadGame()
	SaveManager.connect("newGameReady", _new_game_ready)
	SaveManager.connect("loadRequested", _load_game_ready)
	
func checkLoadGame() :
	if (!SaveManager.saveExists()) :
		$ButtonContainer.get_node("LoadButton").set_disabled(true)

func _on_new_button_pressed() -> void:
	SaveManager.newGameSaveSelection()
	
func _new_game_ready() :
	emit_signal("newGame")

func _on_load_button_pressed() -> void:
	SaveManager.loadGameSaveSelection(self)
	await SaveManager.finished
	checkLoadGame()
	
func _load_game_ready() :
	emit_signal("loadGame")

func _on_quit_button_pressed() -> void:
	get_tree().quit()

func _on_options_button_pressed() -> void:
	emit_signal("swapToMainMenuOptions")
	
