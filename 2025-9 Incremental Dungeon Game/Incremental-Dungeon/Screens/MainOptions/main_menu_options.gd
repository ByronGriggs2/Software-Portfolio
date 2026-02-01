extends Control

signal swapToMainMenu

func _on_return_button_pressed() -> void:
	$Content/VBoxContainer/ScrollContainer/VBoxContainer/masterVolume.onExit()
	emit_signal("swapToMainMenu")

var currentSettings : Dictionary = {}
func _ready() :
	##Main is responsible for creating the file (using getDefaultSettings)
	currentSettings = MainOptionsHelpers.loadSettings()
	
	getWindowMode().selected = currentSettings["Window Mode"]
	oldWindowMode = currentSettings["Window Mode"]
	
func getWindowMode() :
	return $Content/VBoxContainer/ScrollContainer/VBoxContainer/WindowMode/WindowMode

const popupLoader = preload("res://Graphic Elements/popups/my_popup_button.tscn")
var oldWindowMode = 0
var windowModeConfirmationPopup : Node = null
var windowModeTimer : Timer = null
var windowModeSubTimer : Timer = null
func _on_window_mode_item_selected(index: int) -> void:
	currentSettings["Window Mode"] = index
	MainOptionsHelpers.applyWindowMode(index)
	windowModeConfirmationPopup = popupLoader.instantiate()
	add_child(windowModeConfirmationPopup)
	windowModeConfirmationPopup.setTitle("Confirm Window Mode")
	windowModeConfirmationPopup.setText("Is this Window Mode okay? Reverting in 10 seconds.")
	windowModeConfirmationPopup.setButtonText("Confirm and save setting")
	windowModeConfirmationPopup.connect("finished", _on_window_mode_confirmed)
	windowModeTimer = Timer.new()
	add_child(windowModeTimer)
	windowModeTimer.one_shot = true
	windowModeTimer.wait_time = 10
	windowModeTimer.connect("timeout", _revert_window_mode)
	windowModeTimer.start()
	windowModeSubTimer = Timer.new()
	add_child(windowModeSubTimer)
	windowModeSubTimer.wait_time = 1
	windowModeSubTimer.connect("timeout", _update_window_mode_text)
	windowModeSubTimer.start()
	
func killWindowModeChildren() :
	windowModeConfirmationPopup = null
	windowModeTimer.stop()
	windowModeTimer.queue_free()
	windowModeTimer = null
	windowModeSubTimer.stop()
	windowModeSubTimer.queue_free()
	windowModeSubTimer = null

func _on_window_mode_confirmed() :
	killWindowModeChildren()
	var oldSettings = MainOptionsHelpers.loadSettings()
	oldSettings["Window Mode"] = currentSettings["Window Mode"]
	MainOptionsHelpers.saveSettings(oldSettings)
	oldWindowMode = currentSettings["Window Mode"]

func _revert_window_mode() :
	windowModeConfirmationPopup.queue_free()
	killWindowModeChildren()
	MainOptionsHelpers.applyWindowMode(oldWindowMode)
	currentSettings["Window Mode"] = oldWindowMode

func _update_window_mode_text() :
	windowModeConfirmationPopup.setText("Is this Window Mode okay? Reverting in " + str(Helpers.myRound(windowModeTimer.time_left,1)) + " seconds.")

func _on_save_pressed() -> void:
	#$Content/VBoxContainer/ScrollContainer/VBoxContainer/masterVolume.onSave()
	currentSettings["audio"] = AudioHandler.getMainOptionsDictionary()
	MainOptionsHelpers.queueSaveSettings(currentSettings)

const keybindsLoader = preload("res://Screens/MainOptions/keybinds.tscn")
func _on_keybinds_pressed() -> void:
	var keybinds = keybindsLoader.instantiate()
	add_child(keybinds)

func _unhandled_input(event: InputEvent) -> void:
	if (event.is_action("ui_cancel")) :
		accept_event()
		emit_signal("swapToMainMenu")

const popupLoader2 = preload("res://Graphic Elements/popups/binary_decision.tscn")
func _on_reset_global_encyclopedia_pressed() -> void:
	var popup = popupLoader2.instantiate()
	add_child(popup)
	popup.setTitle("Reset Global Encyclopedia")
	popup.setText("The Global Encyclopedia is the record of encountered enemies and items maintained across all save files, and is used for achievements. It can be toggled in the in-game options.\n\nResetting this encyclopedia will allow you to experience the game anew, but erase all progress towards the 2 100% encyclopedia achievements.")
	popup.setButton0Name(" I know what I'm doing ")
	popup.setButton1Name(" Sorry ma'am I thought this was a W****'s ")
	var choice = await popup.binaryChosen
	if (choice == 0) :
		var current = MainOptionsHelpers.loadSettings()
		var default = MainOptionsHelpers.getDefaultSettings()
		current["globalEncyclopedia"] = default["globalEncyclopedia"]
		current["herophile"] = default["herophile"]
		MainOptionsHelpers.saveSettings(current)
		SaveManager.globalSettings = current.duplicate()
