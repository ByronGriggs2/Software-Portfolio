extends Control

var currencyList : Array[Node] = []

func getCurrencyAmount(type) -> float :
	var equip = getEquip(type)
	if (equip == null) :
		return -1
	var currencyRef = findCurrency(equip)
	if (currencyRef == null) :
		return 0
	else :
		return currencyRef.getCount()
func setCurrencyAmount(type, val) :
	var equip = getEquip(type)
	if (equip == null) :
		return
	var currencyRef = findCurrency(equip)
	if (currencyRef == null) :
		addNewCurrency(equip)
	findCurrency(equip).setCount(val)
	updateCurrencyGraphic(equip)
## Allows negative adds and even negative results
func addToCurrency(type, val) :
	var equip = getEquip(type)
	if (equip == null) :
		return
	var currencyRef = findCurrency(equip)
	if (currencyRef == null) :
		addNewCurrency(equip)
		findCurrency(equip).setCount(val)
	else :
		var currentCount = currencyRef.getCount()
		if (currentCount is float) :
			currencyRef.setCount(currentCount + float(val))
		elif (is_equal_approx(currentCount, 0)) :
			currencyRef.setCount(val)
		else :
			var currentRatio = currentCount/(9.0*pow(10,18))
			var newRatio = currentRatio * (1+val/float(currentCount))
			if (newRatio >= 0.95) :
				currencyRef.setCount(float(currentCount)+float(val))
			else :
				currencyRef.setCount(currentCount + val)
		#currencyRef.setCount(currencyRef.getCount() + val)
	updateCurrencyGraphic(equip)
###################################################
## Internal
func updateCurrencyGraphic(equip : Currency) :
	var index = findCurrencyIndex(equip)
	if (index == null) :
		return
	$VBoxContainer/HBoxContainer.get_child(index).get_node("Number").text = " " + Helpers.engineeringRound($VBoxContainer/HBoxContainer.get_child(index).get_child(0).getCount(),3) + " "
func alphabetise() :
	currencyList.sort_custom(func(a,b):a.name<b.name)
	for index in range(0,currencyList.size()) :
		var entry : Node = null
		for child in $VBoxContainer/HBoxContainer.get_children() :
			if (child.get_child(0) == currencyList[index]) :
				entry = child
		$VBoxContainer/HBoxContainer.move_child(entry,index)
func addNewCurrency(type : Currency) :
	var newEntry = $Sample.duplicate()
	$VBoxContainer/HBoxContainer.add_child(newEntry)
	newEntry.visible = true
	currencyList.append(SceneLoader.createEquipmentScene(type.getItemName()))
	newEntry.add_child(currencyList.back())
	var newCurrency = currencyList.back()
	newEntry.move_child(newCurrency,0)
	newEntry.name = newCurrency.getItemName()
	alphabetise()
	newCurrency.mouse_filter = Control.MOUSE_FILTER_IGNORE
	newCurrency.get_node("SuperButton").mouse_filter = Control.MOUSE_FILTER_IGNORE
	newCurrency.use48x48()
	updateCurrencyGraphic(type)
	
func getEquip(item) :
	if (item is Currency) :
		return item
	elif (item is Node && item.has_method("getType") && item.core is Currency) :
		return item.core
	else :
		return null
func findCurrency(type : Currency) :
	for item in currencyList :
		if (item.core == type) :
			return item
	return null
func findCurrencyIndex(type : Currency) :
	for index in range(0,currencyList.size()) :
		if (currencyList[index].core.getItemName() == type.getItemName()) :
			return index
	return null

############################################################
func getSaveDictionary() -> Dictionary :
	var tempDict : Dictionary = {}
	tempDict["currencyList"] = {}
	for index in range(0, currencyList.size()) :
		tempDict["currencyList"][index] = {
			"name" : currencyList[index].getItemName(),
			"count" : currencyList[index].getCount()
	}
	return tempDict
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
func beforeLoad(newGame) :
	myReady = false
	if (Definitions.DEVMODE) :
		$VBoxContainer/DevOptions.visible = true
	myReady = true
	emit_signal("myReadySignal")
	if (newGame) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
func onLoad(loadDict) :
	myReady = false
	if (loadDict.get("currencyList") != null) :
		for key in loadDict["currencyList"].keys() :
			var item = loadDict["currencyList"][key]
			setCurrencyAmount(EquipmentDatabase.getEquipment(item["name"]),item["count"])
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")


func _on_line_edit_text_submitted(new_text: String) -> void:
	if (!Definitions.DEVMODE) :
		return
	setCurrencyAmount(EquipmentDatabase.getEquipment("gold_coin"), int(new_text))

func _on_line_edit_2_text_submitted(new_text: String) -> void:
	if (!Definitions.DEVMODE) :
		return
	setCurrencyAmount(EquipmentDatabase.getEquipment("ore"), int(new_text))

func _on_line_edit_3_text_submitted(new_text: String) -> void:
	if (!Definitions.DEVMODE) :
		return
	setCurrencyAmount(EquipmentDatabase.getEquipment("soul"), int(new_text))
