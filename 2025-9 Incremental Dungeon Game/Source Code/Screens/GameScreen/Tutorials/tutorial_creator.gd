extends Node

const tutorialLoader = preload("res://Screens/GameScreen/Tutorials/tutorial.tscn")
const tutorialTutorialLoader = preload("res://Screens/GameScreen/Tutorials/tutorial_tutorial.tscn")

var myCanvas : CanvasLayer = null
var myTutorial : Node = null

const signalMap : Dictionary = {
	"continueSignal" : "_on_tutorial_continue",
	"setTutorialsEnabledRequested" : "_on_set_tutorials_enabled"
}

var nestingLevel : int = 0
signal finished
func createTutorial(parent : Node, tutorialName : Encyclopedia.tutorialName, tutorialPos : Vector2, args) :
	cleanupPending = true
	myCanvas = CanvasLayer.new()
	parent.add_child(myCanvas)
	myCanvas.layer = Helpers.getTopLayer() + 1
	if (tutorialName == Encyclopedia.tutorialName.tutorial) :
		myTutorial = tutorialTutorialLoader.instantiate()
	else :
		myTutorial = tutorialLoader.instantiate()
	myCanvas.add_child(myTutorial)
	myTutorial.initialise(tutorialName, tutorialPos, args)
	myTutorial.connect("continueSignal", _on_tutorial_continue)
	for mySignal in signalMap.keys() :
		if (myTutorial.has_signal(mySignal)) :
			if (parent.has_method(signalMap[mySignal])) :
				myTutorial.connect(mySignal, Callable(parent, signalMap[mySignal]))
	if (parent.has_method("_on_tutorial_creator_finished")) :
		self.connect("finished", Callable(parent, "_on_tutorial_creator_finished"))
	if (cleanupPending) :
		await cleanupComplete
	var nextTutorial = Encyclopedia.tutorialPointers.get(tutorialName)
	if (nextTutorial != null) :
		createTutorial(nextTutorial, Encyclopedia.tutorialPointerPos.get(tutorialName), tutorialPos, args)
		nestingLevel += 1
	if (nestingLevel > 0) :
		nestingLevel -= 1
	else :
		emit_signal("finished")
		queue_free()
	
var cleanupPending : bool = false
signal cleanupComplete
func _on_tutorial_continue(_tutorialType : Encyclopedia.tutorialName, _disable : bool) -> void:
	myCanvas.queue_free()
	myCanvas = null
	myTutorial = null
	cleanupPending = false
	emit_signal("cleanupComplete")
