extends PanelContainer
var myType : Encyclopedia.tutorialName

func _ready() :
	for child in get_children() :
		connectRecursive(child)
	
func connectRecursive(current : Node) :
	if (current.has_signal("resized")) :
		current.connect("resized", _on_resized)
	for child in current.get_children() :
		connectRecursive(child)
	
func _on_resized() :
	queue_sort()
	size = Vector2(0,0)

func initialise(type : Encyclopedia.tutorialName, myPosition : Vector2, args : Array) :
	position = Vector2(9999,9999)
	var startAsDisabled = args[0]
	myType = type
	$VBoxContainer/Title.text = Encyclopedia.tutorialTitles[type]
	var presumedCurrentLayer = Helpers.getTopLayer()
	$VBoxContainer/Panel/EncyclopediaTextLabel.currentLayer = presumedCurrentLayer+1
	await $VBoxContainer/Panel/EncyclopediaTextLabel.setText(Encyclopedia.tutorialDesc[type])
	var tempPosition
	await get_tree().process_frame
	if (myPosition == Vector2(0,0)) :
		var screenSize = Engine.get_singleton("DisplayServer").screen_get_size()
		tempPosition = Vector2((screenSize.x-size.x)/2.0,(screenSize.y-size.y)/2.0)
	else :
		tempPosition = myPosition
		var screenSize = Engine.get_singleton("DisplayServer").screen_get_size()
		tempPosition.x = clamp(tempPosition.x, 0, screenSize.x-size.x)
		tempPosition.y = clamp(tempPosition.y, 0, screenSize.y-size.y)
	global_position = tempPosition
	if (Encyclopedia.oneOffTutorials.find(type) != -1) :
		$VBoxContainer/FuckOffContainer.visible = false
		$VBoxContainer.queue_sort()
		queue_sort()
		$VBoxContainer/FuckOffContainer/CheckBox.set_pressed(true)
	elif (startAsDisabled) :
		$VBoxContainer/FuckOffContainer/CheckBox.set_pressed(true)

signal continueSignal
func _on_my_button_pressed() -> void:
	emit_signal("continueSignal", myType, $VBoxContainer/FuckOffContainer/CheckBox.is_pressed())
	queue_free()
