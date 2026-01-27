extends "res://ScenePreloads/Equipment_old/equipment.gd"

var count : int = 0

func setCount(val) : 
	count = val
func getCount() :
	return count
func isEquipped() :
	return false
func equip() :
	pass
func unequip() :
	pass
func getType() -> Definitions.equipmentTypeEnum :
	return Definitions.equipmentTypeEnum.currency
