@tool
extends Equipment

class_name Weapon
@export var primaryAttribute : Definitions.attributeEnum
@export var attackBonus : float
@export var basicAttack : AttackAction
@export var isUnarmed : bool = false
var type = Definitions.equipmentTypeEnum.weapon
