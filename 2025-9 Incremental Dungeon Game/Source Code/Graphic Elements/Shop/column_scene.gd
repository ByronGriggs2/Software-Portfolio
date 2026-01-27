extends PanelContainer

var myCurrency : Texture2D = null
func setCurrency(val : Texture2D) :
	myCurrency = val
	for child in $VBoxContainer.get_children() :
		if (valid(child)) :
			child.setCurrency(val)
			
func refreshPrice(itemName, value) :
	for child in $VBoxContainer.get_children() :
		if (valid(child) && child.getName() == itemName) :
			child.setPrice(value)
			
func softNotification(purchasable) :
	for child in $VBoxContainer.get_children() :
		if (valid(child) && child.getPurchasable() == purchasable) :
			createSoftNotification(child)
			
const notificationLoader = preload("res://Graphic Elements/soft_notification.tscn")
func createSoftNotification(child : Node) :
	var purchase : Purchasable = child.getPurchasable()
	var iname = purchase.purchasableName
	var text
	if (iname == Shopping.routinePurchasableDictionary[Shopping.routinePurchasable.randomRoutine]) :
		var newRoutine : AttributeTraining = Shopping.getLastUnlockedRoutine()
		text = newRoutine.getText()
		text = text.substr(0,1).to_lower() + text.substr(1)
		text = "Unlocked " + text
	elif (iname == Shopping.routinePurchasableDictionary[Shopping.routinePurchasable.upgradeRoutine]) :
		var upgraded : AttributeTraining = Shopping.getLastUpgradedRoutine()
		text = upgraded.getText()
		text = text.substr(0,1).to_lower() + text.substr(1)
		text = "Upgraded " + text
	elif (iname == Shopping.armorPurchasableDictionary[Shopping.armorPurchasable.newArmor] || iname == Shopping.weaponPurchasableDictionary[Shopping.weaponPurchasable.newWeapon]) :
		var newItem : Equipment = Shopping.getLastCreatedItem()
		text = EquipmentGroups.colourText(newItem.equipmentGroups.quality, "Forged " + newItem.getName(), true)
	elif (iname == Shopping.armorPurchasableDictionary[Shopping.armorPurchasable.reforge] || iname == Shopping.weaponPurchasableDictionary[Shopping.weaponPurchasable.reforge]) :
		var reforged : Equipment = Shopping.getLastReforgedItem()
		text = EquipmentGroups.colourText(reforged.equipmentGroups.quality, "Reforged " + reforged.getName(), true)
	elif (iname == Shopping.soulPurchasableDictionary[Shopping.soulPurchasable.randomStat]) :
		var upgraded : Array[String] = Shopping.getLastUpgradedString()
		if (upgraded.size() == 1) :
			text = upgraded[0]
		else :
			spamNotifications(child, upgraded)
			return
	else : 
		return
	var newNotif = notificationLoader.instantiate()
	get_parent().get_parent().get_parent().add_child(newNotif)
	getSoftPos(text, newNotif)

func getSoftPos(text, newNotif) :
	var estimatedSize = Vector2(0,20)
	var mousePos = get_global_mouse_position()
	estimatedSize.x = await Helpers.getTextWidthWaitFrame(null, 35, text) + 50
	var screenSize : Vector2i = Engine.get_singleton("DisplayServer").screen_get_size()
	var X0 = clamp(mousePos.x - 150, 0, screenSize.x-estimatedSize.x)
	var X1 = clamp(mousePos.x + 150,0,screenSize.x-estimatedSize.x)
	var Y0 = clamp(mousePos.y - 150,100,screenSize.y-estimatedSize.y)
	var Y1 = mousePos.y-estimatedSize.y
	newNotif.initialiseAndRun(text,X0,X1,Y0,Y1)
	
func spamNotifications(_child, upgraded : Array[String]) :
	var myTimer = Timer.new()
	add_child(myTimer)
	myTimer.one_shot = false
	myTimer.wait_time = 0.1
	myTimer.start()
	for item in upgraded :
		await myTimer.timeout
		var newNotif = notificationLoader.instantiate()
		get_parent().get_parent().get_parent().add_child(newNotif)
		getSoftPos(item, newNotif)
	remove_child(myTimer)
	myTimer.queue_free()

func setCategoryName(val : String) :
	$VBoxContainer/CategoryName.text = " " + val + " "
	
func addPurchasable(val : Purchasable) :
	var newEntry = entryLoader.instantiate()
	$VBoxContainer.add_child(newEntry)
	if (!newEntry.myReady) :
		await newEntry.myReadySignal
	if (myCurrency != null) :
		newEntry.setCurrency(myCurrency)
	newEntry.setFromDetails(val)
	newEntry.connect("wasSelected", _on_purchasable_selected)
	newEntry.connect("wasDeselected", _on_purchasable_deselected)
	
const entryLoader = preload("res://Graphic Elements/Shop/purchasable_wrapper.tscn")
func setFromDetails(val : ShopColumn) :
	setCategoryName(val.columnName)
	for purchasable in val.purchasables :
		addPurchasable(purchasable)
		
#func setPurchasables(val : Array[Purchasable]) :
	#for key in val.keys() :
		#var newEntry = entryLoader.instantiate()
		#$VBoxContainer.add_child(newEntry)
		#if (!newEntry.myReady) :
			#await newEntry.myReadySignal
		#if (myCurrency != null) :
			#newEntry.setCurrency(myCurrency)
		#newEntry.setName(key)
		#newEntry.setPrice(val[key])
		#newEntry.connect("wasSelected", _on_purchasable_selected)
	
signal purchaseRequested
signal displayRequested
func _on_purchasable_selected(emitter) :
	if (emitter.getEquipment() != null) :
		emit_signal("displayRequested", emitter.getPurchasable(), self)
		for child in $VBoxContainer.get_children() :
			if (child != emitter && child.has_method("deselect")) :
				child.deselect()
	else :
		emit_signal("purchaseRequested", emitter.getName(), emitter.getPrice(), emitter.getCore())
		
signal hideDisplayIfEmitter
func _on_purchasable_deselected(emitter) :
	if (emitter.getEquipment() != null) :
		emit_signal("hideDisplayIfEmitter", emitter.getEquipment())
	
func valid(val) :
	return (val != $VBoxContainer/CategoryName && val != $VBoxContainer/Spacer)
	
func onEquipmentSold() :
	var purchased : Node = null
	for child in $VBoxContainer.get_children() :
		if (child.has_method("isSelected") && child.isSelected()) :
			purchased = child
			break
	if (purchased == null) :
		return
	$VBoxContainer.remove_child(purchased)
	purchased.queue_free()
	purchased = null

var myReady : bool = false
signal myReadySignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
	
func deselectAll() :
	for child in $VBoxContainer.get_children() :
		if (child.has_method("deselect")) :
			child.deselect()
