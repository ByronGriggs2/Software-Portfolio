extends Panel

signal newGame
signal loadGame
signal swapToMainMenuOptions

func _ready() :
	checkLoadGame()
	if (Definitions.hasDLC) :
		$DLC.visible = true
	SaveManager.connect("newGameReady", _new_game_ready)
	SaveManager.connect("loadRequested", _load_game_ready)
	
func checkLoadGame() :
	if (!SaveManager.saveExists()) :
		$ButtonContainer.get_node("LoadButton").set_disabled(true)

var newMenu : Node = null
func _on_new_button_pressed() -> void:
	newMenu = SaveManager.newGameSaveSelection()
	newMenu.connect("optionChosen", _on_new_menu_finished)
	
func _on_new_menu_finished(_val) :
	newMenu = null
	
func _new_game_ready() :
	emit_signal("newGame")

var loadMenu : Node = null
func _on_load_button_pressed() -> void:
	loadMenu = SaveManager.loadGameSaveSelection(self)
	loadMenu.connect("finished", _on_load_menu_finished)
	await SaveManager.finished
	checkLoadGame()
	
func _load_game_ready() :
	emit_signal("loadGame")

func _on_quit_button_pressed() -> void:
	get_tree().quit()

func _on_options_button_pressed() -> void:
	emit_signal("swapToMainMenuOptions")
	
func _on_load_menu_finished() :
	loadMenu = null
	
func _unhandled_input(event : InputEvent) :
	if (event.is_action_released("ui_cancel")) :
		accept_event()
		if (loadMenu != null) :
			loadMenu.queue_free()
			loadMenu = null
		if (newMenu != null) :
			newMenu.queue_free()
			newMenu = null
		
