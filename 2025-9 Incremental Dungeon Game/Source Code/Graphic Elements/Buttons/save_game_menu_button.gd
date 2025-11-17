extends "res://Graphic Elements/Buttons/super_button.gd"

var disabled : bool = false

func setSlot(val : Definitions.saveSlots) :
	$HBoxContainer/SlotLabel.text = "Slot " + str(val as int)
func setName(val : String) :
	$HBoxContainer/Extra/Name.text = val
func setTime(val : String) :
	$HBoxContainer/Extra/Playtime.text = "Playtime: " + val
func showExtra() :
	$HBoxContainer/Extra.visible = true
	$HBoxContainer/Empty.visible = false
func hideExtra() :
	$HBoxContainer/Extra.visible = false
	$HBoxContainer/Empty.visible = true
func set_disabled(val : bool) :
	disabled = val
	if (val) :
		var disabledFontColor = get_theme_color("font_disabled_color", "Button")
		setTextColor(disabledFontColor)
		var disabledBackground = get_theme_stylebox("disabled", "Button")
		add_theme_stylebox_override("panel", disabledBackground)
	else :
		removeTextColor()
		if (hovered) :
			setPanelTheme("hover")
		else :
			setPanelTheme("normal")

func setTextColor(newColor) :
	$HBoxContainer/SlotLabel.add_theme_color_override("default_color", newColor)
	$HBoxContainer/Extra/Name.add_theme_color_override("default_color", newColor)
	$HBoxContainer/Extra/Playtime.add_theme_color_override("default_color", newColor)
	$HBoxContainer/Empty.add_theme_color_override("default_color", newColor)

func removeTextColor() :
	$HBoxContainer/SlotLabel.remove_theme_color_override("default_color")
	$HBoxContainer/Extra/Name.remove_theme_color_override("default_color")
	$HBoxContainer/Extra/Playtime.remove_theme_color_override("default_color")
	$HBoxContainer/Empty.remove_theme_color_override("default_color")

func _on_gui_input(event: InputEvent) -> void:
	if (!disabled) :
		super(event)
		
func _on_mouse_entered() :
	if (!disabled) :
		super()
		
func _on_mouse_exited() :
	if (!disabled) :
		super()
		
func setTheme(newTheme) :
	if (!disabled) :
		super(newTheme)
	else :
		theme = newTheme
		set_disabled(true)
