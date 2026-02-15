extends "res://ScenePreloads/Equipment_old/equipment.gd"

var count : float = 0

func setCount(val) : 
	if (val < 9*pow(10,18)) :
		count = int(val)
	else :
		count = val
func getCount() :
	if (count < 9*pow(10,18)) :
		return int(count)
	else :
		return count
func isEquipped() :
	return false
func equip() :
	pass
func unequip() :
	pass
func getType() -> Definitions.equipmentTypeEnum :
	return Definitions.equipmentTypeEnum.currency
