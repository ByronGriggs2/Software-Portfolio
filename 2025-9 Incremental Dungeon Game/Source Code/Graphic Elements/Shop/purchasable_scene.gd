extends "res://Graphic Elements/Buttons/super_button.gd"
var myName = ""
var myPrice : int = -1
var myEquipment : Equipment

var myReady : bool = false
signal myReadySignal
func _ready() :
	super()
	myReady = true
	emit_signal("myReadySignal")

func setCurrency(val : Texture2D) :
	$HBoxContainer/HBoxContainer2/CurrencySprite.setTexture(val)
func setPrice(val : int) :
	myPrice = val
	$HBoxContainer/HBoxContainer2/Price.text = " " + str(val) + " "
func setName(val : String) :
	myName = val
	$HBoxContainer/HBoxContainer/PurchaseableName.text = " " + val + " "
func setFromDetails(val : Purchasable) :
	setName(val.purchasableName)
	setPrice(val.purchasablePrice)
	myEquipment = val.equipment_optional
	if (myEquipment != null) :
		self.toggle = true

func getName() :
	return myName
func getPrice() :
	return myPrice
func getEquipment() :
	return myEquipment
