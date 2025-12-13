extends Control

## Responsible for launching tutorials at normal times. 
var tutorialQueue : Array[Encyclopedia.tutorialName] = []
var tutorialQueuePos : Array[Vector2] = []
##################################
## Components of IGOptions that this node is allowed to change
var tutorialsEnabled : bool = true
var individualTutorialDisable : Dictionary
##################################
## Other
const tutorialCreatorLoader = preload("res://Screens/GameScreen/Tutorials/tutorial_creator.gd")
func createTutorial(myName : Encyclopedia.tutorialName, myPosition : Vector2) :
	IGOptions.addToTutorialListNoSignal(myName)
	var individual = individualTutorialDisable.get(myName)
	if (!tutorialsEnabled || (individual != null && individual == true)) :
		if (tutorialQueue.front() == myName) :
			tutorialQueue.pop_front()
			tutorialQueuePos.pop_front()
		return
	var tutorialCreator = tutorialCreatorLoader.new()
	add_child(tutorialCreator)
	var args : Array = []
	var startAsDisabled : bool = false
	args.append(startAsDisabled)
	tutorialCreator.createTutorial(self, myName, myPosition, args)
func queueTutorial(myName : Encyclopedia.tutorialName, myPosition : Vector2) :
	tutorialQueue.append(myName)
	tutorialQueuePos.append(myPosition)
	if (tutorialQueue.size() == 1) :
		createTutorial(myName, myPosition)


################################################
## Signals
func _on_tutorial_continue(tutorialType : Encyclopedia.tutorialName, disable : bool) -> void:
	individualTutorialDisable[tutorialType] = disable
	updateIGOptions()
		
func _on_tutorial_creator_finished() :
	tutorialQueue.pop_front()
	tutorialQueuePos.pop_front()
	if (!(tutorialQueue.is_empty())) :
		await get_tree().process_frame
		createTutorial(tutorialQueue[0], tutorialQueuePos[0])

func _on_set_tutorials_enabled(val) :
	tutorialsEnabled = val
	updateIGOptions()
###############################################
## Internal
func updateIGOptions() :
	var tempDict = IGOptions.getIGOptionsCopy()
	tempDict["individualTutorialDisable"] = individualTutorialDisable
	tempDict[IGOptions.options.tutorialsEnabled] = tutorialsEnabled
	IGOptions.saveIGOptionsNoUpdate(tempDict)
##############################################
## IGOptions
func updateFromOptions(changeDict : Dictionary) :
	tutorialsEnabled = changeDict[IGOptions.options.tutorialsEnabled]
	individualTutorialDisable = changeDict["individualTutorialDisable"]
###############################################
## Saving
## IGOptions is responsible for saving data to disk
