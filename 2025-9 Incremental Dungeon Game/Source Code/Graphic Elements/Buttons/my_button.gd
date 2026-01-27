extends Button

@export var frame : Texture2D
var myText : String

func setText(newText) :
	myText = newText
	text = " " + newText + " "
func getText() :
	return myText

signal myPressed
func _on_pressed() -> void:
	emit_signal("myPressed", self)
