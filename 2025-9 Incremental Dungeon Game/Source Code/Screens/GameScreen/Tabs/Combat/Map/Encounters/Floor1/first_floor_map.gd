extends "res://Screens/GameScreen/Tabs/Combat/Map/combat_map.gd"
		
const popupLoader = preload("res://Graphic Elements/popups/my_popup_button.tscn")
const starterClassWeapons = [
	preload("res://Screens/GameScreen/Tabs/Combat/Map/Encounters/Floor1/tutorial_weapon_Fighter.tres"),
	preload("res://Screens/GameScreen/Tabs/Combat/Map/Encounters/Floor1/tutorial_weapon_Mage.tres"),
	preload("res://Screens/GameScreen/Tabs/Combat/Map/Encounters/Floor1/tutorial_weapon_Rogue.tres")
]

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
func completeLevel(completedRoom) :
	var playerClass = await getPlayerClass()
	var tutorialPos = $CombatMap/RoomContainer/Room2.position
	tutorialPos.x += $CombatMap/RoomContainer/Room2.size.x
	if (completedRoom == $CombatMap/RoomContainer/Room1) :
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.tutorialFloor2, Vector2(0,0))
	elif (completedRoom == $CombatMap/RoomContainer/Room2) :
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.tutorialFloor4, Vector2(0,0))
		$CombatMap/RoomContainer/WeaponRoom.overrideEncounter(starterClassWeapons[playerClass.classEnum], true)
	elif (completedRoom == $CombatMap/RoomContainer/OrcRoom) :
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.tutorialFloorEnd, Vector2(0,0))
	super(completedRoom)
	
func onCombatLoss(room : Node) :
	if (room == $CombatMap/RoomContainer/Room2) :
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.tutorialFloor3, Vector2(0,0))
	super(room)
		
#func launchGoblinPopup() :
	#var popup = popupLoader.instantiate()
	#add_child(popup)
	#popup.setTitle("Ouch!")
	#popup.setText("Those goblins are really tough! Maybe you missed something in a previous room that could help?")
	#await popup.finished
	#var classCopy = playerRef.getClass()
	#var override = starterClassWeapons[classCopy.classEnum]
	#$CombatMap/RoomContainer/CombatMap/RoomContainer/Room1.overrideEncounter(override, true)
