extends Action

class_name AttackAction
enum damageType {
	PHYS,
	MAG
}
@export var type : damageType
@export var power : float
#add elements, status?

func getPower() :
	return power
	
func getDamageType() :
	return type
