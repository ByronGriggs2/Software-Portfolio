extends Node
const currentVersion : String = "V0.6 development"
var GODMODE : bool = false
var DEVMODE : bool = false

func _ready() :
	attributeCount = 0
	for key in attributeDictionary :
		attributeCount += 1
	if (currentVersion != "V0.6 development") :
		GODMODE = false
		DEVMODE = false

## Not every saveable scene needs to have a load dependency name, but every saveable scene
## that has a dependency or is a dependency does.
enum loadDependencyName {
	player,
	inventory
}
const loadDependencyNameArray = [
	loadDependencyName.player,
	loadDependencyName.inventory
]
		
## Doesn't need to be all inclusive, but you shouldn't save any dictionaries
## that have enum keys not included in this list
#enum enumType {
	#baseStatEnum, 
	#attributeEnum, 
	#classEnum, 
	#saveSlots, 
	#scalingEnum, 
	#equipmentTypeEnum, 
	#damageTypeEnum,
	#optionType,
	#options,
	#tutorialName,
	#tutorialType
	#}
#const enumTypeDictionary = {
	#enumType.baseStatEnum : "baseStatEnum",
	#enumType.attributeEnum : "attributeEnum",
	#enumType.classEnum : "classEnum",
	#enumType.saveSlots : "saveSlots",
	#enumType.scalingEnum : "scalingEnum",
	#enumType.equipmentTypeEnum : "equipmentTypeEnum",
	#enumType.damageTypeEnum : "damageTypeEnum",
	#enumType.optionType : "optionType",
	#enumType.options : "option",
	#enumType.tutorialName : "tutorialName",
	#enumType.tutorialType : "tutorialType"
#}

enum baseStatEnum {
	MAXHP,AR,DR,PHYSDEF,MAGDEF
}
const baseStatDictionary = {
	baseStatEnum.MAXHP : "Max HP",
	baseStatEnum.AR : "Attack Rating",
	baseStatEnum.DR : "Damage Rating",
	baseStatEnum.PHYSDEF : "Physical Defense",
	baseStatEnum.MAGDEF : "Magic Defense"
}
const baseStatDictionaryShort = {
	baseStatEnum.MAXHP : "MAXHP",
	baseStatEnum.AR : "AR",
	baseStatEnum.DR : "DR",
	baseStatEnum.PHYSDEF : "PHYSDEF",
	baseStatEnum.MAGDEF : "MAGDEF"
}
enum attributeEnum {
	DEX,DUR,INT,SKI,STR
}
const attributeDictionary = {
	attributeEnum.DEX : "Dexterity",
	attributeEnum.DUR : "Durability",
	attributeEnum.INT : "Intelligence",
	attributeEnum.SKI : "Skill",
	attributeEnum.STR : "Strength"
}
const attributeDictionaryShort = {
	attributeEnum.DEX : "DEX",
	attributeEnum.DUR : "DUR",
	attributeEnum.INT : "INT",
	attributeEnum.SKI : "SKI",
	attributeEnum.STR : "STR"
}
enum otherStatEnum {
	magicFind
}
const otherStatDictionary = {
	otherStatEnum.magicFind : "Magic Find"
}
enum classEnum {
	fighter,mage,rogue
}
var attributeCount : int
const classDictionary = {
	classEnum.fighter : "Fighter",
	classEnum.mage : "Mage",
	classEnum.rogue: "Rogue"
}
enum saveSlots {
	nullVal,slot0,slot1,slot2,slot3,current
}
var slotPaths = {
	saveSlots.current : "",
	saveSlots.slot0 : "user://save_slot_0.json",
	saveSlots.slot1 : "user://save_slot_1.json",
	saveSlots.slot2 : "user://save_slot_2.json",
	saveSlots.slot3 : "user://save_slot_3.json"
}
var slotNames = {
	saveSlots.slot0 : "save slot 0",
	saveSlots.slot1 : "save slot 1",
	saveSlots.slot2 : "save slot 2",
	saveSlots.slot3 : "save slot 3"
}
const mainSettingsPath = "user://main_settings.json"
#enum scalingEnum {
	#STR,INT,DEX
#}
enum equipmentTypeEnum {
	weapon,armor,accessory,currency
}
const equipmentTypeDictionary = {
	equipmentTypeEnum.weapon : "Weapon",
	equipmentTypeEnum.armor : "Armor",
	equipmentTypeEnum.accessory : "Accessory",
	equipmentTypeEnum.currency : "Currency"
}
const equippableDictionary = {
	equipmentTypeEnum.weapon : "Weapon",
	equipmentTypeEnum.armor : "Armor",
	equipmentTypeEnum.accessory : "Accessory",
}
func isEquippable(item) :
	if (item is Node) :
		return equippableDictionary.get(item.getType()) != null
	elif (item is Equipment) :
		return equippableDictionary.get(item.type) != null
enum damageTypeEnum {
	physical,magic
}
const damageTypeDictionary = {
	damageTypeEnum.physical : "Physical",
	damageTypeEnum.magic : "Magic"
}
const equipmentPaths = {
	equipmentTypeEnum.weapon : "res://Global Scene Preloads/Equipment/Resources/Weapon",
	equipmentTypeEnum.armor : "res://Global Scene Preloads/Equipment/Resources/Armor",
	equipmentTypeEnum.accessory : "res://Global Scene Preloads/Equipment/Resources/Accessory",
	equipmentTypeEnum.currency : "res://Global Scene Preloads/Equipment/Resources/Currency"
}
