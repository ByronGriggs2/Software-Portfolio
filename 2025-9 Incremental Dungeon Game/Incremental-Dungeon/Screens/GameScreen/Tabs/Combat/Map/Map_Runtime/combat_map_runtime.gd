extends Panel

signal levelChosen
signal tutorialRequested

#######################################
## Getters
func getTypicalEnemyDefense() :
	var numArr : Array[float] = []
	for room in $CombatMap/RoomContainer.get_children() :
		if (room.has_method("getEncounterRef") && room.getEncounterRef() != null) :
			for enemy in room.getEncounterRef().enemies :
				numArr.append(enemy.PHYSDEF)
				numArr.append(enemy.MAGDEF)
	numArr.sort_custom(func(a,b):return a<b)
	if (numArr.size()%2 != 0) :
		return numArr[(numArr.size()-1)/2]
	else :
		return (numArr[(numArr.size()-2)/2]+numArr[numArr.size()/2])/2.0
func getBossName() -> String :
	if (mapData == null) :
		return ""
	var boss = mapData.bossEncounter.enemies[0]
	return boss.getName()
func getEnvironment() -> MyEnvironment :
	var ret : MyEnvironment
	if (mapData == null) :
		ret = MyEnvironment.new()
	else :
		ret = MegaFile.getEnvironment(mapData.environmentName)
	return ret
func getRoomRow(room : Node) :
	return getRow(room)
		
#######################################
## Setters
var UIEnabled : bool = true
func disableUI() :
	UIEnabled = false
	$HBoxContainer.visible = false
func enableUI() :
	UIEnabled = true
	$HBoxContainer.visible = true
#######################################
## Callbacks
func onCombatLoss(room) :
	room.onCombatLoss()
func onCombatRetreat(room) :
	room.onCombatRetreat()
func _on_level_chosen(emitter) :
	emit_signal("levelChosen", emitter, emitter.getEncounterRef())
signal shopRequested
func _on_shop_requested(details : ShopDetails, room) :
	completeRoom(room)
	emit_signal("shopRequested", details)
func _on_home_button_was_selected(_emitter) -> void:
	goHome()
func _on_resized() -> void:
	if (fullyInitialised) :
		initScroll()
		setFromMapPosRatio()
signal addChildRequested
func _on_add_child_requested(emitter, node : Node) :
	emit_signal("addChildRequested", emitter, node)
signal mapCompleted
func completeRoom(completedRoom) :
	checkHerophile(completedRoom)
	checkApophis(completedRoom)
	checkDragon(completedRoom)
	var firstCompletion = !completedRoom.isCompleted()
	completedRoom.onCombatComplete()
	if (firstCompletion || (completedRoom.has_method("isEmpty") && completedRoom.isEmpty())) :
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
				if (adjacentRoom.has_signal("shopRequested")) :
					adjacentRoom.setVisibility(2)
				else :
					adjacentRoom.setVisibility("full")
				connection.fullReveal()
				#Find rooms that are 2 links away
				for potentialLooseConnection in $CombatMap/ConnectionContainer.get_children() :
					var overAdjacentRoom = null
					if (potentialLooseConnection.Room1 == adjacentRoom) : 
						overAdjacentRoom = potentialLooseConnection.Room2
					elif (potentialLooseConnection.Room2 == adjacentRoom) : 
						overAdjacentRoom = potentialLooseConnection.Room1
					#Half reveal
					if (overAdjacentRoom != null && overAdjacentRoom.has_signal("shopRequested") && overAdjacentRoom.getVisibility() != 2) :
						overAdjacentRoom.setVisibility(1)
						potentialLooseConnection.halfReveal()
					elif (overAdjacentRoom != null && overAdjacentRoom.getVisibility() != 2) :
						overAdjacentRoom.setVisibility("partial")
						potentialLooseConnection.halfReveal()
		var completedIndex = Helpers.findIndexInContainer($CombatMap/RoomContainer, completedRoom)
		if (completedIndex == $CombatMap/RoomContainer.get_child_count()-1) :
			emit_signal("mapCompleted", self)
######################################
## Internal
##This prevents a warning
func doNotCall() :
	emit_signal("tutorialRequested")
#####################################
## Set from MapData
const roomLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/Map_Runtime/room_runtime.tscn")
const connectionLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/connection.tscn")
var mapData : MapData = null
func setFromMapData(val : MapData) :
	mapData = val
	for index in range(0,val.rows.size()) :
		addRow(val, index)
	addBossRoom(val)
	for child in $CombatMap/RoomContainer.get_children() :
		if (child.has_signal("myReadySignal") && !child.myReady) :
			await child.myReadySignal
	if ($CombatMap/RoomContainer.get_node("N0").has_method("isEmpty") && $CombatMap/RoomContainer.get_node("N0").isEmpty()) :
		completeRoom($CombatMap/RoomContainer.get_node("N0"))
const shopLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/shop_room_base.tscn")
const emptyLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/Map_Runtime/emptyRoom_runtime.tscn")
func addRow(val : MapData, row : int) :
	var centerRoom
	## Either an empty row or a shop
	if (val.shopName != "" && row == 0) :
		if (val.shopName == "empty") :
			centerRoom = emptyLoader.instantiate()
			$CombatMap/RoomContainer.add_child(centerRoom)
		else :
			centerRoom = shopLoader.instantiate()
			$CombatMap/RoomContainer.add_child(centerRoom)
			centerRoom.setShopType(val.shopName)
			centerRoom.setName(Shopping.shopNames[val.shopName])
	else :
		centerRoom = roomLoader.instantiate()
		$CombatMap/RoomContainer.add_child(centerRoom)
		var visibility
		if (row == 0) :
			visibility = 2
		elif (row == 1) :
			visibility = 1
		else :
			visibility = 0
		centerRoom.initialise(val.rows[row].centralEncounter, visibility)
	centerRoom.name = "N" + str(row)
	setRoomToOrigin(centerRoom)
	setRoomPosVertical(centerRoom, row, false)
	if (row != 0) :
		var newConnection = connectionLoader.instantiate()
		$CombatMap/ConnectionContainer.add_child(newConnection)
		newConnection.Room1 = $CombatMap/RoomContainer.get_node("N"+str(row-1))
		newConnection.Room2 = centerRoom
		if (row == 1) :
			if (val.shopName == "empty") :
				newConnection.visibilityOnStartup = 2
				newConnection.setVisibility(2)
			else :
				newConnection.visibilityOnStartup = 1
				newConnection.setVisibility(1)
		elif (row == 2 && val.shopName == "empty") :
			newConnection.visibilityOnStartup = 1
			newConnection.setVisibility(1)
		else :
			newConnection.setVisibility(0)
	for index in range(0, val.rows[row].leftEncounters.size()) :
		addSideRoom(val, row, index, true)
	for index in range(0, val.rows[row].rightEncounters.size()) :
		addSideRoom(val, row, index, false)
	
func addSideRoom(val : MapData, row : int, leafCounter : int, isLeft : bool) :
	var newRoom = roomLoader.instantiate()
	$CombatMap/RoomContainer.add_child(newRoom)
	if (isLeft) :
		newRoom.initialise(val.rows[row].leftEncounters[leafCounter], 0)
	else :
		newRoom.initialise(val.rows[row].rightEncounters[leafCounter], 0)
	if (row == 0 && leafCounter == 0) :
		newRoom.setVisibility(1)
	var leftRightChar
	if (isLeft) :
		leftRightChar = "L"
	else :
		leftRightChar = "R"
	newRoom.name = "N" + str(row) + leftRightChar + str(leafCounter)
	setRoomToOrigin(newRoom)
	setRoomPosVertical(newRoom, row, false)
	var horizontalPos = leafCounter+1
	if (isLeft) :
		horizontalPos *= -1
	setRoomPosHorizontal(newRoom, horizontalPos)
	var newConnection = connectionLoader.instantiate()
	$CombatMap/ConnectionContainer.add_child(newConnection)
	newConnection.Room2 = newRoom
	if (leafCounter == 0) :
		newConnection.Room1 = $CombatMap/RoomContainer.get_node("N"+str(row))
	else :
		newConnection.Room1 = $CombatMap/RoomContainer.get_node("N"+str(row)+leftRightChar+str(leafCounter-1))
	if (row == 0 && leafCounter == 0) :
		if (val.shopName == "empty") :
			newConnection.visibilityOnStartup = 2
		else :
			newConnection.visibilityOnStartup = 1
	elif (row == 0 && leafCounter == 1 && val.shopName == "empty"):
		newConnection.visibilityOnStartup = 1
	elif (row == 1 && leafCounter == 0 && val.shopName == "empty"):
		newConnection.visibilityOnStartup = 1
		
func addBossRoom(val : MapData) :
	#var tempEncounter = val.bossEncounter
	var bossRoom = roomLoader.instantiate()
	$CombatMap/RoomContainer.add_child(bossRoom)
	bossRoom.initialise(val.bossEncounter, 0)
	bossRoom.name = "N" + str(val.rows.size())
	setRoomToOrigin(bossRoom)
	setRoomPosVertical(bossRoom, val.rows.size(), true)
	var bossConnection = connectionLoader.instantiate()
	$CombatMap/ConnectionContainer.add_child(bossConnection)
	bossConnection.Room1 = $CombatMap/RoomContainer.get_node("N"+str(val.rows.size()-1))
	bossConnection.Room2 = bossRoom
	
		
func setRoomToOrigin(room : Button) :
	room.anchor_left = 0.5
	room.anchor_right = 0.5
	room.anchor_top = 1.0
	room.anchor_bottom = 1.0
	room.offset_bottom = -20
	room.offset_top = 0
	room.offset_left = 0
	room.offset_right = 0
	room.grow_horizontal = Control.GROW_DIRECTION_BOTH
	room.grow_vertical = Control.GROW_DIRECTION_BEGIN
func setRoomPosHorizontal(room : Button, amount : int) :
	room.offset_left = 300*amount
	room.offset_right = 300*amount
func setRoomPosVertical(room : Button, amount : int, isBoss : bool) :
	if (isBoss) :
		room.offset_bottom += -200*(amount-1) - 300
	else :
		room.offset_bottom += -200*amount
#######################################################
## Construction
## It is not safe to call beforeLoad() or onLoad() before constructing the map
## with this function.
## This function constructs itself as well as its rooms and calls initialise() for each
## room. room.beforeLoad() and room.onLoad() is left for map.beforeLoad() and map.onLoad().
var fullyInitialised : bool = false
signal fullyInitialisedSignal
func initialise(val) :
	if (val is MapData) :
		await setFromMapData(val)
		setupRoomConnections()
		clip_contents = true
		initScroll()
		goHome()
		fullyInitialised = true
		emit_signal("fullyInitialisedSignal")
	elif (val == null || (val is String && val == "null")) :
		return
	else :
		initialise(MapData.createFromSaveDictionary(val["mapData"]))
		
func getRowTotal() -> int :
	var highest = 0
	for room in $CombatMap/RoomContainer.get_children() :
		if ((room.name as String).find("L") != -1 || (room.name as String).find("R") != -1) :
			continue
		var num = int((room.name as String).substr(1))
		if (num > highest) :
			highest = num
	return highest
func getRow(room : Node) :
	if ((room.name as String).find("L") != -1 || (room.name as String).find("R") != -1) :
		var cutoff = (room.name as String).find("L")
		if (cutoff == -1) :
			cutoff = (room.name as String).find("R")
		var parentName = (room.name as String).substr(0,(room.name as String).length()-cutoff)
		return int(parentName.substr(1))
	else :
		return int((room.name as String).substr(1))
	
func getFurthestProgression() -> int :
	var highest = 0
	for room in $CombatMap/RoomContainer.get_children() :
		if (!room.isCompleted() || (room.name as String).find("L") != -1 || (room.name as String).find("R") != -1) :
			continue
		var num = int((room.name as String).substr(1))
		if (num > highest) :
			highest = num
	return highest
	
func setupRoomConnections() :
	for child in $CombatMap/RoomContainer.get_children() :
		if (child.has_signal("levelChosen")) :
			child.connect("levelChosen", _on_level_chosen)
		if (child.has_signal("shopRequested")) :
			child.connect("shopRequested", _on_shop_requested)
		if (child.has_signal("addChildRequested")) :
			child.connect("addChildRequested", _on_add_child_requested)
			
## Intended function call pseudocode from parent class
## var mapData = createNewMap()
##      OR
## var mapData = loadCreatedMap()
## var newMap = mapLoader.instantiate()
## add_child(newMap)
## newMap.initialise(mapData)
## newMap.beforeLoad()
## if (mapData was from loadCreatedMap()) :
## newMap.onLoad(loadData)
##
#######################################################
## Manual Saving
var myReady : bool = false
signal myReadySignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
	
func beforeLoad() :
	for room in $CombatMap/RoomContainer.get_children() :
		if (room.has_signal("myReadySignal")) :
			room.beforeLoad()
	for connection in $CombatMap/ConnectionContainer.get_children() :
		connection.updatePos()
	
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	tempDict["mapPosX"] = mapPosRatio.x
	tempDict["mapPosY"] = mapPosRatio.y
	var connections = $CombatMap/ConnectionContainer.get_children()
	for index in range (connections.size()) :
		tempDict["connection"+str(index)] = connections[index].getVisibility()
	var rooms = $CombatMap/RoomContainer.get_children()
	for index in range (rooms.size()) :
		tempDict["room"+str(index)] = rooms[index].getSaveDictionary()
	if (mapData == null) :
		tempDict["mapData"] = "null"
	else :
		tempDict["mapData"] = mapData.getSaveDictionary()
	return tempDict

func onLoad(loadDict) -> void :
	var connections = $CombatMap/ConnectionContainer.get_children()
	for index in range (connections.size()) :
		var key : String = "connection" + str(index) 
		connections[index].setVisibility(loadDict[key])
	var rooms = $CombatMap/RoomContainer.get_children()
	for index in range (rooms.size()) :
		var key : String = "room" + str(index)
		rooms[index].onLoad(loadDict[key])
	mapPosRatio = Vector2(loadDict["mapPosX"], loadDict["mapPosY"])
	setFromMapPosRatio()
	
######################################################
## Map navigation bullshit
## Settable variables
@export var scrollStepSize : int = 80

## Saveable variables
var mapPosRatio : Vector2 = Vector2(-99,-99)

## Runtime variables 
var homePosition : Vector2
var minX : float
var maxX : float
var minY : float
var maxY : float

func setMapPosRatio() :
	if (minX > maxX) :
		mapPosRatio.x = -1
	else :
		mapPosRatio.x = $CombatMap.position.x/(maxX-minX)
	if (minY > maxY) :
		mapPosRatio.y = -1
	else :
		mapPosRatio.y = $CombatMap.position.y/(maxY-minY)

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

var debounceTimer = 0
func _process(delta) :
	debounceTimer += delta
	if (dragging) :
		var posDif = get_global_mouse_position()-dragPos
		$CombatMap.global_position += posDif
		dragPos = get_global_mouse_position()
var dragging : bool = false
var dragPos : Vector2 = Vector2(0,0)
func _gui_input(event : InputEvent) :
	if (event is InputEventMouseButton && (event.button_index == MOUSE_BUTTON_LEFT || event.button_index == MOUSE_BUTTON_RIGHT)) :
		accept_event()
		dragging = event.pressed
		if (dragging) :
			dragPos = get_global_mouse_position()
func _unhandled_input(event: InputEvent) -> void :
	if (!is_visible_in_tree() || !UIEnabled) :
		return
	if (Helpers.popupIsPresent()) :
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
	
func setFromMapPosRatio() :
	if (mapPosRatio.x == -99 || (minX > maxX)) :
		$CombatMap.position.x = size.x/2-$CombatMap.size.x/2
	else :
		$CombatMap.position.x = (maxX-minX)*mapPosRatio.x
	if (mapPosRatio.y == -99 || (minY>maxY)) :
		$CombatMap.position.y = size.y/2-$CombatMap.size.y/2
	else :
		$CombatMap.position.y = (maxY-minY)*mapPosRatio.y

func checkHerophile(room) :
	if (room.has_method("getEncounterRef")) :
		var encounter : Encounter = room.getEncounterRef()
		if (encounter != null) :
			for enemy in encounter.enemies :
				if (enemy.getResourceName() == "champion_of_poseidon") :
					emit_signal("tutorialRequested", Encyclopedia.tutorialName.herophile, Vector2(0,0))
					return
func checkDragon(room) :
	if (room.has_method("getEncounterRef")) :
		var encounter : Encounter = room.getEncounterRef()
		if (encounter != null) :
			for enemy in encounter.enemies :
				var myName = enemy.getName()
				if (myName.to_upper().find("DRAGON") != -1) :
					SteamWrapper.unlockAchievement(Definitions.achievementEnum.legend)
signal apophisKilled
func checkApophis(room) :
	if (room.has_method("getEncounterRef")) :
		var encounter : Encounter = room.getEncounterRef()
		if (encounter != null) :
			for enemy in encounter.enemies :
				if (enemy.getResourceName() == "apophis") :
					emit_signal("apophisKilled")
					return
