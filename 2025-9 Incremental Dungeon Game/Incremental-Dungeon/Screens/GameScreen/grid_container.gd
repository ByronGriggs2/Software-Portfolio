extends GridContainer

signal filterChanged
func _on_button_toggled(toggled_on: bool) -> void:
	var ret : Dictionary = {
		"technology" : [$Button.button_pressed, $Button2.button_pressed, $Button3.button_pressed, $Button4.button_pressed, $Button10.button_pressed],
		"offense" : [$Button12.button_pressed, $Button5.button_pressed, $Button6.button_pressed],
		"defense" : [$Button7.button_pressed, $Button8.button_pressed, $Button9.button_pressed, $Button11.button_pressed]
	}
	emit_signal("filterChanged", ret)
