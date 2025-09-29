extends Control

signal setSelfAsActive

var active : bool = false

var currentLevel : int = 0
const baseGrowth = 10
const maxGrowth = 10000
var currentGrowth = baseGrowth
var actualProgress = 0

func _process(delta) :
	if (!active) : #may later want to update the internals even when not active for a tooltip
		$LevelLabel.text = "Lv " + str(currentLevel)
		return
	updateCurrentGrowth()
	#I'm using a very explicit [get data]->[calculate]->[set data] pattern here because I'm a noob
	#and I'm scared of how Godot doesn't have security levels. Might do this the whole game,
	#might not.
	var oldProgress = actualProgress
	var oldLevel : int = currentLevel
	var progressGained = currentGrowth*delta
	var levelGained : int = floor((oldProgress+progressGained)/100)
	var newProgress = oldProgress + (progressGained - 100*levelGained)
	var newLevel : int = oldLevel + levelGained
	actualProgress = newProgress
	currentLevel = newLevel
	$LevelLabel.text = "Lv " + str(newLevel)	
	if (is_equal_approx(currentGrowth, maxGrowth)) :
		$ProgressBar.value = 100
	else :
		$ProgressBar.value = actualProgress
	
func updateCurrentGrowth() -> void :
	var tempGrowth = baseGrowth
	if (tempGrowth >= maxGrowth) :
		currentGrowth = maxGrowth
	else :
		currentGrowth = tempGrowth

func unlock() :
	visible = true
	$EnableButton.visible = true
	$EnableButton.disabled = false

func _on_enable_button_pressed() -> void:
	if (active) :
		deactivate()
		return
	emit_signal("setSelfAsActive", self)
	active = true
	
func deactivate() -> void :
	active = false
	$EnableButton.button_pressed = false

func getLevel() -> int :
	return currentLevel
	
func setLevel(val : int) -> void :
	currentLevel = val

func setLabel(val : String) :
	$NameLabel.text = val

func getLabelWidth() :
	return $NameLabel.size.x
	
func setLabelWidth(val) :
	$NameLabel.custom_minimum_size.x = val
	
func getValue() :
	return actualProgress
	
func setValue(val) :
	actualProgress = val
	$ProgressBar.value = val
