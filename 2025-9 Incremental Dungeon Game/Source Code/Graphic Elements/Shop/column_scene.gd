extends PanelContainer

var myCurrency : Texture2D = null
func setCurrency(val : Texture2D) :
	myCurrency = val
	for child in $VBoxContainer.get_children() :
		if (valid(child)) :
			child.setCurrency(val)

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
	
const entryLoader = preload("res://Graphic Elements/Shop/purchasable_scene.tscn")
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
		emit_signal("displayRequested", emitter.getEquipment(), emitter.getPrice(), self)
	else :
		emit_signal("purchaseRequested", emitter.getName(), emitter.getPrice())
	
func valid(val) :
	return (val != $VBoxContainer/CategoryName && val != $VBoxContainer/Spacer)

var myReady : bool = false
signal myReadySignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
	
func deselectAll() :
	for child in $VBoxContainer.get_children() :
		if (child.has_method("deselect")) :
			child.deselect()
