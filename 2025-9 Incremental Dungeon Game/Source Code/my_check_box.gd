extends PanelContainer

@export var myText : String = "" :
	set(val) :
		myText = val
		$HBoxContainer/RichTextLabel.text = " " + val + " "
	get :
		return myText
func setText(val) :
	myText = val
	$HBoxContainer/RichTextLabel.text = " " + val + " "
	
func _ready() :
	$HBoxContainer/RichTextLabel.text = " " + myText + " "
func isPressed() -> bool :
	return $HBoxContainer/CheckBox.button_pressed()
	
signal pressed
func _on_check_box_pressed() -> void:
	emit_signal("pressed")
func is_pressed() :
	return $HBoxContainer/CheckBox.is_pressed()
func set_pressed_no_signal(val) :
	$HBoxContainer/CheckBox.set_pressed_no_signal(val)
