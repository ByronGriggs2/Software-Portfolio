extends "res://Graphic Elements/Buttons/super_button.gd"

func deselect() :
	pass
	
func select() :
	pass
	
func setBorderColor(color : Color) :
	pass

func setBorderWidth(width : int) :
	pass

func _on_mouse_entered() -> void:
	pass

func _on_mouse_exited() -> void:
	pass

func _on_gui_input(event: InputEvent) -> void:
	if (event is InputEventMouseButton && event.get_button_index() == MOUSE_BUTTON_LEFT && event.is_pressed()) :
		selected = true
		emit_signal("wasSelected", self)
