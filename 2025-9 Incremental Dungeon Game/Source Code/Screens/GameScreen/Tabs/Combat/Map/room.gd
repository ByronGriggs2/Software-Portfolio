extends Button

signal levelChosen
enum myVisibilityEnum {invisible, halfVisible, fullVisible}
@export var encounter : Encounter = null
@export var nameWhenUnexplored : String = "Unexplored"
@export var nameWhenCompleted : String = ""
@export var isEvent : bool = false
@export var isShop : bool = false
@export var shopDetails : ShopDetails
@export var visibilityOnStartup : myVisibilityEnum = myVisibilityEnum.invisible

var currentVisibility : myVisibilityEnum = myVisibilityEnum.invisible
var visited : bool = false
var completed : bool = false

var encounterOverride : Encounter = null
var overrideOnce : bool = false

func isFirstEntry() :
	return !visited
	
func enter() :
	visited = true

const enemyEntryLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/enemy_entry.tscn")

#func _on_pressed() -> void:
	#emit_signal("levelChosen", self)

func getVisibility() :
	return currentVisibility
	
func setVisibility(param) :
	var val : myVisibilityEnum
	if (param is String) :
		if (param == "full") :
			val = myVisibilityEnum.fullVisible
		elif (param == "partial" || param == "half") :
			val = myVisibilityEnum.halfVisible
		elif (param == "invisible" || param == "none") :
			val = myVisibilityEnum.invisible
		else :
			return
	else :
		val = param
	if (val as myVisibilityEnum == myVisibilityEnum.invisible) :
		visible = false
		currentVisibility = myVisibilityEnum.invisible
		set_disabled(true)
		text = ""
	elif (val as myVisibilityEnum == myVisibilityEnum.halfVisible) :
		visible = true
		currentVisibility = myVisibilityEnum.halfVisible
		set_disabled(true)
		text = ""
	elif (val as myVisibilityEnum == myVisibilityEnum.fullVisible) :
		visible = true
		currentVisibility = myVisibilityEnum.fullVisible
		set_disabled(false)
		if (!visited) :
			text = " " + nameWhenUnexplored + " "
		elif (isEvent) :
			setTextEventComplete()
		elif (isShop) :
			$ShopSymbol.visible = true
		elif (isNormalCombat()) :
			enableEnemyList()
		elif (completed) :
			text = " " + nameWhenCompleted + " "
		else :
			return
			
func setTextEventComplete() :
	text = "Event completed\n\n(" + nameWhenCompleted + ")"
	set_disabled(true)

func getEncounterRef() :
	if (encounterOverride != null) :
		return encounterOverride
	return encounter
	
func onCombatComplete() :	
	if (overrideOnce) :
		encounterOverride = null
		overrideOnce = false
		return
	completed = true
	if (isEvent) :
		setTextEventComplete()
	elif (isNormalCombat()) :
		enableEnemyList()
	else :
		return

func onCombatLoss() :
	if (isNormalCombat()) :
		enableEnemyList()

func onCombatRetreat() :
	if (isNormalCombat()) :
		enableEnemyList()

signal shopRequested
func _on_gui_input(event: InputEvent) -> void:
	if (disabled) :
		return
	elif (event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.pressed) :
		accept_event()
		if (isShop) :
			visited = true
			emit_signal("shopRequested", shopDetails)
		elif (Input.is_key_pressed(KEY_CTRL) && visited && isNormalCombat()) :
			createBigEntry()
		else :
			emit_signal("levelChosen", self)
			
func createBigEntry() :
	for child in $VBoxContainer.get_children() :
		if (!EnemyDatabase.getEnemyKilled(child.getEnemy().getName())) :
			continue
		var x1 = child.global_position.x
		var x2 = x1 + child.size.x
		var y1 = child.global_position.y
		var y2 = y1 + child.size.y
		var mousePos = get_global_mouse_position()
		if (mousePos.x >= x1 && mousePos.x <= x2 && mousePos.y >= y1 && mousePos.y <= y2) :
			child.createBigEntry()
			return
			
var myReady : bool = false
func _ready() :
	myReady = true
	
func beforeLoad(newGame) :
	if (encounter != null) :
		for enemy in encounter.enemies :
			var newEntry = enemyEntryLoader.instantiate()
			$VBoxContainer.add_child(newEntry)
			newEntry.setEnemy(enemy)
	if (newGame) :
		setVisibility(visibilityOnStartup as int)

func onLoad(loadDict) :
	visited = loadDict["visited"]
	completed = loadDict["completed"]
	setVisibility(loadDict["visibility"])

func enableEnemyList() :
	$VBoxContainer.visible = true
	text = ""

func disableEnemyList() :
	$VBoxContainer.visible = false
	if (!visited) :
		text = " " + nameWhenUnexplored + " "
	elif (completed) :
		text = " " + nameWhenCompleted + " "
	else :
		text = ""

func getSaveDictionary() :
	var tempDict : Dictionary = {}
	tempDict["visited"] = visited
	tempDict["completed"] = completed
	tempDict["visibility"] = currentVisibility
	return tempDict

func isCompleted() :
	return completed
	
func isNormalCombat() :
	return !isEvent && !isShop

## To be removed
func overrideEncounter(newEncounter : Encounter, once : bool) :
	encounterOverride = newEncounter
	overrideOnce = once
		
func getOverridePath() :
	if (encounterOverride == null) :
		return "null"
	return encounterOverride.resource_path
	
func getOverrideOnce() :
	return overrideOnce
