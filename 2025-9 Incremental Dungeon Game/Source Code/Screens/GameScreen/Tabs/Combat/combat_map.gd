extends Panel

signal levelChosen2

func _ready() :
	$RoomContainer.get_child(0).myEnable()
	for child in $RoomContainer.get_children() :
		child.connect("levelChosen", _on_level_chosen)
		
func _on_level_chosen(emitter) :
	emit_signal("levelChosen2", emitter, emitter.encounter)
	
func completeLevel(emitter) :
	#For all rooms, find rooms linked to this one
	for child in $ConnectionContainer.get_children() :
		var otherSide = null
		if (child.Room1 == emitter) : otherSide = child.Room2
		if (child.Room2 == emitter) : otherSide = child.Room1
		#For rooms linked to this one
		if (otherSide != null) :
			#Fully reveal
			otherSide.myEnable()
			child.myEnable()
			#Find rooms that are 2 links away
			for innerChild in $ConnectionContainer.get_children() :
				var looseConnection = null
				if (innerChild.Room1 == otherSide) : looseConnection = innerChild.Room2
				elif (innerChild.Room2 == otherSide) : looseConnection = innerChild.Room1
				#Half reveal
				if (looseConnection != null) :
					looseConnection.visible = true
					innerChild.visible = true
			
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var connections = $ConnectionContainer.get_children()
	for index in range (connections.size()) :
		var key : String = "connection" + str(index)
		tempDict[key] = connections[index].getVisibility()
	var rooms = $RoomContainer.get_children()
	for index in range (rooms.size()) :
		var key : String = "room" + str(index)
		tempDict[key] = rooms[index].getVisibility()
	return tempDict
	
func setFromSaveDictionary(loadDict) -> void :
	var connections = $ConnectionContainer.get_children()
	for index in range (connections.size()) :
		var key : String = "connection" + str(index) 
		if (loadDict[key] == 0) :
			connections[index].visible = false
			connections[index].myDisable()
		elif (loadDict[key] == 1) :
			connections[index].visible = true
			connections[index].myDisable()
		elif (loadDict[key] == 2) :
			connections[index].visible = true
			connections[index].myEnable()
		else :
			pass
	var rooms = $RoomContainer.get_children()
	for index in range (rooms.size()) :
		var key : String = "room" + str(index)
		if (loadDict[key] == 0) :
			rooms[index].visible = false
			rooms[index].myDisable()
		elif (loadDict[key] == 1) :
			rooms[index].visible = true
			rooms[index].myDisable()
		elif (loadDict[key] == 2) :
			rooms[index].visible = true
			rooms[index].myEnable()
		else :
			pass
