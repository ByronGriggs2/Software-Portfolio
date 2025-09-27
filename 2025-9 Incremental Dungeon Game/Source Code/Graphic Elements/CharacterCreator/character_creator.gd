extends Panel

signal characterDone

func _ready() :
	var optionArr : Array[String]
	for key in Definitions.classDictionary :
		optionArr.append(Definitions.classDictionary[key])
	$Carousels/ClassContainer/Class.options = optionArr

func initialise(options, details, index) :
	$Carousels/Reason.options = options
	$Carousels/Reason.currentPos = index
	var classes : Array[String]
	for key in Definitions.classDictionary :
		classes.append(Definitions.classDictionary[key])
	$Carousels/ClassContainer/Class.options = classes
	$Carousels/ClassContainer/Class.details = classDescriptions
	if (index == 0 || index == 1) :
		$Carousels/ClassContainer/Class.currentPos = 0
	elif (index == 2 || index == 3) :
		$Carousels/ClassContainer/Class.currentPos = 1
	elif (index == 4 || index == 5) :
		$Carousels/ClassContainer/Class.currentPos = 2
	$Carousels/Reason.refresh()
	$Carousels/ClassContainer/Class.refresh()
	$Carousels/ClassDescriptor.text = $Carousels/ClassContainer/Class.details[$Carousels/ClassContainer/Class.currentPos]

func _on_class_move(detail) -> void:
	$Carousels/ClassDescriptor.text = detail
	$StatDescription.setStats($Carousels/ClassContainer/Class.currentPos)
	
var classDescriptions : Array[String] = [
	"As a warrior, you have a basic understanding of martial weapons. Your training thus far has focused on raw strength and endurance.",
	"As a mage, you have knowledge of basic magic. More importantly, your education in the scientific method and the laws of nature has prepared you to continue honing your magical skill.",
	"As a rogue, you take opponents apart with skill rather than raw power. Your years of experience have made you an artist with the blade, bow, and lockpick."
]

const popup = preload("res://Graphic Elements/Popups/binary_decision.tscn")
func _on_my_button_pressed() -> void:
	var tempPop = popup.instantiate()
	add_child(tempPop)
	tempPop.setTitle("Are you sure?")
	tempPop.setText("Is this your true self?")
	tempPop.connect("binaryChosen", _on_binary_chosen)
func _on_binary_chosen(chosen : int) :
	if (chosen == 0) :
		var character = $StatDescription.currentStats
		emit_signal("characterDone", character)
	elif (chosen == 1) :
		return
