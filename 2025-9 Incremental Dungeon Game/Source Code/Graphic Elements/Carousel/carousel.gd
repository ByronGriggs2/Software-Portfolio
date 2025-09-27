extends Control

signal move

@export var options : Array[String]
@export var details : Array[String]
var currentPos : int = 0

func _ready() :
	if (!options.is_empty()) :
		$RichTextLabel.text = options[currentPos]
	if (details.is_empty()) :
		for index in range (options.size()) :
			details.append("")

func _on_left_pressed() -> void:
	if (currentPos == 0) :
		currentPos = options.size()-1
	else :
		currentPos -= 1
	$RichTextLabel.text = options[currentPos]
	emit_signal("move", details[currentPos])

func _on_right_pressed() -> void:
	if (currentPos == options.size()-1) :
		currentPos = 0
	else :
		currentPos += 1
	$RichTextLabel.text = options[currentPos]
	emit_signal("move", details[currentPos])
	
func refresh() :
	if (!options.is_empty()) :
		$RichTextLabel.text = options[currentPos]
	if (details.is_empty()) :
		for index in range (options.size()) :
			details.append("")
	emit_signal("move", details[currentPos])
