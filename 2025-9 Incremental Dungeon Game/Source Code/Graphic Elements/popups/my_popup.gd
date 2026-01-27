extends CanvasLayer

func hasScrollContainer() :
	for child in $Panel/CenterContainer/Window.get_children() :
		if child is ScrollContainer :
			return true
	return false

func setTitle(newTitle) :
	if (hasScrollContainer()) :
		$Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/Title.text = newTitle
	else :
		$Panel/CenterContainer/Window/VBoxContainer/Title.text = newTitle
	
func setText(newText) :
	if (hasScrollContainer()) :
		$Panel/CenterContainer/Window/ScrollContainer/PanelContainer/VBoxContainer/Text.text = newText
	else :
		$Panel/CenterContainer/Window/VBoxContainer/Text.text = newText
	
func nestedPopupInit(parentPopup) :
	var oldPos = $Panel.position
	$Panel.position = Vector2(9999,9999)
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
	$Panel.position = oldPos
	
func hasNestedPopup() -> bool :
	return getFirstNestedPopup() != null
	
##Assumes only 1 nested popup
func getFirstNestedPopup() :
	for child in get_children() :
		if (child.has_method("nestedPopupInit")) :
			return child
	return null
	
func getDeepestNestedPopup() :
	var retVal = self
	var tempVal = getFirstNestedPopup()
	while (tempVal != null) :
		retVal = tempVal
		tempVal = tempVal.getFirstNestedPopup()
	return retVal
	
func closeOutermostNestedPopup() :
	getDeepestNestedPopup().queue_free()
	
func addButtonContainer() :
	var newContainer = VBoxContainer.new()
	$Panel/CenterContainer/Window/VBoxContainer.add_child(newContainer)
	newContainer.name = "VBoxContainer"
	
const myButtonLoader = preload("res://Graphic Elements/Buttons/my_button.tscn")
func addButton(textVal) :
	var newButton = myButtonLoader.instantiate()
	$Panel/CenterContainer/Window/VBoxContainer/VBoxContainer.add_child(newButton)
	newButton.add_theme_font_size_override("font_size", 20)
	newButton.text = " " + textVal + " "
	newButton.connect("myPressed", _on_my_button_pressed)
	
signal buttonPressed
func _on_my_button_pressed(emitter) :
	var children = $Panel/CenterContainer/Window/VBoxContainer/VBoxContainer.get_children()
	for index in range(0, children.size()) :
		if (children[index] == emitter) :
			emit_signal("buttonPressed", index)
			return
		
func _ready() :
	$Panel.grab_focus()
