extends "res://Graphic Elements/popups/my_popup.gd"

signal exitToMenu

func _ready() :
	SaveManager.connect("loadRequested", _on_load_ready)
	
signal loadGameNow
func _on_load_ready() :
	emit_signal("loadGameNow")

func _on_quit_button_0_pressed() -> void:
	SaveManager.saveGame(Definitions.saveSlots.current)
	emit_signal("exitToMenu")

func _on_return_button_pressed() -> void:
	queue_free()

func _on_manual_save_button_pressed() -> void:
	SaveManager.saveGameSaveSelection(self)

func _on_load_button_pressed() -> void:
	SaveManager.loadGameSaveSelection(self)
