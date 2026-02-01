extends "res://Graphic Elements/popups/my_popup_button.gd"

func _ready() :
	const myOffset : int = 1
	var index : int = 0
	var copy = Encyclopedia.keywords.duplicate(true)
	copy.sort_custom(func(a,b):return a<b)
	for key in copy :
		var newEntry = getContainer().get_node("Sample").duplicate()
		getContainer().add_child(newEntry)
		getContainer().move_child(newEntry, index+myOffset)
		newEntry.visible = true
		newEntry.setText(key)
		newEntry.connect("myPressed", _on_my_pressed)
		index += 1
		
func getContainer() :
	return $Panel/CenterContainer/Window/VBoxContainer/PanelContainer/ScrollContainer/VBoxContainer
	
const popupLoader = preload("res://Graphic Elements/popups/my_popup_button_encyclopedia.tscn")
func _on_my_pressed(emitter : Node) :
	var myPopup = popupLoader.instantiate()
	add_child(myPopup)
	myPopup.nestedPopupInit(self)
	myPopup.setKey(emitter.getText())

func _on_line_edit_text_changed(new_text: String) -> void:
	if (new_text == "") :
		for child in getContainer().get_children() :
			if (child == getContainer().get_node("Sample")) :
				continue
			child.visible = true
		return
	for child in getContainer().get_children() :
		if (child == getContainer().get_node("Sample")) :
			continue
		elif (child.text.to_upper().find(new_text.to_upper()) == -1) :
			child.visible = false
		else :
			child.visible = true
