extends Node
const currentVersion : String = "V0.4 development"

func _ready() :
	attributeCount = 0
	for key in attributeDictionary :
		attributeCount += 1

enum baseStatEnum {
	MAXHP,AR,PHYSDEF,MAGDEF
}
const baseStatDictionary = {
	baseStatEnum.MAXHP : "Max HP",
	baseStatEnum.AR : "Attack Rating",
	baseStatEnum.PHYSDEF : "Physical Defense",
	baseStatEnum.MAGDEF : "Magic Defense"
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

var playerClassGlobalCopy = null
func setPlayerClass(newClass : classEnum) :
	playerClassGlobalCopy = newClass
func getPlayerClass() :
	return playerClassGlobalCopy

func getUnarmedPath() :
	return equipmentPaths[equipmentTypeEnum.weapon] + "unarmed_" + classDictionary[playerClassGlobalCopy] + ".tres"
