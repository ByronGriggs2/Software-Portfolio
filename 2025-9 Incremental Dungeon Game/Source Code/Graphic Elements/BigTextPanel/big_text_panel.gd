@tool
extends Panel

@export_multiline var text : String :
	set(value) :
		internalText = value
		if (is_instance_valid($VBoxContainer/RichTextLabel)) :
			$VBoxContainer/RichTextLabel.text = value
	get :
		return internalText
var internalText : String = ""

@export var title : String :
	set(value) :
		internalTitle = value
		if (is_instance_valid($VBoxContainer/Title)) :
			$VBoxContainer/Title.text = value 
	get :
		return internalTitle
var internalTitle : String = ""

func _ready() :
	$VBoxContainer/RichTextLabel.text = internalText
	$VBoxContainer/Title.text = internalTitle
