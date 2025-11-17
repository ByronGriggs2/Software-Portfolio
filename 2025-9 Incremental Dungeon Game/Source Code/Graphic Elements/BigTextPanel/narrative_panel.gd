extends Panel

@export_multiline var text : String :
	set(value) :
		internalText = value
	get :
		return internalText
var internalText : String = ""

func _ready() :
	$PanelContainer/VBoxContainer/RichTextLabel.text = internalText

signal continueSignal
func _on_my_button_pressed() -> void:
	emit_signal("continueSignal", self)
