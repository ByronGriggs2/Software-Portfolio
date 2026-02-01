extends "res://Graphic Elements/popups/my_popup_button.gd"

var placeholderSprite = preload("res://Images/PlaceholderCharacterGraphic.jpg")

func initialise(smallEntry : Node) :
	setEnemy(smallEntry.getEnemy())
	
func setEnemy(enemy : ActorPreset) :
	getSpriteRef().texture = enemy.portrait
	getTitleRef().text = enemy.getName()
	getTagContainer().setEnemy(enemy)
	setFactionSymbol(enemy)
	var killCount = EnemyDatabase.getKillCount(enemy.getResourceName())
	writeDescription(enemy, killCount)
	if (Definitions.hasDLC && enemy.resourceName == "athena") :
		getTagContainer().visible = false

func writeDescription(enemy : ActorPreset, killCount) :
	var description : String = ""
	description += "Standard Stats:\n"
	var baseEnemy = enemy.getAdjustedCopy(1)
	for key in Definitions.baseStatDictionary.keys() :
		description += "\t" + Definitions.baseStatDictionary[key] + ": " + str(Helpers.myRound(baseEnemy.getStat(key),3)) + "\n"
	if (enemy.getResourceName() == "apophis" || enemy.getResourceName() == "athena") :
		description += "\n[font_size=25]Attacks[/font_size]\n"
		for attack in enemy.actions :
			description = writeAttack(attack, description)
	else :
		description += "\nAttack - "
		var attack = enemy.actions[0]
		description = writeAttack(attack, description)
	
	description += "\nKilled: " + str(int(killCount)) + "\n\n"
	#description += "\n"
	description += enemy.getDesc()
	setText(description)

func writeAttack(attack, val) -> String :
	var description = val
	description += attack.getName() + "\n"
	if (attack is AttackAction) :
		description += "\t\tType: " + Definitions.damageTypeDictionary[attack.getDamageType()] + "\n"
		description += "\t\tPower: " + str(Helpers.myRound(attack.getPower(),3)) + "\n"
	description += "\t\tWarmup: " + str(Helpers.myRound(attack.getWarmup(),3)) + " seconds\n"
	return description
	
const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
func setFactionSymbol(enemy : ActorPreset) :
	var currentLayer = Helpers.getTopLayer()
	var children = getFactionSymbolContainer().get_children()
	for index in range(0,children.size()) :
		if (index == enemy.enemyGroups.faction as int) :
			children[index].visible = true
			var upperLeft = Vector2(0,0)
			var bottomRight = Vector2(16,16)*children[index].getScale()
			var newTooltip = tooltipLoader.instantiate()
			children[index].add_child(newTooltip)
			newTooltip.initialise(EnemyGroups.factionDictionary[index])
			newTooltip.currentLayer = currentLayer
			newTooltip.setPos(upperLeft, bottomRight)
		else :
			children[index].visible = false
	
func getTitleRef() :
	return $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/Title
func getSpriteRef() :
	return $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/Art/Art
func getHBOX() :
	return $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/HBoxContainer
func getTagContainer() :
	return $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/TagContainerEnemy
func getFactionSymbolContainer() :
	return $Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/FactionSymbol
	
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
	myReady = true
	emit_signal("myReadySignal")
