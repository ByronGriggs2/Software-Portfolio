extends Panel

var playerReference = null
const multiplierEntryLoader = preload("res://Screens/GameScreen/Tabs/Attributes/AttributeMultipliers/attribute_multiplier_entry.tscn")

func _ready() :
	addMultiplierSet(getEmptyArr(), "Title", "Source")
	$Con.get_node("Title").makeTitle()

func addMultiplierSet(val : Array[float], newName : String, sourceName : String) :
	var newEntry = multiplierEntryLoader.instantiate()
	$Con.add_child(newEntry)
	if (newName != null) :
		newEntry.name = newName
	if (sourceName != null) :
		newEntry.setSource(sourceName)
	setMultipliers(newEntry, val)

func setMultipliers(entryReference, val : Array[float]) :
	for key in Definitions.attributeDictionary.keys() :
		entryReference.setMultiplier(key, val[key])
		
func addPlayerReference(player) :
	playerReference = player
	addMultiplierSet(getEmptyArr(), "playerClass", "Class")
	
func _process(_delta) :
	updateClass()
	
func updateClass() :
	if (playerReference == null) :
		return
	var classEntry = $Con.get_node("playerClass")
	if (classEntry == null) :
		var emptyArr = getEmptyArr()
		addMultiplierSet(emptyArr, "playerClass", "Class")
		classEntry = $Con.get_node("playerClass")
		
	for key in Definitions.attributeDictionary.keys() :
		classEntry.setMultiplier(key, playerReference.getClass().getAttributeScaling(key))
		
func getEmptyArr() -> Array[float] :
	var empty : Array[float]
	for key in Definitions.attributeDictionary.keys() :
		empty.append(0)
	return empty
