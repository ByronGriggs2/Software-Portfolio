extends VBoxContainer

const myLabel = preload("res://Screens/GameScreen/Player/my_text_label.tscn")

func _ready() :
	for key in Definitions.baseStatDictionary.keys() :
		var newText = myLabel.instantiate()
		add_child(newText)
	
func update(CombatStats : Array[NumberClass]) :
	for key in Definitions.baseStatDictionary.keys() :
		var child = get_child(key)
		child.setFirst(Definitions.baseStatDictionary[key] + ": ")
		child.setNumberReference(CombatStats[key])
