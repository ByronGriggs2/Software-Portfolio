extends Button

signal levelChosen
@export var hiddenName : String
@export var encounter : Encounter = null
enum myVisibilityEnum {invisible, halfVisible, fullVisible}
@export var visibilityOnStartup : myVisibilityEnum = myVisibilityEnum.invisible

func _ready() :
	if (visibilityOnStartup == myVisibilityEnum.invisible) :
		visible = false
		disabled = true
	elif (visibilityOnStartup == myVisibilityEnum.halfVisible) :
		visible = true
		disabled = true
	elif (visibilityOnStartup == myVisibilityEnum.fullVisible) :
		visible = true
		disabled = false

func _on_pressed() -> void:
	emit_signal("levelChosen", self)

func myEnable() :
	disabled = false
	text = hiddenName
	
func myDisable() :
	disabled = true
	text = ""

func getVisibility() :
	if (!visible) :
		return 0
	elif (disabled) :
		return 1
	elif (!disabled) :
		return 2
	else :
		return 3
