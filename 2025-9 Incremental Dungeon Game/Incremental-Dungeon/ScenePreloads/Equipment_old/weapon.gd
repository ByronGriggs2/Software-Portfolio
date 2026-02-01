extends "res://ScenePreloads/Equipment_old/equipment.gd"

func getType() -> Definitions.equipmentTypeEnum :
	return core.type
func getAttack() -> float :
	return core.attackBonus
#func getPrimaryAttribute() -> Definitions.attributeEnum :
	#return core.primaryAttribute
func getBasicAttack() -> AttackAction :
	return core.basicAttack
func isUnarmed() -> bool :
	return core.isUnarmed
