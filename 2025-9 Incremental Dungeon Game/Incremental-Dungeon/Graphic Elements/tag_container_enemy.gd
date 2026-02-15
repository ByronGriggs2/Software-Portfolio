extends HBoxContainer

##red green blue
const colors = ["#d62910","#57a968","#4566c3", "#d4941c"]
func _ready() :
	for child in $Technology.get_children() :
		putInPanel(child, colors[2])
	for child in $Defense.get_children() :
		putInPanel(child, colors[1])
	for child in $Offense.get_children() :
		putInPanel(child, colors[0])
	for child in $Signature.get_children() :
		putInPanel(child, colors[3])
	setSpriteSizeRecursive(get_children(), 3)
	
func putInPanel(child : Node, color : String):
	var newPanel = $Panel.duplicate()
	child.get_parent().add_child(newPanel)
	child.get_parent().remove_child(child)
	newPanel.add_child(child)
	child.position = Vector2(5,5)
	var panelTheme : StyleBox = newPanel.get_theme_stylebox("panel").duplicate()
	panelTheme.border_color = Color(color)
	newPanel.add_theme_stylebox_override("panel", panelTheme)
	
func setSpriteSizeRecursive(children : Array[Node], val) :
	for child in children :
		if (child.has_method("setScale")) :
			child.setScale(val)
			child.visible = true
		setSpriteSizeRecursive(child.get_children(), val)

func setEnemy(enemy : ActorPreset) :
	var stuff = enemy.enemyGroups
	var tech = $Technology.get_children()
	for index in range(0, tech.size()) :
		if (index == stuff.equipmentLevel as int) :
			tech[index].visible = true
		else :
			tech[index].visible = false
	var off = $Offense.get_children()
	for index in range(0, off.size()) :
		if (index + 1 == stuff.enemyRange as int && stuff.equipmentLevel != EnemyGroups.enemyTechEnum.dragon ) :
			off[index].visible = true
		else :
			off[index].visible = false
	var def = $Defense.get_children()
	for index in range(0, def.size()) :
		if (index == stuff.enemyArmor as int && stuff.equipmentLevel != EnemyGroups.enemyTechEnum.dragon) :
			def[index].visible = true
		else :
			def[index].visible = false
	var sig = $Signature.get_children()
	for index in range(0, sig.size()) :
		if (index + 1 == stuff.enemyQuality as int && !(enemy.getResourceName() == "apophis")) :
			sig[index].visible = true
		else :
			sig[index].visible = false
	for tagContainer in get_children() :
		var disable : bool = true
		for tag in tagContainer.get_children() :
			if (tag.visible) :
				disable = false
		if (disable) :
			tagContainer.visible = false
		else :
			tagContainer.visible = true
	updateAllRecursive(get_children())
	var topLayer = Helpers.getTopLayer()
	addTooltipsRecursive(get_children(), topLayer)
	
func updateAllRecursive(children : Array[Node]) :
	for child in children :
		if (child.has_method("updateSize")) :
			child.updateSize()
		if (child.get_child_count() > 0) :
			updateAllRecursive(child.get_children())

func addTooltipsRecursive(children : Array[Node], topLayer) :
	for child in children :
		if (child.has_method("updateSize") && child.visible) :
			addTooltip(child, topLayer)
		if (child.get_child_count() > 0) :
			addTooltipsRecursive(child.get_children(), topLayer)
			
const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
func addTooltip(child : Node, topLayer) :
	var title : String = child.name
	var upperLeft = Vector2(0,0)
	var bottomRight = Vector2(16,16) * child.getScale() + Vector2(10,10)
	var newTrigger = tooltipLoader.instantiate()
	child.add_child(newTrigger)
	newTrigger.initialise(title)
	newTrigger.currentLayer = topLayer
	newTrigger.setPos(upperLeft, bottomRight)
