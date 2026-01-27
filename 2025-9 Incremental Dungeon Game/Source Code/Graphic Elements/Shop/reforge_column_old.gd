extends "res://Graphic Elements/Shop/column_scene.gd"
var reforgePrice = 0

func getReforgeHovered() :
	var mousePos = get_global_mouse_position()
	var receptorPos = $VBoxContainer/Receptor.global_position
	var receptorSize = $VBoxContainer/Receptor.size
	if (mousePos.x >= receptorPos.x && mousePos.x <= receptorPos.x + receptorSize.x && mousePos.y >= receptorPos.y && mousePos.y <= receptorPos.y + receptorSize.y) :
		return true
	return false
	
func setReforge(itemSceneRef : Node) :
	var type
	var enumVal
	if (itemSceneRef.getType() == Definitions.equipmentTypeEnum.armor) :
		type = "armor"
		enumVal = Shopping.armorPurchasable.reforge
	elif (itemSceneRef.getType() == Definitions.equipmentTypeEnum.weapon) :
		type = "weapon"
		enumVal = -1
	else :
		return
	var reforges = itemSceneRef.getReforges()
	## After 1 reforge it will cost as much as a brand new set of armor
	reforgePrice = Shopping.itemPrices[type][enumVal]*pow(1.5,reforges)
	$VBoxContainer/SuperButton/HBoxContainer/HBoxContainer2/Price.text = str(reforgePrice)
	

func _on_super_button_was_selected() -> void:
	if (!getReforgeHovered()) :
		return
	var purchase : Purchasable = Purchasable.new()
	purchase.purchasablePrice = reforgePrice
	emit_signal("purchaseRequested", "Reforge", "-1", purchase)
