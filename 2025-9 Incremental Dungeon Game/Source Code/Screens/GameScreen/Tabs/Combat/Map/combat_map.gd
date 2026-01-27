extends Panel

signal levelChosen
signal tutorialRequested

var UIEnabled : bool = true
func disableUI() :
	UIEnabled = false
	$HBoxContainer.visible = false
func enableUI() :
	UIEnabled = true
	$HBoxContainer.visible = true
	
func onCombatLoss(room) :
	room.onCombatLoss()
func onCombatRetreat(room) :
	room.onCombatRetreat()

##This prevents a warning
func doNotCall() :
	emit_signal("tutorialRequested")
################################
#map
func _on_level_chosen(emitter) :
	emit_signal("levelChosen", emitter, emitter.getEncounterRef())
	
signal mapCompleted
func completeRoom(completedRoom) :
	var firstCompletion = !completedRoom.isCompleted()
	completedRoom.onCombatComplete()
	if (firstCompletion) :
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
				adjacentRoom.setVisibility("full")
				connection.fullReveal()
				if (adjacentRoom.isShop) :
					completeRoom(adjacentRoom)
				#Find rooms that are 2 links away
				for potentialLooseConnection in $CombatMap/ConnectionContainer.get_children() :
					var overAdjacentRoom = null
					if (potentialLooseConnection.Room1 == adjacentRoom) : 
						overAdjacentRoom = potentialLooseConnection.Room2
					elif (potentialLooseConnection.Room2 == adjacentRoom) : 
						overAdjacentRoom = potentialLooseConnection.Room1
					#Half reveal
					if (overAdjacentRoom != null && overAdjacentRoom.getVisibility() != 2) :
						overAdjacentRoom.setVisibility("partial")
						potentialLooseConnection.halfReveal()
		var completedIndex = Helpers.findIndexInContainer($CombatMap/RoomContainer, completedRoom)
		if (completedIndex == $CombatMap/RoomContainer.get_child_count()-1) :
			emit_signal("mapCompleted", self)
	
#scroll
var mapPosRatio : Vector2 = Vector2(-99,-99)
func setMapPosRatio() :
	if (minX > maxX) :
		mapPosRatio.x = -1
	else :
		mapPosRatio.x = $CombatMap.position.x/(maxX-minX)
	if (minY > maxY) :
		mapPosRatio.y = -1
	else :
		mapPosRatio.y = $CombatMap.position.y/(maxY-minY)

@export var scrollStepSize : int = 80
var homePosition : Vector2
var minX : float
var maxX : float
var minY : float
var maxY : float

func initScroll() :
	#var mapSize = $CombatMap.size
	#var viewportSize = size
	var tempPos = Vector2(-$CombatMap.size.x/2.0, -$CombatMap.size.y)
	#await(get_tree().process_frame)
	if (minX > maxX) :
		homePosition.x = size.x/2-$CombatMap.size.x/2
	else :
		homePosition.x = tempPos.x+(size.x/2.0)
	if (minY > maxY) :
		homePosition.y = size.y/2-$CombatMap.size.y/2
	else :
		homePosition.y = tempPos.y+size.y
	
	setBoundaries()
	
func setBoundaries() :
	maxY = homePosition.y+$CombatMap.size.y-size.y
	minY = homePosition.y
	maxX = homePosition.x + ($CombatMap.size.x/2.0)-(size.x/2.0)
	minX = homePosition.x - ($CombatMap.size.x/2.0)+(size.x/2.0)

func forceBoundaries() :
	if ($CombatMap.position.x < minX) :
		$CombatMap.position.x = minX
	elif ($CombatMap.position.x > maxX) :
		$CombatMap.position.x = maxX
	elif ($CombatMap.position.y < minY) :
		$CombatMap.position.y = minY
	elif ($CombatMap.position.y > maxY) :
		$CombatMap.position.y = maxY
		
#func _on_visibility_changed() -> void:
#	if (visible) :
#		grab_focus()

var debounceTimer = 0
func _process(delta) :
	if (!doneLoading) :
		return
	if (!myReady) :
		return
	debounceTimer += delta
func _unhandled_input(event: InputEvent) -> void:
	if (!is_visible_in_tree() || !UIEnabled) :
		return
	if (debounceTimer < 0.1 && !event.is_echo()) :
		return
	var changed : bool = false
	if (event.is_action("ui_left")) :
		accept_event()
		if ($CombatMap.position.x < maxX - 0.9*scrollStepSize) :
				$CombatMap.position.x += scrollStepSize
		changed = true
	elif (event.is_action("ui_right")) :
		accept_event()
		if ($CombatMap.position.x > minX + 0.9*scrollStepSize) :
				$CombatMap.position.x -= scrollStepSize
		changed = true
	elif (event.is_action("ui_up")) :
		accept_event()
		if ($CombatMap.position.y < maxY - 0.9*scrollStepSize) :
				$CombatMap.position.y += scrollStepSize
		changed = true
	elif (event.is_action("ui_down")) :
		accept_event()
		if ($CombatMap.position.y > minY + 0.9*scrollStepSize) :
				$CombatMap.position.y -= scrollStepSize
		changed = true
	if (changed) :
		setMapPosRatio()
		debounceTimer = 0
	
func goHome() :
	$CombatMap.position = homePosition
	setMapPosRatio()
	
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var connections = $CombatMap/ConnectionContainer.get_children()
	for index in range (connections.size()) :
		var key : String = "connection" + str(index)
		tempDict[key] = connections[index].getVisibility()
	var rooms = $CombatMap/RoomContainer.get_children()
	for index in range (rooms.size()) :
		var key : String = "room" + str(index)
		tempDict[key+"override"] = rooms[index].getOverridePath()
		tempDict[key+"override_once"] = rooms[index].getOverrideOnce()
	tempDict["mapPosX"] = mapPosRatio.x
	tempDict["mapPosY"] = mapPosRatio.y
	return tempDict	
	
var fullyInitialised : bool = false
	
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
	
func beforeLoad(newSave) :
	myReady = false
	clip_contents = true
	for child in $CombatMap/RoomContainer.get_children() :
		child.connect("levelChosen", _on_level_chosen)
		if (child.has_signal("shopRequested")) :
			child.connect("shopRequested", _on_shop_requested)
	initScroll()
	goHome()
	if (newSave) :
		fullyInitialised = true
	myReady = true
	emit_signal("myReadySignal")
	if (newSave) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
		
func getTypicalEnemyDefense() :
	var numArr : Array[float] = []
	for room in $CombatMap/RoomContainer.get_children() :
		for enemy in room.getEncounterRef().enemies :
			numArr.append(enemy.PHYSDEF)
			numArr.append(enemy.MAGDEF)
	if (numArr.size()%2 != 0) :
		return numArr[(numArr.size()-1)/2]
	else :
		return (numArr[(numArr.size()-2)/2]+numArr[numArr.size()/2])/2.0
	
signal shopRequested
func _on_shop_requested(details : ShopDetails) :
	emit_signal("shopRequested", details)
	
func onLoad(loadDict) -> void :
	myReady = false
	var connections = $CombatMap/ConnectionContainer.get_children()
	for index in range (connections.size()) :
		var key : String = "connection" + str(index) 
		connections[index].setVisibility(loadDict[key])
	var rooms = $CombatMap/RoomContainer.get_children()
	for index in range (rooms.size()) :
		var key : String = "room" + str(index)
		var overridePath = loadDict[key+"override"]
		if (overridePath != "null") :
			rooms[index].overrideEncounter(load(overridePath), loadDict[key+"override_once"])
	mapPosRatio = Vector2(loadDict["mapPosX"], loadDict["mapPosY"])
	setFromMapPosRatio()
	fullyInitialised = true
	_on_resized()
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")
	
func setFromMapPosRatio() :
	if (mapPosRatio.x == -99 || (minX > maxX)) :
		$CombatMap.position.x = size.x/2-$CombatMap.size.x/2
	else :
		$CombatMap.position.x = (maxX-minX)*mapPosRatio.x
	if (mapPosRatio.y == -99 || (minY>maxY)) :
		$CombatMap.position.y = size.y/2-$CombatMap.size.y/2
	else :
		$CombatMap.position.y = (maxY-minY)*mapPosRatio.y
	
func _on_home_button_was_selected(_emitter) -> void:
	goHome()

func _on_resized() -> void:
	if (fullyInitialised) :
		initScroll()
		setFromMapPosRatio()

func _on_combat_map_resized() -> void:
	pass
	#if (mapViewportRatio == Vector2(-99,-99)) :
		#return
	#size = Vector2(1/(mapViewportRatio.x/$CombatMap.size.x),1/(mapViewportRatio.y/$CombatMap.size.y))
