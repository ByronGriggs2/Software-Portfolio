extends Control

var currencyList : Array[Node] = []

func getCurrencyAmount(type) -> int :
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
		currencyRef.setCount(currencyRef.getCount() + val)
	updateCurrencyGraphic(equip)
###################################################
## Internal
func updateCurrencyGraphic(equip : Currency) :
	var index = findCurrencyIndex(equip)
	if (index == null) :
		return
	$HBoxContainer.get_child(index).get_node("Number").text = " " + str($HBoxContainer.get_child(index).get_child(0).getCount()) + " "
func alphabetise() :
	currencyList.sort_custom(func(a,b):a.name<b.name)
	for index in range(0,currencyList.size()) :
		var entry : Node = null
		for child in $HBoxContainer.get_children() :
			if (child.get_child(0) == currencyList[index]) :
				entry = child
		$HBoxContainer.move_child(entry,index)
func addNewCurrency(type : Currency) :
	var newEntry = $Sample.duplicate()
	$HBoxContainer.add_child(newEntry)
	newEntry.visible = true
	currencyList.append(SceneLoader.createEquipmentScene(type.getItemName()))
	newEntry.add_child(currencyList.back())
	var newCurrency = currencyList.back()
	newEntry.move_child(newCurrency,0)
	newEntry.name = newCurrency.getItemName()
	alphabetise()
	newCurrency.mouse_filter = Control.MOUSE_FILTER_IGNORE
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
		if (currencyList[index].core == type) :
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
func _ready() :
	myReady = true
func beforeLoad(_newGame) :
	pass
func onLoad(loadDict) :
	if (loadDict.get("currencyList") != null) :
		for key in loadDict["currencyList"].keys() :
			var item = loadDict["currencyList"][key]
			setCurrencyAmount(SceneLoader.equipmentResourceDictionary[item["name"]],item["count"])
