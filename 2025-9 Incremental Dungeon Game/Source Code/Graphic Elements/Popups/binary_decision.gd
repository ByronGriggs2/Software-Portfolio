extends "res://Graphic Elements/popups/my_popup.gd"

signal binaryChosen

func _on_button_1_pressed() -> void:
	emit_signal("binaryChosen", 1)
	queue_free()
	
func _on_button_0_pressed() -> void:
	emit_signal("binaryChosen", 0)
	queue_free()

func setButton0Name(newName) :
	$Window/Option0/Button0.text = newName
	
func setButton1Name(newName) :
	$Window/Option1/Button1.text = newName
