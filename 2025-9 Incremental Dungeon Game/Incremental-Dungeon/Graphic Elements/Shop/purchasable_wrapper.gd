extends VBoxContainer
const otherTheme = preload("res://Graphic Elements/Themes/subTab.tres")

func setDesc(val) :
	$PanelContainer/EncyclopediaTextLabel.setText(val)

var myReady : bool = false
signal myReadySignal
func _ready() :
	if (!$PurchasableScene.myReady) :
		await $PurchasableScene.myReadySignal
	$PanelContainer/EncyclopediaTextLabel.currentLayer += 1
	myReady = true
	emit_signal("myReadySignal")

signal wasSelected
signal wasDeselected
func _on_purchasable_scene_was_deselected(_emitter) -> void:
	emit_signal("wasDeselected", self)
func _on_purchasable_scene_was_selected(_emitter) -> void:
	emit_signal("wasSelected", self)
func getPurchasable() :
	return $PurchasableScene.getPurchasable()
	
func setCurrency(val : Texture2D) :
	$PurchasableScene.setCurrency(val)
func setPrice(val : int) :
	$PurchasableScene.setPrice(val)
#func setName(val : String) :
	#$PurchasableScene.setName(val)
func setFromDetails(val : Purchasable) :
	setDesc(val.description)
	if ($PanelContainer/EncyclopediaTextLabel.text == "") :
		$PanelContainer.visible = false
		$PurchasableScene.theme = otherTheme
		$PurchasableScene.updatePanel()
	$PurchasableScene.setFromDetails(val)
	if ($PanelContainer/EncyclopediaTextLabel.get_line_count() >= 13) :
		$PanelContainer/EncyclopediaTextLabel.add_theme_font_size_override("normal_font_size", 15)
func getName() :
	return $PurchasableScene.getName()
func getPrice() :
	return $PurchasableScene.getPrice()
func getEquipment() :
	return $PurchasableScene.getEquipment()
func deselect() :
	$PurchasableScene.deselect()
func select() :
	$PurchasableScene.select()
func getCore() :
	return $PurchasableScene.core
func isSelected() :
	return $PurchasableScene.selected
func set_disabled(val) :
	$PurchasableScene.set_disabled(val)
	if (val) :
		var disabledBackground : StyleBox = $PanelContainer.get_theme_stylebox("panel").duplicate()
		disabledBackground.bg_color = Color("bababa")
		#$PanelContainer/EncyclopediaTextLabel.add_theme_color_override("default color", Color("ffffff"))
		$PanelContainer.add_theme_stylebox_override("panel", disabledBackground)
		$PanelContainer/EncyclopediaTextLabel.useLightMode()
