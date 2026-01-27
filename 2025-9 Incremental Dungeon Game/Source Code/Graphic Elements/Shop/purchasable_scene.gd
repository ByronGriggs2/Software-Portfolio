extends "res://Graphic Elements/Buttons/super_button.gd"
var core : Purchasable = null

var myReady : bool = false
signal myReadySignal
func _ready() :
	super()
	myReady = true
	emit_signal("myReadySignal")

func setCurrency(val : Texture2D) :
	$HBoxContainer/HBoxContainer2/CurrencySprite.setTexture(val)
func setPrice(val) :
	core.purchasablePrice = val
	if (val == -1) :
		$HBoxContainer/HBoxContainer2/Price.text = " -1 "
	else :
		$HBoxContainer/HBoxContainer2/Price.text = " " + str(Helpers.engineeringRound(val, 3)) + " "
	
var disabled : bool = false
func set_disabled(val : bool) :
	disabled = val
	if (val) :
		var disabledFontColor = Color("000000")
		setTextColor(disabledFontColor)
		var disabledBackground = get_theme_stylebox("panel").duplicate()
		disabledBackground.bg_color = Color("bababa")
		add_theme_stylebox_override("panel", disabledBackground)
	else :
		removeTextColor()
		if (hovered) :
			setPanelTheme("hover")
		else :
			setPanelTheme("normal")

func setTextColor(newColor) :
	$HBoxContainer/HBoxContainer/PurchaseableName.add_theme_color_override("default_color", newColor)
	$HBoxContainer/HBoxContainer2/Price.add_theme_color_override("default_color", newColor)

func removeTextColor() :
	$HBoxContainer/HBoxContainer/PurchaseableName.remove_theme_color_override("default_color")
	$HBoxContainer/HBoxContainer2/Price.remove_theme_color_override("default_color")

func _on_gui_input(event: InputEvent) -> void:
	if (!disabled) :
		super(event)
		
func _on_mouse_entered() :
	if (!disabled) :
		super()
		
func _on_mouse_exited() :
	if (!disabled) :
		super()
		
func setTheme(newTheme) :
	if (!disabled) :
		super(newTheme)
	else :
		theme = newTheme
		set_disabled(true)
		
func setFromDetails(val : Purchasable) :
	core = val
	if ((val.equipment_optional) != null) :
		$HBoxContainer/HBoxContainer/PurchaseableName.text = " " + val.equipment_optional.getName() + " "
	else :
		$HBoxContainer/HBoxContainer/PurchaseableName.text = " " + val.purchasableName + " "
	if (val.purchasablePrice == -1) :
		$HBoxContainer/HBoxContainer2/Price.text = " -1 "
	else :
		$HBoxContainer/HBoxContainer2/Price.text = " " + Helpers.engineeringRound(val.purchasablePrice, 3) + " "
	if (val.equipment_optional != null) :
		self.toggle = true

func getName() :
	return core.purchasableName
func getPrice() :
	return core.purchasablePrice
func getEquipment() :
	return core.equipment_optional
func getPurchasable() :
	return core
