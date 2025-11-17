extends Panel

const multiplierEntryLoader = preload("res://Screens/GameScreen/Tabs/Attributes/AttributeMultipliers/attribute_multiplier_entry.tscn")

enum multiplierType {premultiplier,postmultiplier}

func _ready() :
	addMultiplierSet(multiplierType.premultiplier, getEmptyArr(), "Title", "Source")
	$Panel/Premultipliers.get_node("Title").makeTitle()
	addMultiplierSet(multiplierType.postmultiplier, getEmptyArr(), "Title", "Source")
	$Panel/Postmultipliers.get_node("Title").makeTitle()

func addMultiplierSet(type : multiplierType, val : Array[float], newName : String, sourceName : String) :
	var newEntry = multiplierEntryLoader.instantiate()
	var parent
	if (type == multiplierType.premultiplier) :
		parent = $Panel/Premultipliers
	elif (type == multiplierType.postmultiplier) :
		parent = $Panel/Postmultipliers
	parent.add_child(newEntry)
	if (newName != null) :
		newEntry.name = newName
	if (sourceName != null) :
		newEntry.setSource(sourceName)
	newEntry.myType = type as int
	setMultipliers(newEntry, val)

func setMultipliers(entryReference, val : Array[float]) :
	for key in Definitions.attributeDictionary.keys() :
		entryReference.setMultiplier(key, val[key])
		
func setMultiplierSingle(entryReference, type : Definitions.attributeEnum, val : float) :
	entryReference.setMultiplier(type, val)
	
#########################
var playerClass_comm : CharacterClass = null
var waitingForPlayerClass : bool = false
signal playerClassRequested
signal playerClassReceived

func getPlayerClass() -> CharacterClass:
	waitingForPlayerClass = true
	emit_signal("playerClassRequested", self)
	if (waitingForPlayerClass) :
		await playerClassReceived
	return playerClass_comm
	
func providePlayerClass(val : CharacterClass) :
	playerClass_comm = val
	waitingForPlayerClass = false
	emit_signal("playerClassReceived")
##########################

func getEntryReference(type : multiplierType, myName : String) :
	if (myName == "Subtitle" || myName == "Title") :
		return null
	var parent
	if (type == multiplierType.premultiplier) :
		parent = $Panel/Premultipliers
	elif (type == multiplierType.postmultiplier) :
		parent = $Panel/Postmultipliers		
	for child in parent.get_children() :
		if (child.name == myName) :
			return child
	return null
		
func getEmptyArr() -> Array[float] :
	var empty : Array[float]
	for key in Definitions.attributeDictionary.keys() :
		empty.append(0)
	return empty

func myUpdate(attributeMods : Array[NumberClass]) :
	for attribute in Definitions.attributeDictionary.keys() :
		var premultipliers = attributeMods[attribute].getPremultipliersRaw()
		for key in premultipliers.keys() :
			updateEntrySingle(multiplierType.premultiplier, key, attribute, premultipliers[key])
		var postmultipliers = attributeMods[attribute].getPostmultipliersRaw()
		for key in postmultipliers.keys() :
			updateEntrySingle(multiplierType.postmultiplier, key, attribute, postmultipliers[key])
	updateSize()
	
func updateSize() :
	updateSize2($Panel/Premultipliers)
	updateSize2($Panel/Postmultipliers)
	
func updateSize2(parent : Node) :
	var minSize = 0
	for child in parent.get_children() :
		if (!(child.has_method("getMinSize"))) :
			continue
		var childSize = child.getMinSize()
		if (childSize > minSize) :
			minSize = childSize
	for child in parent.get_children() :
		if (!(child.has_method("getMinSize"))) :
			continue
		child.setMinSize(minSize)
				
func updateEntrySingle(type : multiplierType, entryName : String, attribute : Definitions.attributeEnum, val : float) :
	var entryRef = getEntryReference(type, entryName)
	if (entryRef == null) :
		addMultiplierSet(type, getEmptyArr(), entryName, entryName)
		entryRef = getEntryReference(type, entryName)
	setMultiplierSingle(entryRef, attribute, val)
