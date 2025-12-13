extends "res://Graphic Elements/popups/my_popup_button.gd"

var placeholderSprite = preload("res://Images/PlaceholderCharacterGraphic.jpg")

func setEnemy(enemy : ActorPreset) :
	## Add pic later
	getSpriteRef().texture = enemy.portrait
	getTitleRef().text = enemy.getName()
	getHBOX().get_node("Star").visible = EnemyDatabase.getAllEnemyDropsCollected(enemy.getResourceName())
	var killCount = EnemyDatabase.getKillCount(enemy.getResourceName())
	if (killCount > 0) :
		getHBOX().get_node("Skull").visible = true
	var description : String = ""
	for key in Definitions.baseStatDictionary.keys() :
		description += Definitions.baseStatDictionary[key] + ": " + str(Helpers.myRound(enemy.getStat(key),3)) + "\n"
	description += "Actions:\n"
	var attack = enemy.actions[0]
	description += "\t1. " + attack.getName() + "\n"
	description += "\t\tType: " + Definitions.damageTypeDictionary[attack.getDamageType()] + "\n"
	description += "\t\tPower: " + str(Helpers.myRound(attack.getPower(),3)) + "\n"
	description += "\t\tWarmup: " + str(Helpers.myRound(attack.getWarmup(),3)) + " seconds\n"
	description += "\nKilled: " + str(int(killCount)) + "\n\n"
	description += "Drops\n"
	if (enemy.drops.is_empty()) :
		description += "\t-None-\n"
	else :
		for index in range(0,enemy.drops.size()) :
				## swap to item encyclopedia later???
				if (EnemyDatabase.getItemObtained(enemy.getResourceName(), enemy.drops[index].getName())) :
					description += enemy.drops[index].getName() + " - " + str(100*enemy.dropChances[index]) + "%\n"
				else :
					description += "??? (" + Definitions.equipmentTypeDictionary[enemy.drops[index].type] + ") - " + str(100*enemy.dropChances[index]) + "%\n"
	description += "\n"
	description += enemy.getDesc()
	setText(description)
	
func getTitleRef() :
	return $Panel/CenterContainer/Window/ScrollContainer/VBoxContainer/Title
func getSpriteRef() :
	return $Panel/CenterContainer/Window/ScrollContainer/VBoxContainer/Art
func getHBOX() :
	return $Panel/CenterContainer/Window/ScrollContainer/VBoxContainer/HBoxContainer
	
var myReady
signal myReadySignal
func _ready() :
	var myWindow = $Panel/CenterContainer/Window
	var myContainer : ScrollContainer = myWindow.get_node("ScrollContainer")
	var myVBox = myWindow.get_node("VBoxContainer")
	myWindow.remove_child(myVBox)
	myContainer.add_child(myVBox)
	myVBox.size_flags_vertical = Control.SIZE_EXPAND_FILL
	myVBox.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	myReady = true
	emit_signal("myReadySignal")
