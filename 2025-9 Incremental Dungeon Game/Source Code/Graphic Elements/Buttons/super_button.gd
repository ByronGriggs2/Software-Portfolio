extends PanelContainer

signal wasSelected
var selected : bool = false

func _ready() :
	var myBox = get_theme_stylebox("panel").duplicate(true)
	add_theme_stylebox_override("panel", myBox)
	$HBoxContainer.queue_sort()

func deselect() :
	selected = false
	setBorderWidth(2)
	setBorderColor(Color(0,0,0,1))
	
func select() :
	selected = true
	setBorderWidth(4)
	setBorderColor(Color(0,0,0,1))
	
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
	if (!selected) :
		setBorderColor(Color(1,1,0,1))

func _on_mouse_exited() -> void:
	if (!selected) :
		setBorderColor(Color(0,0,0,1))

func _on_gui_input(event: InputEvent) -> void:
	if (event is InputEventMouseButton && event.get_button_index() == MOUSE_BUTTON_LEFT && event.is_pressed()) :
		selected = true
		setBorderWidth(4)
		setBorderColor(Color(0,0,0,1))
		emit_signal("wasSelected", self)
