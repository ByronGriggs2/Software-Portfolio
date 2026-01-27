extends HBoxContainer

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

var currentLayer = 0
func setLayer(val) :
	currentLayer = val
			
func setEquipment(item : Equipment) :
	var stuff = item.equipmentGroups
	var tech = $Technology.get_children()
	for index in range(0, tech.size()) :
		if (index == stuff.technology as int && (!stuff.isSignature) && stuff.isEligible) :
			tech[index].visible = true
		else :
			tech[index].visible = false
	var off = $Offense.get_children()
	for index in range(0, off.size()) :
		if (index == stuff.weaponClass as int && (!stuff.isSignature) && stuff.isEligible) :
			off[index].visible = true
		else :
			off[index].visible = false
	var def = $Defense.get_children()
	for index in range(0, def.size()) :
		if (index == stuff.armorClass as int && (!stuff.isSignature) && stuff.isEligible) :
			def[index].visible = true
		else :
			def[index].visible = false
	if (stuff.isSignature) :
		$Signature.get_child(0).visible = true
	else :
		$Signature.get_child(0).visible = false
	updateAllRecursive(get_children())
	addTooltipsRecursive(get_children())
	for child in get_children() :
		for child2 in child.get_children() :
			child2.get_child(0).get_child(1).currentLayer = currentLayer
	
func updateAllRecursive(children : Array[Node]) :
	for child in children :
		if (child.has_method("updateSize")) :
			child.updateSize()
		if (child.get_child_count() > 0) :
			updateAllRecursive(child.get_children())

func addTooltipsRecursive(children : Array[Node]) :
	for child in children :
		if (child.has_method("updateSize") && child.visible) :
			addTooltip(child)
		if (child.get_child_count() > 0) :
			addTooltipsRecursive(child.get_children())
			
const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
func addTooltip(child : Node) :
	if (child.get_child_count() == 2) :
		var oldTooltip = child.get_child(1)
		child.remove_child(oldTooltip)
		oldTooltip.queue_free()
	var title : String = child.name
	var upperLeft = Vector2(0,0)
	var bottomRight = Vector2(16,16) * child.getScale() + Vector2(10,10)
	var newTrigger = tooltipLoader.instantiate()
	child.add_child(newTrigger)
	newTrigger.initialise(title)
	#newTrigger.currentLayer = Helpers.getTopLayer()
	newTrigger.setPos(upperLeft, bottomRight)
