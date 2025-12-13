#This auto-generated singleton preloads small nodes such as equipment and can instantiate them as needed
extends Node

#Equipment section
const equipmentResourceDictionary = {
#Weapon
	"arming_sword" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/arming_sword.tres"),
	"bat_bat" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/bat_bat.tres"),
	"bat_staff" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/bat_staff.tres"),
	"cat_claws" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/cat_claws.tres"),
	"estoc" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/estoc.tres"),
	"executioners_sword" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/executioners_sword.tres"),
	"goose_flail" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/goose_flail.tres"),
	"kiwi_dagger" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/kiwi_dagger.tres"),
	"magic_stick_int" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/magic_stick_int.tres"),
	"magic_stick_str" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/magic_stick_str.tres"),
	"manticore_dart" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/manticore_dart.tres"),
	"manticore_wand" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/manticore_wand.tres"),
	"mole_pickaxe" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/mole_pickaxe.tres"),
	"ram_spear" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/ram_spear.tres"),
	"rapier" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/rapier.tres"),
	"ruby_staff" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/ruby_staff.tres"),
	"shiv" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/shiv.tres"),
	"shortsword" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/shortsword.tres"),
	"sling" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/sling.tres"),
	"unarmed_Fighter" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/unarmed_Fighter.tres"),
	"unarmed_Mage" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/unarmed_Mage.tres"),
	"unarmed_Rogue" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/unarmed_Rogue.tres"),
	"wakizashi" : preload("res://Global Scene Preloads/Equipment/Resources/Weapon/wakizashi.tres"),
#Armor
	"casual" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/casual.tres"),
	"enchanted_robes" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/enchanted_robes.tres"),
	"feathered_cloak" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/feathered_cloak.tres"),
	"lion_armor" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/lion_armor.tres"),
	"manticore_armor" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/manticore_armor.tres"),
	"mole_armor" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/mole_armor.tres"),
	"scraps" : preload("res://Global Scene Preloads/Equipment/Resources/Armor/scraps.tres"),
#Accessory
	"clown_horn" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/clown_horn.tres"),
	"kiwi" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/kiwi.tres"),
	"magpie_eye" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/magpie_eye.tres"),
	"mandragora_potion" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/mandragora_potion.tres"),
	"manticore_tooth" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/manticore_tooth.tres"),
	"manticore_venom" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/manticore_venom.tres"),
	"ram_horn" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/ram_horn.tres"),
	"ring_mild_fortitude" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/ring_mild_fortitude.tres"),
	"silver_bracers" : preload("res://Global Scene Preloads/Equipment/Resources/Accessory/silver_bracers.tres"),
#Currency
	"pebble" : preload("res://Global Scene Preloads/Equipment/Resources/Currency/pebble.tres")}

const equipmentSceneDictionary = {
#Weapon
	"arming_sword" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/arming_sword.tscn"),
	"bat_bat" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/bat_bat.tscn"),
	"bat_staff" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/bat_staff.tscn"),
	"cat_claws" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/cat_claws.tscn"),
	"estoc" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/estoc.tscn"),
	"executioners_sword" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/executioners_sword.tscn"),
	"goose_flail" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/goose_flail.tscn"),
	"kiwi_dagger" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/kiwi_dagger.tscn"),
	"magic_stick_int" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/magic_stick_int.tscn"),
	"magic_stick_str" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/magic_stick_str.tscn"),
	"manticore_dart" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/manticore_dart.tscn"),
	"manticore_wand" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/manticore_wand.tscn"),
	"mole_pickaxe" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/mole_pickaxe.tscn"),
	"ram_spear" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/ram_spear.tscn"),
	"rapier" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/rapier.tscn"),
	"ruby_staff" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/ruby_staff.tscn"),
	"shiv" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/shiv.tscn"),
	"shortsword" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/shortsword.tscn"),
	"sling" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/sling.tscn"),
	"unarmed_Fighter" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/unarmed_Fighter.tscn"),
	"unarmed_Mage" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/unarmed_Mage.tscn"),
	"unarmed_Rogue" : preload("res://Global Scene Preloads/Equipment/Scenes/Weapon/unarmed_Rogue.tscn"),
#Armor
	"casual" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor/casual.tscn"),
	"enchanted_robes" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor/enchanted_robes.tscn"),
	"feathered_cloak" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor/feathered_cloak.tscn"),
	"lion_armor" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor/lion_armor.tscn"),
	"manticore_armor" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor/manticore_armor.tscn"),
	"mole_armor" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor/mole_armor.tscn"),
	"scraps" : preload("res://Global Scene Preloads/Equipment/Scenes/Armor/scraps.tscn"),
#Accessory
	"clown_horn" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/clown_horn.tscn"),
	"kiwi" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/kiwi.tscn"),
	"magpie_eye" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/magpie_eye.tscn"),
	"mandragora_potion" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/mandragora_potion.tscn"),
	"manticore_tooth" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/manticore_tooth.tscn"),
	"manticore_venom" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/manticore_venom.tscn"),
	"ram_horn" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/ram_horn.tscn"),
	"ring_mild_fortitude" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/ring_mild_fortitude.tscn"),
	"silver_bracers" : preload("res://Global Scene Preloads/Equipment/Scenes/Accessory/silver_bracers.tscn"),
#Currency
	"pebble" : preload("res://Global Scene Preloads/Equipment/Scenes/Currency/pebble.tscn")}

func createEquipmentScene(itemName : String) :
	var newScene = equipmentSceneDictionary[itemName].instantiate()
	newScene.core = equipmentResourceDictionary[itemName]
	return newScene
