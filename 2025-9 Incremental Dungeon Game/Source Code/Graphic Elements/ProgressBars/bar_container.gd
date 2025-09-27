extends VBoxContainer

func _ready() :
	for child in get_children() :
		child.connect("setSelfAsActive", _on_bar_activated)

func _on_bar_activated(emitter) :
	for child in get_children() :
		if (child != emitter) :
			child.deactivate()
