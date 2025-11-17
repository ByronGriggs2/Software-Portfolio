extends Control

signal levelChosen2
signal tutorialRequested

func onCombatLoss(room) :
	room.onCombatComplete()

##This prevents a warning
func doNotCall() :
	emit_signal("tutorialRequested")
################################
#map
func _on_level_chosen(emitter) :
	emit_signal("levelChosen2", emitter, emitter.getEncounterRef())
	
func completeLevel(completedRoom) :
	completedRoom.onCombatComplete()
	#For all rooms, find rooms linked to this one
	for connection in $CombatMap/ConnectionContainer.get_children() :
		var adjacentRoom = null
		if (connection.Room1 == completedRoom) : 
			adjacentRoom = connection.Room2
		if (connection.Room2 == completedRoom) : 
			adjacentRoom = connection.Room1
		#For rooms linked to this one
		if (adjacentRoom != null) :
			#Fully reveal
			adjacentRoom.fullReveal()
			connection.fullReveal()
			#Find rooms that are 2 links away
			for potentialLooseConnection in $CombatMap/ConnectionContainer.get_children() :
				var overAdjacentRoom = null
				if (potentialLooseConnection.Room1 == adjacentRoom) : 
					overAdjacentRoom = potentialLooseConnection.Room2
				elif (potentialLooseConnection.Room2 == adjacentRoom) : 
					overAdjacentRoom = potentialLooseConnection.Room1
				#Half reveal
				if (overAdjacentRoom != null) :
					overAdjacentRoom.halfReveal()
					potentialLooseConnection.halfReveal()
	
#scroll
@export var scrollStepSize : int = 80
var homePosition : Vector2
var minX : float
var maxX : float
var minY : float
var maxY : float

func initScroll() :
	var tempPos = Vector2(-$CombatMap.size.x/2.0, -$CombatMap.size.y)
	await(get_tree().process_frame)
	homePosition.x = tempPos.x+(size.x/2.0)
	homePosition.y = tempPos.y+size.y
	maxY = homePosition.y+$CombatMap.size.y-size.y
	minY = homePosition.y
	maxX = homePosition.x + ($CombatMap.size.x/2.0)-(size.x/2.0)
	minX = homePosition.x - ($CombatMap.size.x/2.0)+(size.x/2.0)
	goHome()

#func _on_visibility_changed() -> void:
#	if (visible) :
#		grab_focus()

func _unhandled_input(event: InputEvent) -> void:
	if event.is_action("ui_cancel") :
		return
	if (!is_visible_in_tree()) :
		return
	if event is InputEventKey and event.pressed:
		accept_event()
		match event.keycode:
			KEY_LEFT:
				if ($CombatMap.position.x < maxX - 0.9*scrollStepSize) :
					$CombatMap.position.x += scrollStepSize
			KEY_RIGHT:
				if ($CombatMap.position.x > minX + 0.9*scrollStepSize) :
					$CombatMap.position.x -= scrollStepSize
			KEY_UP:
				if ($CombatMap.position.y < maxY - 0.9*scrollStepSize) :
					$CombatMap.position.y += scrollStepSize
			KEY_DOWN:
				if ($CombatMap.position.y > minY + 0.9*scrollStepSize) :
					$CombatMap.position.y -= scrollStepSize
				
func goHome() :
	$CombatMap.position = homePosition
	
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var connections = $CombatMap/ConnectionContainer.get_children()
	for index in range (connections.size()) :
		var key : String = "connection" + str(index)
		tempDict[key] = connections[index].getVisibility()
	var rooms = $CombatMap/RoomContainer.get_children()
	for index in range (rooms.size()) :
		var key : String = "room" + str(index)
		tempDict[key+"visibility"] = rooms[index].getVisibility()
		tempDict[key+"override"] = rooms[index].getOverridePath()
		tempDict[key+"override_once"] = rooms[index].getOverrideOnce()
	return tempDict
	
var myReady : bool = false
func _ready() :
	myReady = true
	
func beforeLoad(_newSave) :
	for child in $CombatMap/RoomContainer.get_children() :
		child.connect("levelChosen", _on_level_chosen)
	initScroll()
	
func onLoad(loadDict) -> void :
	var connections = $CombatMap/ConnectionContainer.get_children()
	for index in range (connections.size()) :
		var key : String = "connection" + str(index) 
		connections[index].setVisibility(loadDict[key])
	var rooms = $CombatMap/RoomContainer.get_children()
	for index in range (rooms.size()) :
		var key : String = "room" + str(index)
		rooms[index].setVisibility(loadDict[key + "visibility"])
		var overridePath = loadDict[key+"override"]
		if (overridePath != "null") :
			rooms[index].overrideEncounter(load(overridePath), loadDict[key+"override_once"])

func _on_home_button_was_selected(_emitter) -> void:
	goHome()
