@tool
extends Panel

@export_multiline var text : String :
	set(value) :
		internalText = value
		if (is_instance_valid($RichTextLabel)) :
			$RichTextLabel.text = value
	get :
		return internalText

var internalText : String = ""

func _ready() :
	$RichTextLabel.text = internalText
