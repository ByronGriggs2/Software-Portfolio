extends "res://Graphic Elements/popups/my_popup.gd"

signal finished
func _on_button_pressed() -> void:
	AudioHandler.playMenuSfx(AudioHandler.menuSfx.select2NauPhy)
	emit_signal("finished")
	queue_free()

func setButtonText(val) :
	$Panel/CenterContainer/Window/VBoxContainer/VBoxContainer/Button.text = " " + val + " "
