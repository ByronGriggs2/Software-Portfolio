extends Button

signal levelChosen
enum myVisibilityEnum {invisible, halfVisible, fullVisible}
@export var encounter : Encounter = null
@export var nameWhenUnexplored : String = "Unexplored"
@export var nameWhenCompleted : String = ""

var currentVisibility : myVisibilityEnum = myVisibilityEnum.invisible
var visited : bool = false
var completed : bool = false
########################################
## Construction
## After calling this function (with valid arguments), it is safe to call 
## beforeLoad(), and optionally onLoad()
func initialise(myEncounter : Encounter, visibility : int) :
	setEncounter(myEncounter)
	setVisibility(visibility)
#########################################
## Getters
func isCompleted() :
	return completed
func isFirstEntry() :
	return !visited
func getVisibility() :
	return currentVisibility
func getEncounterRef() :
	return encounter
#########################################
## Setters
func enter() :
	visited = true
func setEncounter(val : Encounter) :
	encounter = val
	setupEnemies()
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
		else :
			enableEnemyList()
##########################################
## Internal
const enemyEntryLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/Map_Runtime/enemy_entry_runtime.tscn")
func setupEnemies() :
	var oldVis = $VBoxContainer.visible
	$VBoxContainer.visible = false
	if (encounter.enemies.size() > 1) :
		$VBoxContainer.set_block_signals(true)
	for index in range(0, encounter.enemies.size()) :
		if (index == encounter.enemies.size() -1) :
			$VBoxContainer.set_block_signals(false)
		var enemy = encounter.enemies[index]
		var newEntry = enemyEntryLoader.instantiate()
		$VBoxContainer.add_child(newEntry)
		newEntry.setEnemy(enemy)
	$VBoxContainer.visible = oldVis
		
func createBigEntry() :
	for child in $VBoxContainer.get_children() :
		var x1 = child.global_position.x
		var x2 = x1 + child.size.x
		var y1 = child.global_position.y
		var y2 = y1 + child.size.y
		var mousePos = get_global_mouse_position()
		if (mousePos.x >= x1 && mousePos.x <= x2 && mousePos.y >= y1 && mousePos.y <= y2) :
			if (!child.has_connections("addChildRequested")) :
				child.connect("addChildRequested", _on_add_child_requested)
			child.createBigEntry()
			return
			
signal addChildRequested
func _on_add_child_requested(emitter, node : Node) :
	emit_signal("addChildRequested", emitter, node)
			
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
###########################################
## Callbacks
func onCombatComplete() :	
	completed = true
	var enemyEntries = $VBoxContainer.get_children()
	for index in range(0,enemyEntries.size()) :
		enemyEntries[index].incrementKilled()
	enableEnemyList()
	if (Definitions.hasDLC && encounter != null && encounter.enemies.size() != 0 && encounter.enemies[0].getResourceName() == "athena") :
		self.set_disabled(true)
func onCombatLoss() :
	enableEnemyList()
func onCombatRetreat() :
	enableEnemyList()
func _on_gui_input(event: InputEvent) -> void:
	if (disabled) :
		return
	elif (event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.pressed) :
		accept_event()
		if (Input.is_key_pressed(KEY_CTRL) && visited) :
			createBigEntry()
		else :
			emit_signal("levelChosen", self)

##################################################
## Manual Saving
var myReady : bool = false
signal myReadySignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
	
func beforeLoad() :
	## visited, completed, and visibility have default values in their definitions
	## although visibility can be overridden by initialise() prior to calling this function
	pass

func onLoad(loadDict) :
	visited = loadDict["visited"]
	completed = loadDict["completed"]
	setVisibility(loadDict["visibility"])
	if (Definitions.hasDLC) :
		if (completed && encounter != null && encounter.enemies.size() != 0 && encounter.enemies[0].getResourceName() == "athena") :
			self.set_disabled(true)
	#var enemies = $VBoxContainer.get_children()
	#for index in range(0, enemies.size()) :
		#enemies[index].onLoad(loadDict["enemies"][index])

func getSaveDictionary() :
	var tempDict : Dictionary = {}
	tempDict["visited"] = visited
	tempDict["completed"] = completed
	tempDict["visibility"] = currentVisibility
	#tempDict["enemies"] = []
	#for enemy in $VBoxContainer.get_children() :
		#tempDict["enemies"].append(enemy.getSaveDictionary())
	return tempDict

func _process(_delta) :
	var vboxSize = $VBoxContainer.size
	if ((size.x < vboxSize.x + 19 || size.y < vboxSize.y + 19) && $VBoxContainer.visible) :
		var originalSize = size
		size = $VBoxContainer.size + Vector2(20,20)
		global_position.y -= (size.y-originalSize.y)/2.0
		global_position.x -= (size.x-originalSize.x)/2.0
