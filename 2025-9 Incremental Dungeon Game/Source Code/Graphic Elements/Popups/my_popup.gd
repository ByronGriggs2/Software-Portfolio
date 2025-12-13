extends CanvasLayer

func hasScrollContainer() :
	for child in $Panel/CenterContainer/Window.get_children() :
		if child is ScrollContainer :
			return true
	return false

func setTitle(newTitle) :
	if (hasScrollContainer()) :
		$Panel/CenterContainer/Window/ScrollContainer/VBoxContainer/Title.text = newTitle
	else :
		$Panel/CenterContainer/Window/VBoxContainer/Title.text = newTitle
	
func setText(newText) :
	if (hasScrollContainer()) :
		$Panel/CenterContainer/Window/ScrollContainer/VBoxContainer/Text.text = newText
	else :
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
	
