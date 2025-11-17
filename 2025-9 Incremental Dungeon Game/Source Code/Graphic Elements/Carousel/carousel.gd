extends Control

signal move

@export var options : Array[String]
@export var details : Array[String]
var currentPos : int = 0
var resizing : bool = false

func _ready() :
	if (!options.is_empty()) :
		$HBoxContainer/EncyclopediaTextLabel.setText(options[currentPos])
	if (details.is_empty()) :
		for index in range (options.size()) :
			details.append("")
	await resize()
			
func setOptions(val : Array[String]) :
	options = val
	resize()
	
func resize() :
	#var maxSize = 0
	#for option in options :
		#var size = Helpers.getTextWidth($HBoxContainer/EncyclopediaTextLabel, option)
		#if (size > maxSize) :
			#maxSize = size
	resizing = true
	var maxSize = await Helpers.getTextArrayMaxWidthWaitFrame(null, $HBoxContainer/EncyclopediaTextLabel.get_theme_font_size("normal_font_size"), options)
	$HBoxContainer/EncyclopediaTextLabel.set_custom_minimum_size(Vector2(maxSize,0))
	resizing = false

func _on_left_pressed() -> void:
	if (currentPos == 0) :
		currentPos = options.size()-1
	else :
		currentPos -= 1
	$HBoxContainer/EncyclopediaTextLabel.setText(options[currentPos])
	emit_signal("move", details[currentPos])

func _on_right_pressed() -> void:
	if (currentPos == options.size()-1) :
		currentPos = 0
	else :
		currentPos += 1
	$HBoxContainer/EncyclopediaTextLabel.setText(options[currentPos])
	emit_signal("move", details[currentPos])
	
func refresh() :
	if (!options.is_empty()) :
		$HBoxContainer/EncyclopediaTextLabel.setText(options[currentPos])
	if (details.is_empty()) :
		for index in range (options.size()) :
			details.append("")
	emit_signal("move", details[currentPos])

func getMinWidth() :
	while (resizing) :
		await get_tree().process_frame
	return $HBoxContainer/EncyclopediaTextLabel.custom_minimum_size.x
func setMinWidth(val) :
	$HBoxContainer/EncyclopediaTextLabel.set_custom_minimum_size(Vector2(val, 0))
