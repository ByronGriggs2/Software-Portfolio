extends Control

var currentLevel : int = 0
const baseGrowth = 10
var growthMultiplier = 0
const maxGrowth = 10000
var currentGrowth = baseGrowth
var actualProgress = 0
var secondaryMultiplier = 1
var multiplicity : float = 1
func setSecondaryGrowthMultiplier(val) :
	secondaryMultiplier = val
func setMultiplicity(val) :
	multiplicity = val

func _process(delta) :
	updateCurrentGrowth()
	#I'm using a very explicit [get data]->[calculate]->[set data] pattern here because I'm a noob
	#and I'm scared of how Godot doesn't have security levels. Might do this the whole game,
	#might not.
	var oldProgress = actualProgress
	var oldLevel : int = currentLevel
	var progressGained = currentGrowth*delta
	
	var baseLevelGained : int = floor((oldProgress+progressGained)/100)
	var guaranteedThroughMultiplicity : int = baseLevelGained * floor(multiplicity)
	var total = guaranteedThroughMultiplicity
	var chance : float = multiplicity-floor(multiplicity)
	for index in range(0, baseLevelGained) :
		if (randf_range(0,1) <= chance) :
			total += 1
	var newProgress = oldProgress + (progressGained - 100*baseLevelGained)
	var newLevel : int = oldLevel + total
	actualProgress = newProgress
	currentLevel = newLevel
	$LevelLabel.text = "Lv " + Helpers.engineeringRound(newLevel,3)	
	if (currentGrowth > baseGrowth*55) :
		$ProgressBar.value = 100
	else :
		$ProgressBar.value = actualProgress
	
func updateCurrentGrowth() -> void :
	var tempGrowth = baseGrowth * growthMultiplier * secondaryMultiplier
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
