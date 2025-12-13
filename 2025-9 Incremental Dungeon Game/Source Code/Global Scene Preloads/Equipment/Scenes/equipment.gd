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
func getRewardBehaviour() -> rewardBehaviour :
	return myBehaviour
###################################################################################
##This scene is not "saveable" but is explicitly saved by EquipmentTab/Inventory##
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	return tempDict
func loadSaveDictionary(_loadDict) -> void :
	pass
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
