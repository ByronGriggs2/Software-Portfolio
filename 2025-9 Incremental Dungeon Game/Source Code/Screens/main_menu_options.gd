extends Control

signal swapToMainMenu

func _on_return_button_pressed() -> void:
	emit_signal("swapToMainMenu")
