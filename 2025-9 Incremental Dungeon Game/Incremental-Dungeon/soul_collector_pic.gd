extends Panel

const popupLoader = preload("res://Graphic Elements/popups/my_popup_button.tscn")



func _on_super_button_was_selected(_emitter) -> void:
	var newPopup = popupLoader.instantiate()
	add_child(newPopup)
	var topLayer = Helpers.getTopLayer()
	newPopup.layer = topLayer + 1
	newPopup.setTitle("")
	newPopup.setText("")
	newPopup.setButtonText("Continue")
	var container = newPopup.get_node("Panel").get_node("CenterContainer").get_node("Window").get_node("VBoxContainer")
	var bigTexture : TextureRect = $TextureRect.duplicate(true)
	container.add_child(bigTexture)
	container.move_child(bigTexture, container.get_child_count()-2)
	bigTexture.expand_mode = TextureRect.EXPAND_KEEP_SIZE
	bigTexture.texture_filter = container.texture_filter
	
