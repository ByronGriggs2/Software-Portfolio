extends Control

signal newGame
signal loadGame
signal swapToMainMenuOptions

var hasSavedGame : bool = false #placeholder

func _ready() :
	if (!hasSavedGame) :
		$ButtonContainer.get_node("LoadButton").set_disabled(true)


func _on_new_button_pressed() -> void:
	emit_signal("newGame")

func _on_load_button_pressed() -> void: #may want a saved game dialogue first
	emit_signal("loadGame")

func _on_quit_button_pressed() -> void:
	get_tree().quit()

func _on_options_button_pressed() -> void:
	emit_signal("swapToMainMenuOptions")
	
