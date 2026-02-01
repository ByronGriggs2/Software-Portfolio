extends Panel
@export var shopDetails : ShopDetails = null
@export var shopName : String = ""
func getName() :
	return shopName

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
	newEntry.connect("hideDisplayIfEmitter", _on_hide_display)
	
#func setShopContents(val : Dictionary) :
	#for key in val.keys() :
		#await addShopColumn(val[key])
		
func _on_hide_display(emitter : Equipment) :
	## Does NOT work if there are duplicates in the store. Maybe.
	if ($detailsRight.visible && $detailsRight.getItemSceneRef() != null && $detailsRight.getItemSceneRef().core.getItemName() == emitter.getItemName()) :
		$detailsRight.setItemSceneRef(null)
		$detailsRight.visible = false
	elif ($detailsLeft.visible && $detailsLeft.getItemSceneRef() != null && $detailsLeft.getItemSceneRef().core.getItemName() == emitter.getItemName()) :
		$detailsLeft.setItemSceneRef(null)
		$detailsLeft.visible = false
	
signal purchaseRequested
var myItemSceneRef
var myPurchasableRef
func _on_purchase_requested(item, price, purchase) :
	emit_signal("purchaseRequested", item, price, myCurrency, self, purchase)
func _on_display_requested(purchase : Purchasable, column : Node) :
	if (myItemSceneRef != null) :
		myItemSceneRef.queue_free()
		myItemSceneRef = null
	myItemSceneRef = SceneLoader.createEquipmentScene(purchase.equipment_optional.getItemName())
	add_child(myItemSceneRef)
	myItemSceneRef.visible = false
	myItemSceneRef.core = purchase.equipment_optional
	myPurchasableRef = purchase
	var middleOfColumn = column.global_position.x+column.size.x/2.0
	var middleOfContainer = $VBoxContainer.global_position.x+$VBoxContainer.size.x/2.0
	if (middleOfColumn < middleOfContainer) :
		$detailsRight.visible = true
		$detailsRight.setItemSceneRefBase(myItemSceneRef)
	else :
		$detailsLeft.visible = true
		$detailsLeft.setItemSceneRefBase(myItemSceneRef)

func setFromDetails(det : ShopDetails) :
	shopDetails = det
	$VBoxContainer/RichTextLabel.text = (det.shopName).to_upper()
	shopName = det.shopName
	setCurrencyTexture(det.shopCurrencyTexture)
	setCurrencyAmount(await requestCurrencyAmount(det.shopCurrency))
	myCurrency = det.shopCurrency
	for column in det.shopContents :
		addShopColumn(column)
	Shopping.addNewShop(self)
	
func softNotification(purchasable) :
	for column in $VBoxContainer/Shop.get_children() :
		column.softNotification(purchasable)
	
func refreshPrice(itemName, value) :
	for column in $VBoxContainer/Shop.get_children() :
		column.refreshPrice(itemName, value)
		
func onEquipmentSold() :
	$VBoxContainer/Shop.get_child(0).onEquipmentSold()
	_on_details_option_pressed(myItemSceneRef, 1)
		
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
		emit_signal("purchaseRequested", myPurchasableRef.purchasableName, myPurchasableRef.purchasablePrice, myCurrency, self, myPurchasableRef)
	else :
		$detailsRight.visible = false
		$detailsRight.setItemSceneRefBase(null)
		$detailsLeft.visible = false
		$detailsLeft.setItemSceneRefBase(null)
		myItemSceneRef.queue_free()
		myItemSceneRef = null
		myPurchasableRef = null
		deselectAll()
		
func deselectAll() :
	for column in $VBoxContainer/Shop.get_children() :
		if (column.has_method("deselectAll")) :
			column.deselectAll()
