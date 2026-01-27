extends Panel

const trainingEntry = preload("res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/training_entry.tscn")
#@export var resources : Dictionary
		
func _on_alphabetical_sort_pressed() -> void:
	sortTraining(null)
	
#var cachedRoutineSpeed : Array[float] = [1,1,1,1,1]
#var cachedMultipliers : Array[float] = [0,0,0,0,0]
#func cacheRoutineSpeed(val : Array[float]) :
	#cachedRoutineSpeed = val
#func cacheMultipliers(val : Array[float]) :
	#cachedMultipliers = val
#func _process(_delta) :
	#var grid = $RoutineGrowth/PanelContainer/GridContainer
	#for key in Definitions.attributeDictionary.keys() :
		#var textBox = grid.get_child(Definitions.attributeDictionary.keys().size() + key)
		#var newText = str(Helpers.engineeringRound(cachedMultipliers[key] * cachedRoutineSpeed[key] / 10.0,3))
		#textBox.text = newText
var playerNumberRefs : Array[NumberClass] = []
var myNumberRefs : Array[NumberClass] = []
## Dependency injection of values from the Player Panel
func initialiseNumberRefs(val : Array[NumberClass]) :
	var grid = $RoutineGrowth/PanelContainer/GridContainer
	playerNumberRefs = val
	for index in range(0, playerNumberRefs.size()) :
		var newNumberClass : NumberClass = NumberClass.new()
		myNumberRefs.append(newNumberClass)
		grid.get_child(Definitions.attributeDictionary.keys().size() + index).setNumberReference(myNumberRefs[index])
	
var oldValues : Array[float] = []
func _process(_delta) :
	for index in range(0,playerNumberRefs.size()) :
		myNumberRefs[index].setPremultiplier("Routine Speed", playerNumberRefs[index].getFinal())
		if (currentTrainingResource != null) :
			myNumberRefs[index].setPrebonus("Base RGR/10", currentTrainingResource.getScaling(index as Definitions.attributeEnum)/10.0)
			myNumberRefs[index].setPostmultiplier("Routine Upgrade", routineUpgradeLevels[currentTrainingResource.getResourceName()] * 0.25)
	updateValues()
	
func updateValues() :
	var update : bool = false
	if (oldValues.size() != myNumberRefs.size()) :
		update = true
	else :
		for index in range(0,myNumberRefs.size()) :
			if (!(is_equal_approx(myNumberRefs[index].getFinal(), oldValues[index]))) :
				update = true
				break
	if (update) :
		oldValues = []
		for num in myNumberRefs :
			oldValues.append(num.getFinal())
		if (currentTrainingResource != null) :
			emit_signal("trainingChanged", await createUpgradedTraining(currentTrainingResource))

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
		entries.sort_custom(func(a,b):return a.getResource().text<b.getResource().text)
	else :
		entries.sort_custom(func(a,b):
			var aScaling = a.getResource().scaling[Definitions.attributeDictionary[currentSortAttribute]]*(1+0.25*routineUpgradeLevels[a.getResource().getResourceName()])
			var bScaling = b.getResource().scaling[Definitions.attributeDictionary[currentSortAttribute]]*(1+0.25*routineUpgradeLevels[b.getResource().getResourceName()])
			return (aScaling>bScaling || (aScaling==bScaling&&a.name<b.name)))
	for index in range(0, entries.size()) :
		$Con.move_child(entries[index], index+3)
	
func setTrainingGraphic(val : AttributeTraining) :
	for child in $Con.get_children() :
		if (child == $Con/Title || child == $Con/Spacer || child == $Con/PanelContainer) :
			continue
		if (child.getResource() == val) :
			child.setButton()
		else :
			child.clearButton()
		
signal trainingChanged
var currentTrainingResource : AttributeTraining = null
var routineUpgradeLevels : Dictionary = {}
func _on_requested_enable(emitter) :
	setTrainingGraphic(emitter.getResource())
	currentTrainingResource = emitter.getResource()
	emit_signal("trainingChanged", await createUpgradedTraining(emitter.getResource()))
	
func unlockRoutine(routine : AttributeTraining) :
	for child in $Con.get_children() :
		if (child.has_method("getResource") && child.getResource() == routine) :
			child.visible = true
			return
			
func upgradeRoutine(routine : AttributeTraining) :
	routineUpgradeLevels[routine.resource_path.get_file().get_basename()] += 1
	if (routine == currentTrainingResource) :
		emit_signal("trainingChanged", await createUpgradedTraining(currentTrainingResource))
	$Con.get_node(routine.text).setResource(routine, routineUpgradeLevels[routine.getResourceName()])
		
func createUpgradedTraining(routine : AttributeTraining) :
	while (playerNumberRefs.size() != Definitions.attributeDictionary.keys().size()) :
		await get_tree().process_frame
	var retVal = routine.duplicate()
	for key in retVal.scaling.keys() :
		retVal.scaling[key] *= 1 + 0.25*routineUpgradeLevels[routine.resource_path.get_file().get_basename()]
		retVal.scaling[key] *= playerNumberRefs[Definitions.attributeDictionary.find_key(key)].getFinal()
	return retVal

func _on_requested_disable(_emitter) :
	currentTrainingResource = null
	emit_signal("trainingChanged", null)
	
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
func beforeLoad(newGame) :
	myReady = false
	var routineList = MegaFile.getAllRoutine()
	for routine in routineList :
		routineUpgradeLevels[routine.resource_path.get_file().get_basename()] = 0
	## Fill out header
	for key in Definitions.attributeDictionary.keys() :
		var newHeader = $Con/PanelContainer/HBoxContainer/HBoxContainer/Sample.duplicate()
		$Con/PanelContainer/HBoxContainer/HBoxContainer.add_child(newHeader)
		newHeader.visible = true
		newHeader.name = Definitions.attributeDictionary[key]
		newHeader.text = Definitions.attributeDictionaryShort[key]
		newHeader.connect("myPressed", _on_header_button_pressed)
	for key in MegaFile.Routine_FilesDictionary.keys() :
		var newEntry = trainingEntry.instantiate()
		$Con.add_child(newEntry)
		newEntry.setResource(MegaFile.getRoutine(key), 0)
		newEntry.name = newEntry.getResource().text
		newEntry.visible = !newEntry.getResource().hidden
		newEntry.connect("requestedEnable", _on_requested_enable)
		newEntry.connect("requestedDisable", _on_requested_disable)
	myReady = true
	emit_signal("myReadySignal")
	if (newGame) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
func onLoad(loadDict : Dictionary) :
	myReady = false
	routineUpgradeLevels = loadDict["routineUpgrades"]
	for node in $Con.get_children() :
		if (node != $Con/Title && node != $Con/Spacer && node != $Con/PanelContainer) :
			node.visible = loadDict["routineUnlocks"][node.name]
			var upgrades = routineUpgradeLevels[node.getResource().getResourceName()]
			node.setResource(node.getResource(),upgrades)
	if (loadDict["currentTraining"] == "null") :
		currentTrainingResource = null
	else :
		currentTrainingResource = MegaFile.getRoutine(loadDict["currentTraining"])
		for child in $Con.get_children() :
			if (child.has_method("getResource") && child.getResource().getResourceName() == loadDict["currentTraining"]) :
				child.setButton()
				break
		emit_signal("trainingChanged", await createUpgradedTraining(currentTrainingResource))
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	var routineUnlocks : Dictionary = {}
	for node in $Con.get_children() :
		if (node != $Con/Title && node != $Con/Spacer && node != $Con/PanelContainer) :
			routineUnlocks[node.name] = node.visible
	retVal["routineUnlocks"] = routineUnlocks
	retVal["routineUpgrades"] = routineUpgradeLevels
	if (currentTrainingResource == null) :
		retVal["currentTraining"] = "null"
	else :
		retVal["currentTraining"] = currentTrainingResource.getResourceName()
	return retVal
