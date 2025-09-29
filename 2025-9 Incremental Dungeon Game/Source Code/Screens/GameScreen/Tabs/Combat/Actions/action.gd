extends Resource

class_name Action
@export var text : String = "UndefAction"
enum targetingMode {
	RAND,
	LOW,
	ALL
}
@export var mode : targetingMode
@export var warmup : float
