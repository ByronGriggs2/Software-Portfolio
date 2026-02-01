extends PanelContainer

@export var myText : String = "" :
	set(val) :
		myText = val
		$HBoxContainer/RichTextLabel.text = val + " "
	get :
		return myText
func setText(val) :
	myText = val
	$HBoxContainer/RichTextLabel.text = val + " "
@export var pressedOnStartup : bool = false
	
func _ready() :
	$HBoxContainer/RichTextLabel.text = myText + " "
	set_pressed_no_signal(pressedOnStartup)
func isPressed() -> bool :
	return $HBoxContainer/CheckBox.button_pressed
	
signal pressed
func _on_check_box_pressed() -> void:
	emit_signal("pressed")
func is_pressed() :
	return $HBoxContainer/CheckBox.is_pressed()
func set_pressed_no_signal(val) :
	$HBoxContainer/CheckBox.set_pressed_no_signal(val)

func getTextRef() -> Node :
	return $HBoxContainer/RichTextLabel
func setTooltipCurrentLayer(val : int) :
	if ($HBoxContainer/RichTextLabel.get_child_count() == 0) : 
		return
	var tooltip = $HBoxContainer/RichTextLabel.get_child(0)
	if (!tooltip.has_method("setCurrentLayer")) :
		return
	tooltip.setCurrentLayer(val)
