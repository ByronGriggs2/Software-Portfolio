@tool
extends PanelContainer

signal myToggled
var toggled : bool = false
var hovered : bool = false
var text : String :
	set(val) :
		$RichTextLabel.text = val
	get :
		return $RichTextLabel.text

func initialise() :
	add_theme_stylebox_override("panel", theme.get_stylebox("normal", "Button"))
	$RichTextLabel.add_theme_font_size_override("normal_font_size", theme.get_font_size("font_size", "Button"))
	visible = true

func setText(newText : String) :
	$RichTextLabel.text = newText

func _on_gui_input(event: InputEvent) -> void:
	if (event is InputEventMouseButton && event.button_index == 1 && event.pressed == true) :
		if (!toggled) :
			toggled = true
		if (toggled) :
			add_theme_stylebox_override("panel", theme.get_stylebox("pressed", "Button"))
		elif (hovered) :
			add_theme_stylebox_override("panel", theme.get_stylebox("hover", "Button"))
		else :
			add_theme_stylebox_override("panel", theme.get_stylebox("normal", "Button"))
		emit_signal("myToggled", toggled, self)
		
func _on_mouse_entered() -> void:
	if (!toggled) :
		add_theme_stylebox_override("panel", theme.get_stylebox("hover", "Button"))
	hovered = true

func _on_mouse_exited() -> void:
	if (!toggled) :
		add_theme_stylebox_override("panel", theme.get_stylebox("normal", "Button"))
	hovered = false

func detoggle() :
	toggled = false
	if (hovered) :
		add_theme_stylebox_override("panel", theme.get_stylebox("hover", "Button"))
	else :
		add_theme_stylebox_override("panel", theme.get_stylebox("normal", "Button"))
		
func select() :
	toggled = true
	add_theme_stylebox_override("panel", theme.get_stylebox("pressed", "Button"))
