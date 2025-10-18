extends "res://Global Scene Preloads/Equipment/Scenes/equipment.gd"

func getPhysicalDefense() -> float :
	return core.PHYSDEF
func getMagicDefense() -> float :
	return core.MAGDEF
func getType() -> Definitions.equipmentTypeEnum :
	return Definitions.equipmentTypeEnum.armor
