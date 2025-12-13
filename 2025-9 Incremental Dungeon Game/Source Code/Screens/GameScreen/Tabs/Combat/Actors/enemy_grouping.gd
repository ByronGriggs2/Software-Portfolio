extends Resource

class_name EnemyGroups
## All enemies need to set these
@export var isEligible : bool = true
enum enemyQualityEnum {normal,veteran,elite}
const enemyQualityDictionary = {
	enemyQualityEnum.normal : "normal",
	enemyQualityEnum.veteran : "veteran",
	enemyQualityEnum.elite : "elite"
}
@export var enemyQuality : enemyQualityEnum = enemyQualityEnum.normal
@export var droppedArmorClasses : Array[EquipmentGroups.armorClassEnum] = []
@export var droppedWeaponClasses : Array[EquipmentGroups.weaponClassEnum] = []
@export var droppedTechnologyClasses : Array[EquipmentGroups.technologyEnum] = []

##Humanoid types
@export var isHumanoid : bool = false
@export var isGreenskin : bool = false
@export var isMerfolk : bool = false
@export var isDemon : bool = false

## Climate resistance
@export var isHot : bool = false
@export var isCold : bool = false

## Non-humanoid types
@export var isAnimal : bool = false
@export var isDragon : bool = false
@export var isUndead : bool = false
@export var isPlant : bool = false
@export var isElemental : bool = false
@export var isFey : bool = false
@export var isMindless : bool = false
