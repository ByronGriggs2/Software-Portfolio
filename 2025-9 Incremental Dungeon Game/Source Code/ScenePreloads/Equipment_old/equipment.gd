extends VBoxContainer

#########################
##shared among all instances of this scene##
@export var core : Equipment = null
enum rewardBehaviour{wait,alwaysTake,alwaysDiscard}
var myBehaviour : rewardBehaviour = rewardBehaviour.wait
#########################
##unique to this scene##
var equipped : bool = false
@export var myScale : Vector2
func _ready() :
	$SuperButton/HBoxContainer/Sprite.setScale(myScale.x)
#########################
##setters##
func equip() :
	equipped = true
func unequip() :
	equipped = false
func setVisibility(val : bool) :
	visible = val
func setRewardBehaviour(val : rewardBehaviour) :
	myBehaviour = val
########################
##getters##
func isEquipped() -> bool :
	return equipped
func getSprite() :
	return $SuperButton/HBoxContainer/Sprite
func getItemName() :
	return core.getItemName()
func addToModifierPacket(packetRef : ModifierPacket) -> ModifierPacket :
	if (core == null) :
		return
	return ModifierPacket.add(packetRef, core.getModifierPacket())
func getModPacketRef() -> ModifierPacket :
	if (core == null) :
		return null
	return core.getModifierPacket()
func getRewardBehaviour() -> rewardBehaviour :
	return myBehaviour
func getQuality() -> EquipmentGroups.qualityEnum :
	return core.equipmentGroups.quality
func getTextureCopy() -> Texture2D :
	return getSprite().get_node("Icon").texture.duplicate()
var reforges : int = 0
func reforge(scalingVal, rows) :
	core = core.getAdjustedCopy(scalingVal)
	if (rows != 0) :
		var baseTitle = EquipmentDatabase.getEquipment(core.getItemName()).getTitle()
		var newTitle = baseTitle + " +" + str(rows-1)
		core.setTitle(newTitle)
	reforges += 1
func getReforges() -> int :
	return reforges
func getName() -> String :
	return core.getName()
###################################################################################
##This scene is not "saveable" but is explicitly saved by EquipmentTab/Inventory##
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	if (core == null) :
		tempDict["core"] = "null"
	else :
		tempDict["core"] = core.getSaveDictionary()
	tempDict["reforges"] = reforges
	return tempDict
func loadSaveDictionary(loadDict) -> void :
	if (loadDict.get("core") != null) :
		if (!(loadDict["core"] is Dictionary) && loadDict["core"] == "null") :
			core = null
		else :
			## If it's old equipment, sets to resource reference rather than creating a duplicates
			var temp = EquipmentDatabase.getEquipment(loadDict["core"]["resourceName"])
			core = temp.createFromSaveDictionary(loadDict["core"])
	reforges = loadDict["reforges"]
##########################################################
##SuperButton proxies##
func setContainerExpandHorizontal() :
	size_flags_horizontal = SizeFlags.SIZE_EXPAND_FILL
	$SuperButton.setContainerExpandHorizontal()
func setContentMargin(val) :
	$SuperButton.setContentMargin(val)
func setSeparation(val) :
	$SuperButton.setSeparation(val)
signal wasSelected
func _on_super_button_was_selected(_emitter) -> void:
	emit_signal("wasSelected", self)
signal wasDeselected
func _on_super_button_was_deselected(_emitter) -> void :
	emit_signal("wasDeselected", self)
func select() :
	$SuperButton.select()
func deselect() :
	$SuperButton.deselect()
func isSelected() :
	return $SuperButton.isSelected()
func setTheme(val) :
	theme = val
	$SuperButton.setTheme(val)
func addToContainer(val :Control) :
	$SuperButton.addToContainer(val)
#########################################################################
##Equipment Proxies##
func getDesc() -> String :
	return core.description
func getTitle() -> String :
	return core.title
func addStatBonus(currentBonus : Dictionary) -> void :
	for key in currentBonus.keys() :
		currentBonus[key] += core.statBonus[Definitions.baseStatDictionary[key]]
func addAttributeBonus(currentBonus : Dictionary) -> void :
	for key in currentBonus.keys() :
		currentBonus[key] += core.attributeBonus[Definitions.attributeDictionary[key]]
##########################################################################
func use48x48() :
	var sprite = $SuperButton/HBoxContainer/Sprite
	sprite.setScale(3)
func setMouseFilter(enumVal) :
	mouse_filter = enumVal
	$SuperButton.mouse_filter = enumVal

const passiveLoader = preload("res://Graphic Elements/Buttons/passive_super_button.tscn")
func createGhostCopy() -> Node :
	var topLevel = HBoxContainer.new()
	var button = passiveLoader.instantiate()
	topLevel.add_child(button)
	button.contentMargin = $SuperButton.contentMargin
	var spriteDuplicate = $SuperButton/HBoxContainer/Sprite.duplicate(true)
	button.addToContainer(spriteDuplicate)
	return topLevel
