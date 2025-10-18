@tool
extends Panel

const trainingEntry = preload("res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/training_entry.tscn")
@export var resources : Dictionary

func _ready() :
	if (Engine.is_editor_hint()) :
		#updateChildren()
		pass
	else :
		for child in $Con.get_children() :
			child.setResource(resources[child.name])
			child.connect("requestedEnable", _on_requested_enable)
			child.connect("requestedDisable", _on_requested_disable)
		var maxSize = 0
		for child in $Con.get_children() :
			if (child.getNameWidth() > maxSize) :
				maxSize = child.getNameWidth()
		for child in $Con.get_children() :
			if (child is Panel) :
				child.setNameWidth(maxSize)
			
func setCurrentTraining(val : AttributeTraining) :
	for child in $Con.get_children() :
		if (child.getResource() == val) :
			child.setButton()
		else :
			child.clearButton()
		
signal trainingChanged
func _on_requested_enable(emitter) :
	for child in $Con.get_children() :
		if (child != emitter) :
			child.clearButton()
	emit_signal("trainingChanged", emitter.getResource())

func _on_requested_disable(_emitter) :
	emit_signal("trainingChanged", null)
	
func updateChildren() :
	for child in $Con.get_children() :
		child.queue_free()
		$Con.remove_child(child)
	resources = {}
	var directory : String = "res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/Training"
	var dir = DirAccess.open(directory)
	dir.list_dir_begin()
	var filename = dir.get_next()
	while (filename != "") :
		var newEntry = trainingEntry.instantiate()
		$Con.add_child(newEntry)
		newEntry.name = filename
		newEntry.owner = $Con.get_owner()
		resources[newEntry.name] = directory + "/" + filename
		filename = dir.get_next()
