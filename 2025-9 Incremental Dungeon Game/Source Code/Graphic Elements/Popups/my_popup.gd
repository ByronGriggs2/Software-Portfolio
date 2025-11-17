extends CanvasLayer

func setTitle(newTitle) :
	$Panel/CenterContainer/Window/VBoxContainer/Title.text = newTitle
	
func setText(newText) :
	$Panel/CenterContainer/Window/VBoxContainer/Text.text = newText
	
func nestedPopupInit(parentPopup) :
	$Panel/Background.color = Color(0,0,0,0)
	layer = parentPopup.layer + 1
	$Panel/CenterContainer/Window.queue_sort()
	await get_tree().process_frame
	await get_tree().process_frame
	var newSize = $Panel/CenterContainer/Window.size
	var parentSize = parentPopup.get_node("Panel").get_node("CenterContainer").get_node("Window").size
	if (parentSize.x > newSize.x) :
		newSize.x = parentSize.x
	if (parentSize.y > newSize.y) :
		newSize.y = parentSize.y
	$Panel/CenterContainer/Window.custom_minimum_size = newSize
