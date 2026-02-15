class_name EnemyGroups
extends Resource


## All enemies need to set these
@export var isEligible : bool = true
@export var glassCannonIndex : float = 1.0
@export var HPToDefRatio : float = 1.0
@export var defRatio : float = 1.0
@export var atkRatio : float = 1.0

enum enemyTechEnum {none, poor, well, elite, dragon}
const enemyEquipmentDictionary = {
	enemyTechEnum.none : "Unequipped",
	enemyTechEnum.poor : "Poorly Equipped",
	enemyTechEnum.well : "Well Equipped",
	enemyTechEnum.elite : "Elite",
	enemyTechEnum.dragon : "Dragon's Hoard"
}
@export var equipmentLevel : enemyTechEnum = -1 
enum enemyQualityEnum {normal, veteran, elite}
const enemyQualityDictionary = {
	enemyQualityEnum.normal : "Grunt",
	enemyQualityEnum.veteran : "Veteran",
	enemyQualityEnum.elite : "Boss"
}
@export var enemyQuality : enemyQualityEnum = enemyQualityEnum.normal
enum enemyArmorEnum {magical,resistant,hardened,ironclad}
const enemyArmorDictionary = {
	enemyArmorEnum.magical : "Magical",
	enemyArmorEnum.resistant : "Resistant",
	enemyArmorEnum.hardened: "Hardened",
	enemyArmorEnum.ironclad : "Ironclad"
}
@export var enemyArmor : enemyArmorEnum = -1
enum enemyRangeEnum {noDesc, melee, ranged}
const enemyRangeDictionary = {
	enemyRangeEnum.noDesc : "",
	enemyRangeEnum.melee : "Vanguard",
	enemyRangeEnum.ranged : "Backline"
}
@export var enemyRange : enemyRangeEnum = enemyRangeEnum.noDesc
##Humanoid types
enum factionEnum {
	fire,
	water,
	earth,
	ice,
	undead,
	demonic_military,
	demonic,
	merfolk,
	greenskin,
	misc
}
const factionDictionary = {
	factionEnum.fire : "Flamekin",
	factionEnum.water : "Nautikin",
	factionEnum.earth : "Naturekin",
	factionEnum.ice : "Frostkin",
	factionEnum.undead : "Undead",
	factionEnum.demonic_military : "Demonic Military",
	factionEnum.demonic : "Demonic Civilian",
	factionEnum.merfolk : "Merfolk",
	factionEnum.greenskin : "Greenskin",
	factionEnum.misc : "Wanderkin"
}
@export var faction : factionEnum
