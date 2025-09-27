extends Control

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
			
