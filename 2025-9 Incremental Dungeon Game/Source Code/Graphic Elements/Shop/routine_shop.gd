extends "res://Graphic Elements/Shop/shop_window.gd"

func setFromDetails(val) :
	super(val)
	if (!Shopping.getAllRoutinesPurchased()) :
		$VBoxContainer/Shop.get_child(1).get_child(0).get_child(3).visible = false
	

var routinesHandled : bool = false
func _process(_delta) :
	if (!routinesHandled) :
		if (Shopping.getAllRoutinesPurchased()) :
			var newDet : ShopDetails = shopDetails
			newDet.shopContents[1].purchasables[0].description = Shopping.routineDescriptionDictionary[str(Shopping.routinePurchasable.randomRoutine)+"alt"]
			newDet.shopContents[1].purchasables[0].purchasablePrice = 1
			for child in $VBoxContainer/Shop.get_children() :
				$VBoxContainer/Shop.remove_child(child)
				child.queue_free()
			await get_tree().process_frame
			await setFromDetails(newDet)
			
			$VBoxContainer/Shop.get_child(1).get_child(0).get_child(2).set_disabled(true)
			$VBoxContainer/Shop.get_child(1).get_child(0).get_child(3).visible = true
			routinesHandled = true
