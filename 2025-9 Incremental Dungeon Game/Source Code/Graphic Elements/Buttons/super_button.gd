extends PanelContainer

@export var toggle : bool = true
@export var sticky : bool = false
@export var contentMargin : float = 0

signal wasSelected
var selected : bool = false
var hovered : bool = false
var mouseButtonWasPressed : bool = false

func setAlignment(val : String) :
	if (val == "Begin") :
		$HBoxContainer.set_alignment(BoxContainer.ALIGNMENT_BEGIN)
	elif (val == "Center") :
		$HBoxContainer.set_alignment(BoxContainer.ALIGNMENT_CENTER)
	elif (val == "End") :
		$HBoxContainer.set_alignment(BoxContainer.ALIGNMENT_END)
		
func addToContainer(node : Node) :
	$HBoxContainer.add_child(node)
	if (node is Control) :
		node.set_mouse_filter(MOUSE_FILTER_IGNORE)
	
func isSelected() :
	return selected

func setSticky(val : bool) :
	sticky = val

func _ready() :
	setPanelTheme("normal")
	$HBoxContainer.queue_sort()

func deselect() :
	selected = false
	updatePanel()
	#setBorderWidth(2)
	#setBorderColor(Color(0,0,0,1))
	
func select() :
	selected = true
	setPanelTheme("pressed")
	#setBorderWidth(4)
	#setBorderColor(Color(0,0,0,1))
	
#func setBorderColor(color : Color) :
	#var myBox : StyleBoxFlat = get_theme_stylebox("panel")
	#myBox.border_color = color
	
func setPanelTheme(val : String) :
	var override = theme.get_stylebox(val,"Button").duplicate()
	override.content_margin_left = contentMargin
	override.content_margin_right = contentMargin
	override.content_margin_top = contentMargin
	override.content_margin_bottom = contentMargin
	add_theme_stylebox_override("panel", override)

#func setBorderWidth(width : int) :
	#var myBox : StyleBoxFlat = get_theme_stylebox("panel")
	#myBox.border_width_left = width
	#myBox.border_width_right = width
	#myBox.border_width_top = width
	#myBox.border_width_bottom = width
	#add_theme_stylebox_override("panel", myBox)

func _on_mouse_entered() -> void:
	hovered = true
	if (!selected) :
		#setBorderColor(Color(1,1,0,1))
		setPanelTheme("hover")

func _on_mouse_exited() -> void:
	hovered = false
	if (!selected) :
		setPanelTheme("normal")
		#setBorderColor(Color(0,0,0,1))
		
signal wasDeselected
func _on_gui_input(event: InputEvent) -> void:
	if (event is InputEventMouseButton && event.get_button_index() == MOUSE_BUTTON_LEFT) :
		if (toggle) :
			if (event.is_pressed()) :
				if (selected && !sticky) :
					selected = false
					emit_signal("wasDeselected", self)
				else :
					selected = true
					emit_signal("wasSelected", self)
			elif (!event.is_pressed()) :
				pass
		elif (!toggle) :
			if (event.is_pressed()) :
				mouseButtonWasPressed = true
				selected = true
			elif (!event.is_pressed()) :
				if (mouseButtonWasPressed) :
					emit_signal("wasSelected", self)
				selected = false
				mouseButtonWasPressed = false
		updatePanel()
					
func setTheme(newTheme) :
	theme = newTheme
	updatePanel()

func setContainerExpandHorizontal() :
	size_flags_horizontal = SizeFlags.SIZE_EXPAND_FILL
	
func updatePanel() :
	if (selected) :
		setPanelTheme("pressed") 
	elif (hovered) :
		setPanelTheme("hover")
	else :
		setPanelTheme("normal")
		
func setContentMargin(val) :
	contentMargin = val
	updatePanel()

func setSeparation(val) :
	$HBoxContainer.add_theme_constant_override("separation", val)
