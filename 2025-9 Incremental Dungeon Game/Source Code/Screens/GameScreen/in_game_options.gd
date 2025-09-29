extends CanvasLayer

signal exitToMenu

func _on_quit_button_0_pressed() -> void:
	SaveManager.saveGame()
	emit_signal("exitToMenu")

func _on_quit_button_1_pressed() -> void:
	SaveManager.saveGame()
	get_tree().quit()

func _on_return_button_pressed() -> void:
	queue_free()
