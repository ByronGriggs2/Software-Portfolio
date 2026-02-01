extends PanelContainer

func _ready() :
	#$VBoxContainer2/ClassDescriptor.custom_minimum_size = Vector2(570,195)
	var classes : Array[String]
	for key in Definitions.classDictionary :
		classes.append(Definitions.classDictionary[key])
	$VBoxContainer2/Carousels/ClassContainer/Class.setOptions(classes)
	$VBoxContainer2/Carousels/ClassContainer/Class.details = classDescriptions
	## set to mage (longest description)
	$VBoxContainer2/Carousels/ClassContainer/Class.currentPos = 0
	$VBoxContainer2/Carousels/ClassContainer/Class.refresh()
	$VBoxContainer2/ClassDescriptor.text = $VBoxContainer2/Carousels/ClassContainer/Class.details[$VBoxContainer2/Carousels/ClassContainer/Class.currentPos]
	$VBoxContainer2/Carousels/ClassContainer/Class.setMinWidth(size.x)
	#var classWarning = $VBoxContainer2/Carousels/ClassContainer/ClassWarning
	#var classWarningSize = await Helpers.getTextWidthWaitFrame(classWarning, null, null)
	#classWarning.offset_left = size.x/2 + classWarningSize/2 + 800
	##await get_tree().process_frame
	#await get_tree().process_frame
	#$VBoxContainer2/ClassDescriptor.custom_minimum_size = $VBoxContainer2/ClassDescriptor.size

signal classMoved
func _on_class_move(detail) -> void:
	$VBoxContainer2/ClassDescriptor.text = detail
	$VBoxContainer2/StatDescription.setStats($VBoxContainer2/Carousels/ClassContainer/Class.currentPos)
	emit_signal("classMoved", $VBoxContainer2/Carousels/ClassContainer/Class.currentPos)
	
var classDescriptions : Array[String] = [
	"As a warrior, you have a basic understanding of martial weapons. Your training thus far has focused on raw strength and endurance.",
	"As a mage, you have knowledge of basic magic. More importantly, your education in the scientific method and the laws of nature has prepared you to continue honing your magical skill.",
	"As a rogue, you take opponents apart with skill rather than raw power. Your years of experience have made you an artist with the blade, bow, and lockpick."
]
