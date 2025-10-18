#This auto-generated singleton preloads small nodes such as equipment and can instantiate them as needed
extends Node

#Equipment section
const equipmentResourceDictionary = {
#Weapon
	"magic_stick_int" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/magic_stick_int.tres"),
	"magic_stick_str" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/magic_stick_str.tres"),
	"shiv" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/shiv.tres"),
	"unarmed_Fighter" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/unarmed_Fighter.tres"),
	"unarmed_Mage" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/unarmed_Mage.tres"),
	"unarmed_Rogue" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/unarmed_Rogue.tres"),
#Armor
	"casual" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/casual.tres"),
	"scraps" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/scraps.tres"),
#Accessory
	"ring_mild_fortitude" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/ring_mild_fortitude.tres"),
	"silver_bracers" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/silver_bracers.tres")}

const equipmentSceneDictionary = {
#Weapon
	"magic_stick_int" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon//magic_stick_int.tscn"),
	"magic_stick_str" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon//magic_stick_str.tscn"),
	"shiv" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon//shiv.tscn"),
	"unarmed_Fighter" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon//unarmed_Fighter.tscn"),
	"unarmed_Mage" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon//unarmed_Mage.tscn"),
	"unarmed_Rogue" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon//unarmed_Rogue.tscn"),
#Armor
	"casual" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor//casual.tscn"),
	"scraps" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor//scraps.tscn"),
#Accessory
	"ring_mild_fortitude" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory//ring_mild_fortitude.tscn"),
	"silver_bracers" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory//silver_bracers.tscn")}

func createEquipmentScene(itemName : String) :
	var newScene = equipmentSceneDictionary[itemName].instantiate()
	newScene.core = equipmentResourceDictionary[itemName]
	return newScene
