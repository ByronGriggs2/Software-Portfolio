extends PanelContainer

signal wasSelected
var selected : bool = false

func _ready() :
	var myBox = get_theme_stylebox("panel").duplicate(true)
	add_theme_stylebox_override("panel", myBox)

func deselect() :
	pass
	
func setBorderColor(color : Color) :
	var myBox : StyleBoxFlat = get_theme_stylebox("panel")
	myBox.border_color = color

func setBorderWidth(width : int) :
	var myBox : StyleBoxFlat = get_theme_stylebox("panel")
	myBox.border_width_left = width
	myBox.border_width_right = width
	myBox.border_width_top = width
	myBox.border_width_bottom = width
	add_theme_stylebox_override("panel", myBox)

func _on_mouse_entered() -> void:
	pass

func _on_mouse_exited() -> void:
	pass

func _on_gui_input(event: InputEvent) -> void:
	if (event is InputEventMouseButton && event.get_button_index() == MOUSE_BUTTON_LEFT && event.is_pressed()) :
		emit_signal("wasSelected", self)
