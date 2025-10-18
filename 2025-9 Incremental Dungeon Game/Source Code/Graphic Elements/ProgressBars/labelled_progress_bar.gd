extends Control

var currentLevel : int = 0
const baseGrowth = 10
var growthMultiplier = 1
const maxGrowth = 10000
var currentGrowth = baseGrowth
var actualProgress = 0

func _process(delta) :
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
	var tempGrowth = baseGrowth * growthMultiplier
	if (tempGrowth >= maxGrowth) :
		currentGrowth = maxGrowth
	else :
		currentGrowth = tempGrowth
func setGrowthMultiplier(val) :
	growthMultiplier = val

func setLabel(val : String) :
	$NameLabel.text = val

func getLabelWidth() :
	return $NameLabel.get_minimum_size().x
	
func setLabelWidth(val) :
	$NameLabel.custom_minimum_size.x = val
	
func getLevel() -> int :
	return currentLevel
func setLevel(val : int) -> void :
	currentLevel = val
func getProgress() :
	return actualProgress
func setProgress(val) :
	actualProgress = val
	$ProgressBar.value = val
