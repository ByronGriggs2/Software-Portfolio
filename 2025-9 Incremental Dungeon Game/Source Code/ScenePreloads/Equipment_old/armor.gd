extends "res://ScenePreloads/Equipment_old/equipment.gd"

func getPhysicalDefense() -> float :
	return core.PHYSDEF
func getMagicDefense() -> float :
	return core.MAGDEF
func getType() -> Definitions.equipmentTypeEnum :
	return Definitions.equipmentTypeEnum.armor
