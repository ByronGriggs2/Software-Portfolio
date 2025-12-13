extends "res://Screens/GameScreen/Tabs/Combat/Map/combat_map.gd"

signal routineUnlockRequested
func completeRoom(completedRoom) :
	var firstCompletion = !completedRoom.isCompleted()
	super(completedRoom)
	if (firstCompletion) :
		if (completedRoom == $CombatMap/RoomContainer/N1L1 || completedRoom == $CombatMap/RoomContainer/N1R1) :
			emit_signal("tutorialRequested", Encyclopedia.tutorialName.dropIntro, Vector2(0,0))
		if (completedRoom == $CombatMap/RoomContainer/N2L1) :
			emit_signal("routineUnlockRequested", RoutineReferences.getRoutine("Spar with dummy"))
		if (completedRoom == $CombatMap/RoomContainer/N4) :
			emit_signal("tutorialRequested", Encyclopedia.tutorialName.manticoreKill, Vector2(0,0))
			
