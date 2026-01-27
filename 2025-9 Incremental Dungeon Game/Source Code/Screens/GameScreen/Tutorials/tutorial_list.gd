extends "res://Graphic Elements/popups/my_popup.gd"

var individualTutorialDisableCopy : Dictionary = {}
func initialise(individualTutorialDisable : Dictionary) :
	individualTutorialDisableCopy = individualTutorialDisable
	var container = getTutorialListContainer()
	for key in individualTutorialDisable.keys() :
		var newEntry : Button = container.get_child(0).duplicate()
		container.add_child(newEntry)
		newEntry.setText(Encyclopedia.tutorialTitles[key as Encyclopedia.tutorialName])
		newEntry.visible = true
		newEntry.connect("myPressed", _on_tutorial_button)
		
func _on_tutorial_button(emitter) :
	var key : Encyclopedia.tutorialName = Encyclopedia.tutorialTitles.find_key(emitter.getText())
	createTutorial(key)
	
func getTutorialListContainer() -> Node :
	return $Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer
	
const tutorialCreatorLoader = preload("res://Screens/GameScreen/Tutorials/tutorial_creator.gd")
func createTutorial(myName : Encyclopedia.tutorialName) :
	var tutorialCreator = tutorialCreatorLoader.new()
	add_child(tutorialCreator)
	var args : Array = []
	var initiallyDisabled : bool = individualTutorialDisableCopy[myName]
	args.append(initiallyDisabled)
	tutorialCreator.createTutorial(self, myName, Vector2(0,0), args)
	
func _on_tutorial_continue(type : Encyclopedia.tutorialName, disabled : bool) :
	individualTutorialDisableCopy[type] = disabled

signal tutorialListFinished
func _on_return_button_pressed() -> void:
	emit_signal("tutorialListFinished", individualTutorialDisableCopy)
	queue_free()
