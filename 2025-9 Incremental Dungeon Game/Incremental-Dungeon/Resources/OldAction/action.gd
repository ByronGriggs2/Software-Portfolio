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
var resourceName = null
func getResourceName() :
	if (resourceName == null) :
		resourceName = resource_path.get_file().get_basename()
	return resourceName
