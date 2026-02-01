extends Button

enum myVisibilityEnum {invisible, halfVisible, fullVisible}
var currentVisibility : myVisibilityEnum = myVisibilityEnum.invisible

var myName : String = ""
func setName(val) :
	myName = val
	text = " " + val + " "
func getName() :
	return myName
func isShop() :
	return true
	
func setVisibility(val : myVisibilityEnum) :
	return
	#currentVisibility = val
	#if (val == myVisibilityEnum.invisible) :
		#visible = false
	#elif (val == myVisibilityEnum.halfVisible) :
		#set_disabled(true)
		#visible = true
		#text = ""
	#elif (val == myVisibilityEnum.fullVisible) :
		#set_disabled(false)
		#visible = true
		#text = " Unexplored "
func getVisibility() :
	return currentVisibility
	
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary
	retVal["vis"] = currentVisibility
	return retVal
func onLoad(_loadDict) :
	pass
	#setVisibility(loadDict["vis"])
	
signal shopRequested
var shopType : String = ""
func setShopType(val) :
	shopType = val
func getShopType() :
	return shopType
var completed : bool = false
func _on_pressed() -> void:
	emit_signal("shopRequested", await Shopping.createShop(shopType), self)
func isCompleted() :
	return completed
func onCombatComplete() :
	completed = true

func _ready() :
	remove_from_group("Saveable")
	set_disabled(false)
	visible = true
	text = " " + myName + " "
