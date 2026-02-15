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
	var problemSlot = SaveManager.validateSaves()
	if (problemSlot != -1) :
		handleInvalid(problemSlot)
	
const popupLoader = preload("res://Graphic Elements/popups/my_popup_button.tscn")	
func handleInvalid(slot : int) :
	$ButtonContainer.get_node("LoadButton").set_disabled(true)
	$ButtonContainer.get_node("NewButton").set_disabled(true)
	$ButtonContainer.get_node("OptionsButton").set_disabled(true)
	var popup = popupLoader.instantiate()
	add_child(popup)
	popup.getWindowRef().custom_minimum_size.x += 300
	popup.setTitle("Invalid Save Data")
	popup.setText("Invalid save data has been detected in save slot " + str(slot+1) + ". Play has been disabled to avoid further corrupting your save file. Please contact the developer for support.\n\nAlternatively, you could just delete or move the file and relaunch. It's located in\nC:\\Users\\<username>\\AppData\\Roaming\\Godot\\app_userdata\\Incremental Dungeon.\nThe filename will be 1 less than the in-game name. Example: Save Slot 1 = \"save_slot_0.json\"")
	popup.setButtonText("Ok")

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
		

const creditsLoader = preload("res://Graphic Elements/popups/credits.tscn")
func _on_credits_button_pressed() -> void:
	var credits = creditsLoader.instantiate()
	add_child(credits)
