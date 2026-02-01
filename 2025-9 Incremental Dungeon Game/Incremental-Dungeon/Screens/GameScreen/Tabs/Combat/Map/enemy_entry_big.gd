extends "res://Graphic Elements/popups/my_popup_button.gd"

var placeholderSprite = preload("res://Images/PlaceholderCharacterGraphic.jpg")

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
func addTooltip() :
	var newTooltip = tooltipLoader.instantiate()
	var sprite = $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/Sprite
	sprite.add_child(newTooltip)
	newTooltip.setTitle("Tutorial Enemy")
	newTooltip.setDesc("This enemy is part of the tutorial, and is not eligible to spawn in Biomes or drop random equipment.")
	newTooltip.currentLayer = Helpers.getTopLayer()
	var upperLeft = Vector2(0,0)
	var bottomRight = Vector2(16,16)*sprite.getScale()
	newTooltip.setPos(upperLeft, bottomRight)

func setEnemy(enemy : ActorPreset) :
	getSpriteRef().texture = enemy.portrait
	getTitleRef().text = enemy.getName()
	getHBOX().get_node("Star").visible = EnemyDatabase.getAllEnemyDropsCollected(enemy.getResourceName())
	var killCount = EnemyDatabase.getKillCount(enemy.getResourceName())
	if (killCount > 0) :
		getHBOX().get_node("Skull").visible = true
	var description : String = ""
	for key in Definitions.baseStatDictionary.keys() :
		description += Definitions.baseStatDictionary[key] + ": " + Helpers.engineeringRound(enemy.getStat(key),3) + "\n"
	description += "Action - "
	var attack = enemy.actions[0]
	description += attack.getName() + "\n"
	description += "\tType: " + Definitions.damageTypeDictionary[attack.getDamageType()] + "\n"
	description += "\tPower: " + Helpers.engineeringRound(attack.getPower(),3) + "\n"
	description += "\tWarmup: " + Helpers.engineeringRound(attack.getWarmup(),3) + " seconds\n"
	description += "\nKilled: " + str(int(killCount)) + "\n\n"
	#description += "Drops\n"
	#if (enemy.drops.is_empty()) :
		#description += "\t-None-\n"
	#else :
		#for index in range(0,enemy.drops.size()) :
				### swap to item encyclopedia later???
				#if (EnemyDatabase.getItemObtained(enemy.getResourceName(), enemy.drops[index].getName())) :
					#description += enemy.drops[index].getName() + " - " + str(100*enemy.dropChances[index]) + "%\n"
				#else :
					#description += "??? (" + Definitions.equipmentTypeDictionary[enemy.drops[index].type] + ") - " + str(100*enemy.dropChances[index]) + "%\n"
	#description += "\n"
	description += enemy.getDesc()
	setText(description)
	
func getTitleRef() :
	return $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/Title
func getSpriteRef() :
	return $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/Art/Art
func getHBOX() :
	return $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/HBoxContainer
	
var myReady
signal myReadySignal
func _ready() :
	var myWindow = $Panel/CenterContainer/Window
	var myContainer : PanelContainer = myWindow.get_node("ScrollContainer").get_node("PanelContainer")
	var myVBox = myWindow.get_node("VBoxContainer")
	myWindow.remove_child(myVBox)
	myContainer.add_child(myVBox)
	myVBox.size_flags_vertical = Control.SIZE_EXPAND_FILL
	myVBox.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	if ($Panel/CenterContainer/Window/ScrollContainer/PanelContainer.get_child_count() == 0) :
		await $Panel/CenterContainer/Window/ScrollContainer/PanelContainer.child_order_changed
	addTooltip()
	myReady = true
	emit_signal("myReadySignal")
