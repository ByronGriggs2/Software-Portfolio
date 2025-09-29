extends VBoxContainer

const myLabel = preload("res://Screens/GameScreen/Player/my_text_label.tscn")

func _ready() :
	for key in Definitions.baseStatDictionary.keys() :
		var newText = myLabel.instantiate()
		add_child(newText)
	
func update(core : ActorPreset) :
	for key in Definitions.baseStatDictionary.keys() :
		var child = get_child(key)
		child.setFirst(Definitions.baseStatDictionary[key] + ": ")
		child.setSecond(str(core.getStat(key)))
