extends Panel

const bonusEntryLoader = preload("res://Screens/GameScreen/Tabs/Attributes/AttributeBonuses/attribute_bonus_entry.tscn")

enum bonusType {prebonus,postbonus}

func _ready() :
	addBonusSet(bonusType.prebonus, getEmptyArr(), "Title", "Source")
	$Panel/Prebonuses.get_node("Title").makeTitle()
	addBonusSet(bonusType.postbonus, getEmptyArr(), "Title", "Source")
	$Panel/Postbonuses.get_node("Title").makeTitle()

func addBonusSet(type : bonusType, val : Array[float], newName : String, sourceName : String) :
	var newEntry = bonusEntryLoader.instantiate()
	var parent
	if (type == bonusType.prebonus) :
		parent = $Panel/Prebonuses
	elif (type == bonusType.postbonus) :
		parent = $Panel/Postbonuses
	parent.add_child(newEntry)
	if (newName != null) :
		newEntry.name = newName
	if (sourceName != null) :
		newEntry.setSource(sourceName)
	newEntry.myType = type as int
	setBonuses(newEntry, val)

func setBonuses(entryReference, val : Array[float]) :
	for key in Definitions.attributeDictionary.keys() :
		entryReference.setBonus(key, val[key])
		
func setBonusSingle(entryReference, type : Definitions.attributeEnum, val : float) :
	entryReference.setBonus(type, val)
	
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

func getEntryReference(type : bonusType, myName : String) :
	if (myName == "Subtitle" || myName == "Title") :
		return null
	var parent
	if (type == bonusType.prebonus) :
		parent = $Panel/Prebonuses
	elif (type == bonusType.postbonus) :
		parent = $Panel/Postbonuses		
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
		var prebonuses = attributeMods[attribute].getPrebonusesRaw()
		for key in prebonuses.keys() :
			updateEntrySingle(bonusType.prebonus, key, attribute, prebonuses[key])
		var postbonuses = attributeMods[attribute].getPostbonusesRaw()
		for key in postbonuses.keys() :
			updateEntrySingle(bonusType.postbonus, key, attribute, postbonuses[key])
	updateSize()
	
func updateSize() :
	updateSize2($Panel/Prebonuses)
	updateSize2($Panel/Postbonuses)
	
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
				
func updateEntrySingle(type : bonusType, entryName : String, attribute : Definitions.attributeEnum, val : float) :
	var entryRef = getEntryReference(type, entryName)
	if (entryRef == null) :
		addBonusSet(type, getEmptyArr(), entryName, entryName)
		entryRef = getEntryReference(type, entryName)
	setBonusSingle(entryRef, attribute, val)
