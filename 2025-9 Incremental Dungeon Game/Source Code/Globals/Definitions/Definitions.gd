extends Node
const currentVersion : String = "V0.5 development"
var GODMODE : bool = true

func _ready() :
	attributeCount = 0
	for key in attributeDictionary :
		attributeCount += 1
	if (currentVersion != "V0.5 development") :
		GODMODE = false

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
	MAXHP,AR,PHYSDEF,MAGDEF
}
const baseStatDictionary = {
	baseStatEnum.MAXHP : "Max HP",
	baseStatEnum.AR : "Attack Rating",
	baseStatEnum.PHYSDEF : "Physical Defense",
	baseStatEnum.MAGDEF : "Magic Defense"
}
const baseStatDictionaryShort = {
	baseStatEnum.MAXHP : "MAXHP",
	baseStatEnum.AR : "AR",
	baseStatEnum.PHYSDEF : "PHYSDEF",
	baseStatEnum.MAGDEF : "MAGDEF"
}
enum attributeEnum {
	DEX,DUR,INT,STR
}
const attributeDictionary = {
	attributeEnum.DEX : "Dexterity",
	attributeEnum.DUR : "Durability",
	attributeEnum.INT : "Intelligence",
	attributeEnum.STR : "Strength"
}
const attributeDictionaryShort = {
	attributeEnum.DEX : "DEX",
	attributeEnum.DUR : "DUR",
	attributeEnum.INT : "INT",
	attributeEnum.STR : "STR"
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
enum scalingEnum {
	STR,INT,DEX
}
enum equipmentTypeEnum {
	weapon,armor,accessory
}
const equipmentTypeDictionary = {
	equipmentTypeEnum.weapon : "Weapon",
	equipmentTypeEnum.armor : "Armor",
	equipmentTypeEnum.accessory : "Accessory"
}
enum damageTypeEnum {
	physical,magic
}
const equipmentPaths = {
	equipmentTypeEnum.weapon : "res://Screens/GameScreen/Tabs/EquipmentTab/EquipmentItems/Weapons/",
	equipmentTypeEnum.armor : "res://Screens/GameScreen/Tabs/EquipmentTab/EquipmentItems/Armor/",
	equipmentTypeEnum.accessory : "res://Screens/GameScreen/Tabs/EquipmentTab/EquipmentItems/Accessories/"
}
