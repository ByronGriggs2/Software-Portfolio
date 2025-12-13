extends Panel

const trainingEntry = preload("res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/training_entry.tscn")
#@export var resources : Dictionary
		
func _on_alphabetical_sort_pressed() -> void:
	sortTraining(null)

func _on_header_button_pressed(emitter) :
	var type : Definitions.attributeEnum
	for key in Definitions.attributeDictionary.keys() :
		if (emitter.name == Definitions.attributeDictionary[key]) :
			type = key
			break
	sortTraining(type)
	
enum sortType {descending,alphabetical}
var currentSort : sortType = sortType.alphabetical
var currentSortAttribute
func sortTraining(type) :
	if (type == null) :
		currentSort = sortType.alphabetical
		currentSortAttribute = null
	elif (currentSortAttribute == type) :
		currentSort = sortType.alphabetical
		currentSortAttribute = null
	else :
		currentSortAttribute = type
		currentSort = sortType.descending
		
	var children = $Con.get_children()
	var entries : Array[Node] = []
	for index in range(0, children.size()) :
		if(index > 2) :
			entries.append(children[index])
	if (currentSort == sortType.alphabetical) :
		entries.sort_custom(func(a,b):return a.name<b.name)
	else :
		entries.sort_custom(func(a,b):
			var aScaling = a.getResource().scaling[Definitions.attributeDictionary[currentSortAttribute]]
			var bScaling = b.getResource().scaling[Definitions.attributeDictionary[currentSortAttribute]]
			return (aScaling>bScaling || (aScaling==bScaling&&a.name<b.name)))
	for index in range(0, entries.size()) :
		$Con.move_child(entries[index], index+3)
	
func setCurrentTraining(val : AttributeTraining) :
	for child in $Con.get_children() :
		if (child == $Con/Title || child == $Con/Spacer || child == $Con/PanelContainer) :
			continue
		if (child.getResource() == val) :
			child.setButton()
		else :
			child.clearButton()
		
signal trainingChanged
func _on_requested_enable(emitter) :
	for child in $Con.get_children() :
		if (child == $Con/Title || child == $Con/Spacer || child == $Con/PanelContainer) :
			continue
		if (child != emitter) :
			child.clearButton()
	emit_signal("trainingChanged", emitter.getResource())
	
func unlockRoutine(routine : AttributeTraining) :
	for child in $Con.get_children() :
		if (child.has_method("getResource") && child.getResource() == routine) :
			child.visible = true
			return

func _on_requested_disable(_emitter) :
	emit_signal("trainingChanged", null)
	
var myReady : bool = false
func _ready() :
	myReady = true
func beforeLoad(_newGame) :
	## Fill out header
	for key in Definitions.attributeDictionary.keys() :
		var newHeader = $Con/PanelContainer/HBoxContainer/HBoxContainer/Sample.duplicate()
		$Con/PanelContainer/HBoxContainer/HBoxContainer.add_child(newHeader)
		newHeader.visible = true
		newHeader.name = Definitions.attributeDictionary[key]
		newHeader.text = Definitions.attributeDictionaryShort[key]
		newHeader.connect("myPressed", _on_header_button_pressed)
	for key in RoutineReferences.RoutineDictionary.keys() :
		var newEntry = trainingEntry.instantiate()
		$Con.add_child(newEntry)
		newEntry.setResource(RoutineReferences.getRoutine(key))
		newEntry.name = newEntry.getResource().text
		newEntry.visible = !newEntry.getResource().hidden
		newEntry.connect("requestedEnable", _on_requested_enable)
		newEntry.connect("requestedDisable", _on_requested_disable)
func onLoad(loadDict : Dictionary) :
	for node in $Con.get_children() :
		if (node != $Con/Title && node != $Con/Spacer && node != $Con/PanelContainer) :
			node.visible = loadDict["routineUnlocks"][node.name]
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	var routineUnlocks : Dictionary = {}
	for node in $Con.get_children() :
		if (node != $Con/Title && node != $Con/Spacer && node != $Con/PanelContainer) :
			routineUnlocks[node.name] = node.visible
	retVal["routineUnlocks"] = routineUnlocks
	return retVal
