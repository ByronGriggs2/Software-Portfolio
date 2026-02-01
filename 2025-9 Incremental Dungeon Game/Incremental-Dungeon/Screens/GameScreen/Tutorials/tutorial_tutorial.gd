extends "res://Screens/GameScreen/Tutorials/tutorial.gd"

signal setTutorialsEnabledRequested
func _on_my_button_pressed() :
	emit_signal("setTutorialsEnabledRequested", !$VBoxContainer/CheckBox.is_pressed())
	super()
