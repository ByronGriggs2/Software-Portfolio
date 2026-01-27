extends Resource

class_name Action
@export var text : String = "UndefAction"
enum targetingMode {
	STANDARD,
	RAND,
	LOW,
	ALL
}
@export var mode : targetingMode
@export var warmup : float = 1

func getWarmup() :
	return warmup
func getName() :
	return text
