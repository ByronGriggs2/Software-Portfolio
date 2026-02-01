@tool
extends Control

signal move

@export var options : Array[String]
@export var callbackMode : bool = false
@export var myWrap : bool = true
@export var callable : Callable
@export var textureScale : float :
	set(val) :
		hiddenScale = val
		if (myReady) :
			$HBoxContainer/Left.custom_minimum_size = val*minimumSize
		if (myReady) :
			$HBoxContainer/Right.custom_minimum_size = val*minimumSize
	get() :
		return hiddenScale
var hiddenScale : float = 1.0
		

var currentPos : int = 0
func getPosition() -> int :
	return currentPos

const minimumSize = Vector2(14,12)
var myReady : bool = false
func _ready() :
	myReady = true
	if (!options.is_empty()) :
		$HBoxContainer/RichTextLabel.text = options[currentPos]
	$HBoxContainer/Left.custom_minimum_size = hiddenScale*minimumSize
	$HBoxContainer/Right.custom_minimum_size = hiddenScale*minimumSize

func setOptions(val : Array[String]) :
	options = val
	updateCarousel()

func _on_left_pressed() -> void:
	if (currentPos == 0) :
		currentPos = options.size()-1
	else :
		currentPos -= 1
	updateCarousel()
	moveCarousel()
		
func updateCarousel() :
	$HBoxContainer/RichTextLabel.text = options[currentPos]
	if (!myWrap) :
		if (currentPos == 0) :
			$HBoxContainer/Left.set_disabled(true)
			$HBoxContainer/Right.set_disabled(false)
		elif (currentPos == options.size()-1) :
			$HBoxContainer/Left.set_disabled(false)
			$HBoxContainer/Right.set_disabled(true)
		else :
			$HBoxContainer/Right.set_disabled(false)
			$HBoxContainer/Left.set_disabled(false)
func moveCarousel() :
	if (callbackMode) :
		callable.call(options[currentPos])
	else :
		emit_signal("move", self, currentPos, options[currentPos])
func setPositionNoSignal(val : int) :
	currentPos = val
	updateCarousel()

func _on_right_pressed() -> void:
	if (currentPos == options.size()-1) :
		currentPos = 0
	else :
		currentPos += 1
	updateCarousel()
	moveCarousel()

func setPositionWithSignal(val : int, isOffset : bool) :
	if (isOffset) :
		if (options.size() <= abs(val)) :
			return
		else :
			var newValue = currentPos+val
			if (newValue < 0) :
				currentPos = options.size()+newValue
			elif (newValue > options.size()-1) :
				currentPos = newValue-options.size()
			else :
				currentPos = newValue
	else :
		if (val < 0 || val > options.size()-1) :
			return
		else :
			currentPos = val
	updateCarousel()
	moveCarousel()
