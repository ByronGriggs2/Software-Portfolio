extends VBoxContainer

func getData() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["type"] = ""
	if ($PanelContainer/VBoxContainer/Filter/Option14/CheckBox.button_pressed) :
		retVal["type"] = "whitelist"
	else :
		retVal["type"] = "blacklist"
	retVal["element"] = []
	var startIndex = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option)
	var stopIndexPlus1 = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option5)+1
	var children = $PanelContainer/VBoxContainer/Filter.get_children()
	for index in range(startIndex, stopIndexPlus1) :
		retVal["element"].append(children[index].get_node("CheckBox").button_pressed)
	retVal["quality"] = []
	startIndex = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option6)
	stopIndexPlus1 = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option11)+1
	for index in range(startIndex, stopIndexPlus1) :
		retVal["quality"].append(children[index].get_node("CheckBox").button_pressed)
	retVal["equipmentType"] = []
	startIndex = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option16)
	stopIndexPlus1 = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option18)+1
	for index in range(startIndex, stopIndexPlus1) :
		retVal["equipmentType"].append(children[index].get_node("CheckBox").button_pressed)
	retVal["weaponType"] = []
	startIndex = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option19)
	stopIndexPlus1 = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option21)+1
	for index in range(startIndex, stopIndexPlus1) :
		retVal["weaponType"].append(children[index].get_node("CheckBox").button_pressed)
		
	retVal["directDowngrade"] = []
	for option in $PanelContainer2/DirectDowngrades/HBoxContainer.get_children() :
		retVal["directDowngrade"].append(option.get_node("CheckBox").button_pressed)
	for option in $PanelContainer3/DirectDowngrades/HBoxContainer.get_children() :
		retVal["directDowngrade"].append(option.get_node("CheckBox").button_pressed)
	return retVal

func initialise(loadDict : Dictionary) :
	if (!myReady) :
		await myReadySignal
	var whitelist = loadDict["type"] == "whitelist"
	$PanelContainer/VBoxContainer/Filter/Option14/CheckBox.set_pressed_no_signal(whitelist)
	$PanelContainer/VBoxContainer/Filter/Option15/CheckBox.set_pressed_no_signal(!whitelist)
	## elements
	var startIndex = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option)
	var stopIndexPlus1 = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option5)+1
	var children = $PanelContainer/VBoxContainer/Filter.get_children()
	for index in range(startIndex, stopIndexPlus1) :
		children[index].get_node("CheckBox").set_pressed_no_signal(loadDict["element"][index-startIndex])
	## quality
	startIndex = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option6)
	stopIndexPlus1 = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option11)+1
	for index in range(startIndex, stopIndexPlus1) :
		children[index].get_node("CheckBox").set_pressed_no_signal(loadDict["quality"][index-startIndex])
	## equipment type
	startIndex = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option16)
	stopIndexPlus1 = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option18)+1
	for index in range(startIndex, stopIndexPlus1) :
		children[index].get_node("CheckBox").set_pressed_no_signal(loadDict["equipmentType"][index-startIndex])
		
	startIndex = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option19)
	stopIndexPlus1 = Helpers.findIndexInContainer($PanelContainer/VBoxContainer/Filter, $PanelContainer/VBoxContainer/Filter/Option21)+1
	for index in range(startIndex, stopIndexPlus1) :
		children[index].get_node("CheckBox").set_pressed_no_signal(loadDict["weaponType"][index-startIndex])
		
	var options = $PanelContainer2/DirectDowngrades/HBoxContainer.get_children()
	for index in range(0,options.size()) :
		var option = options[index]
		option.get_node("CheckBox").set_pressed_no_signal(loadDict["directDowngrade"][index])
	var options2 = $PanelContainer3/DirectDowngrades/HBoxContainer.get_children()
	for index in range(0, options2.size()) :
		var option = options2[index]
		option.get_node("CheckBox").set_pressed_no_signal(loadDict["directDowngrade"][index+options.size()])

func setCurrentLayer(val : int) :
	setLayerRecursive(get_children(), val)
	
func setLayerRecursive(children : Array[Node], val : int) :
	for child in children :
		if (child.has_method("setCurrentLayer")) :
			child.setCurrentLayer(val)
		setLayerRecursive(child.get_children(), val)
		
var myReady : bool = false
signal myReadySignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
