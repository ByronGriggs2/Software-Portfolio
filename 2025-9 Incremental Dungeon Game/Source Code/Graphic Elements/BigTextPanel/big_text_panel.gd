@tool
extends Panel

@export_multiline var text : String :
	set(value) :
		internalText = value
		var names : Array[String] = ["VBoxContainer","VBoxContainer","PanelContainer","RichTextLabel"]
		if (Helpers != null && Helpers.childIsValid(self, names)) :
			$VBoxContainer/VBoxContainer/PanelContainer/RichTextLabel.text = value
	get :
		return internalText
var internalText : String = ""

@export var title : String :
	set(value) :
		internalTitle = value
		var names : Array[String] = ["VBoxContainer", "VBoxContainer", "Title"]
		if (Helpers != null && Helpers.childIsValid(self, names)) :
			$VBoxContainer/VBoxContainer/Title.text = value 
	get :
		return internalTitle
var internalTitle : String = ""

func _ready() :
	$VBoxContainer/VBoxContainer/PanelContainer/RichTextLabel.text = internalText
	$VBoxContainer/VBoxContainer/Title.text = internalTitle
