extends "res://Graphic Elements/popups/my_popup.gd"

signal hyperChosen
func _on_button_pressed() -> void:
	var hyperMode = $Panel/CenterContainer/Window/VBoxContainer/HBoxContainer2/CheckBox.button_pressed
	emit_signal("hyperChosen", hyperMode)
	queue_free()

func _on_check_box_1_toggled(toggled_on: bool) -> void:
	$Panel/CenterContainer/Window/VBoxContainer/HBoxContainer2/CheckBox.set_pressed_no_signal(!toggled_on)


func _on_check_box_2_toggled(toggled_on: bool) -> void:
	$Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/CheckBox.set_pressed_no_signal(!toggled_on)
