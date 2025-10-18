extends Control

#########################
##shared among all instances of this scene##
@export var core : Equipment = null
#########################
##unique to this scene##
var equipped : bool = false
@export var myScale : Vector2
func _ready() :
	$SuperButton/HBoxContainer/Sprite.scale = myScale
#########################
##setters##
func equip() :
	equipped = true
func unequip() :
	equipped = false
func setVisibility(val : bool) :
	visible = val
func select() :
	$SuperButton.select()
func deselect() :
	$SuperButton.deselect()
########################
##getters##
func isEquipped() -> bool :
	return equipped
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

signal wasSelected
func _on_super_button_was_selected(emitter) -> void:
	emit_signal("wasSelected", self)

# This scene is not "saveable" but is explicitly saved by EquipmentTab/Inventory
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	return tempDict
func loadSaveDictionary(_loadDict) -> void :
	pass
func getSprite() :
	return $SuperButton/HBoxContainer/Sprite
func getItemName() :
	return core.resource_path.get_file().get_basename()
