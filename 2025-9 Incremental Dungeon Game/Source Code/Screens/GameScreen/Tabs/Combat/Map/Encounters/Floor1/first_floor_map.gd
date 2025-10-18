extends "res://Screens/GameScreen/Tabs/Combat/Map/combat_map.gd"

var goblinDeathCount = 0
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	tempDict["first_floor_property_1"] = goblinDeathCount
	tempDict["super"] = super()
	return tempDict
	
func onLoad(loadDict : Dictionary) :
	goblinDeathCount = loadDict["first_floor_property_1"]
	super(loadDict["super"])

func onCombatLoss(room) :
	if (room == $CombatMap/RoomContainer/Room3) :
		goblinDeathCount += 1
		if (goblinDeathCount == 3) :
			await launchGoblinPopup()
		
const popupLoader = preload("res://Graphic Elements/popups/my_popup_button.tscn")
func launchGoblinPopup() :
	var popup = popupLoader.instantiate()
	add_child(popup)
	popup.setTitle("Ouch!")
	popup.setText("Those goblins are really tough! Maybe you missed something in a previous room that could help?")
	await popup.finished
	var path = "res://Screens/GameScreen/Tabs/Combat/Map/Encounters/Floor1/tutorial_weapon_" + Definitions.classDictionary[Definitions.getPlayerClass()] + ".tres"
	$CombatMap/RoomContainer/Room1.overrideEncounter(load(path), true)
