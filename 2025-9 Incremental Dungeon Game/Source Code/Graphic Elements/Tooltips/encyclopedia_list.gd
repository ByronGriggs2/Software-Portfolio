extends "res://Graphic Elements/popups/my_popup_button.gd"

func _ready() :
	const myOffset : int = 1
	var index : int = 0
	for key in Encyclopedia.keywords :
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
