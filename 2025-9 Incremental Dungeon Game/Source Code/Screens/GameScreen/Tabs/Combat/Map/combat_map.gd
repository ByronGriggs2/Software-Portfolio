extends Control

signal levelChosen2

func onCombatLoss(room) :
	room.onCombatComplete()

################################
#map
func _on_level_chosen(emitter) :
	emit_signal("levelChosen2", emitter, emitter.getEncounterRef())
	
func completeLevel(emitter) :
	emitter.onCombatComplete()
	#For all rooms, find rooms linked to this one
	for child in $CombatMap/ConnectionContainer.get_children() :
		var otherSide = null
		if (child.Room1 == emitter) : otherSide = child.Room2
		if (child.Room2 == emitter) : otherSide = child.Room1
		#For rooms linked to this one
		if (otherSide != null) :
			#Fully reveal
			otherSide.myEnable()
			child.fullReveal()
			#Find rooms that are 2 links away
			for innerChild in $CombatMap/ConnectionContainer.get_children() :
				var looseConnection = null
				if (innerChild.Room1 == otherSide) : looseConnection = innerChild.Room2
				elif (innerChild.Room2 == otherSide) : looseConnection = innerChild.Room1
				#Half reveal
				if (looseConnection != null) :
					looseConnection.visible = true
					innerChild.halfReveal()
		
#scroll
@export var scrollStepSize : int = 80
var homePosition : Vector2
var minX : int
var maxX : int
var minY : int
var maxY : int

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
	$CombatMap/RoomContainer.get_child(0).myEnable()
	for child in $CombatMap/RoomContainer.get_children() :
		child.connect("levelChosen", _on_level_chosen)
	initScroll()
	
func onLoad(loadDict) -> void :
	var connections = $CombatMap/ConnectionContainer.get_children()
	for index in range (connections.size()) :
		var key : String = "connection" + str(index) 
		if (loadDict[key] == 0) :
			connections[index].visible = false
		elif (loadDict[key] == 1) :
			connections[index].fullReveal()
		elif (loadDict[key] == 2) :
			connections[index].halfReveal()
		else :
			pass
	var rooms = $CombatMap/RoomContainer.get_children()
	for index in range (rooms.size()) :
		var key : String = "room" + str(index)
		if (loadDict[key+"visibility"] == 0) :
			rooms[index].visible = false
			rooms[index].myDisable()
		elif (loadDict[key+"visibility"] == 1) :
			rooms[index].visible = true
			rooms[index].myDisable()
		elif (loadDict[key+"visibility"] == 2) :
			rooms[index].visible = true
			rooms[index].myEnable()
		else :
			pass
		var overridePath = loadDict[key+"override"]
		if (overridePath != "null") :
			rooms[index].overrideEncounter(load(overridePath), loadDict[key+"override_once"])
			
func afterLoad() :
	pass
