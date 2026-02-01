extends VBoxContainer

const myLabel = preload("res://Screens/GameScreen/Player/my_text_label.tscn")

func _ready() :
	for key in Definitions.attributeDictionary.keys() :
		var newText = myLabel.instantiate()
		add_child(newText)
		
func update(attributes) :
	for key in Definitions.attributeDictionary.keys() :
		var child = get_child(key)
		child.setFirst(Definitions.attributeDictionary[key] + ": ")
		child.setSecond(str(attributes[key]))
	
