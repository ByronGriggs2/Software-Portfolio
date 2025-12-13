extends Panel
@export var shopDetails : ShopDetails = null
@export var shopName : String = ""

var myTexture : Texture2D
var myCurrency : Equipment

#var myCurrencyTexture : Texture2D = null
func setCurrencyTexture(val : Texture2D) :
	myTexture = val
	$VBoxContainer/CurrencyDisplay/HBoxContainer/Icon/Icon.texture = val
	for child in $VBoxContainer/Shop.get_children() :
		child.setCurrency(val)
		
var currencyAmount : int = -1
func setCurrencyAmount(val) :
	currencyAmount = val
	$VBoxContainer/CurrencyDisplay/HBoxContainer/Count.text = " " + str(val) + " "

const columnLoader = preload("res://Graphic Elements/Shop/column_scene.tscn")
func addShopColumn(val : ShopColumn) :
	var newEntry = columnLoader.instantiate()
	$VBoxContainer/Shop.add_child(newEntry)
	if (!newEntry.myReady) :
		await newEntry.myReadySignal
	newEntry.setCurrency(myTexture)
	newEntry.setFromDetails(val)
	newEntry.connect("purchaseRequested", _on_purchase_requested)
	newEntry.connect("displayRequested", _on_display_requested)
	
#func setShopContents(val : Dictionary) :
	#for key in val.keys() :
		#await addShopColumn(val[key])
	
signal purchaseRequested
var myItemSceneRef
var myItemPrice
func _on_purchase_requested(item, price) :
	emit_signal("purchaseRequested", item, price, myCurrency, self)
func _on_display_requested(item : Equipment, price : int, column : Node) :
	if (myItemSceneRef != null) :
		myItemSceneRef.queue_free()
	myItemPrice = price
	myItemSceneRef = SceneLoader.createEquipmentScene(item.getItemName())
	add_child(myItemSceneRef)
	myItemSceneRef.visible = false
	var middleOfColumn = column.global_position.x+column.size.x/2.0
	var middleOfContainer = $VBoxContainer.global_position.x+$VBoxContainer.size.x/2.0
	if (middleOfColumn < middleOfContainer) :
		$detailsRight.visible = true
		$detailsRight.setItemSceneRefBase(myItemSceneRef)
	else :
		$detailsLeft.visible = true
		$detailsLeft.setItemSceneRefBase(myItemSceneRef)


func setFromDetails(det : ShopDetails) :
	$RichTextLabel.text = (det.shopName).to_upper()
	setCurrencyTexture(det.shopCurrencyTexture)
	setCurrencyAmount(await requestCurrencyAmount(det.shopCurrency))
	myCurrency = det.shopCurrency
	for column in det.shopContents :
		addShopColumn(column)
		
var myReady : bool = false
signal myReadySignal
func _ready() :
	if (shopDetails != null) :
		await setFromDetails(shopDetails)
	myReady = true
	emit_signal("myReadySignal")
	
signal currencyAmountRequested
signal currencyAmountProvided
var currencyAmount_comm
var waitingForCurrency : bool = false
func requestCurrencyAmount(ref : Equipment) :
	waitingForCurrency = true
	emit_signal("currencyAmountRequested", ref, self)
	if (waitingForCurrency) :
		await currencyAmountProvided
	return currencyAmount_comm
func provideCurrencyAmount(val) :
	currencyAmount_comm = val
	waitingForCurrency = false
	emit_signal("currencyAmountProvided")

signal finished
func _on_exit_pressed() -> void:
	emit_signal("finished")
	queue_free()

func _on_details_option_pressed(_itemSceneRef : Node, choice : int) -> void:
	if (choice == 0) :
		emit_signal("purchaseRequested", myItemSceneRef.core, myItemPrice, myCurrency, self)
	else :
		$detailsRight.visible = false
		$detailsRight.setItemSceneRefBase(null)
		$detailsLeft.visible = false
		$detailsLeft.setItemSceneRefBase(null)
		myItemSceneRef.queue_free()
		myItemSceneRef = null
		myItemPrice = -1
		deselectAll()
		
func deselectAll() :
	for column in $VBoxContainer/Shop.get_children() :
		if (column.has_method("deselectAll")) :
			column.deselectAll()
