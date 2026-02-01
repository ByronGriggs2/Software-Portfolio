extends VBoxContainer

const labelledBar = preload("res://Graphic Elements/ProgressBars/labelled_progress_bar.tscn")

func _ready() :
	for key in Definitions.attributeDictionary.keys() :
		var temp = labelledBar.instantiate()
		$VBoxContainer.add_child(temp)
		temp.setLabel(Definitions.attributeDictionary[key])
		temp.connect("setSelfAsActive", _on_bar_activated)
	var widest = 0
	for child in $VBoxContainer.get_children() :
		if (child.getLabelWidth() > 0) :
			widest = child.getLabelWidth()
	for child in $VBoxContainer.get_children() :
		child.setLabelWidth(widest)

func _on_bar_activated(emitter) :
	for child in $VBoxContainer.get_children() :
		if (child != emitter) :
			child.deactivate()
