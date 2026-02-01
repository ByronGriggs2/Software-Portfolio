extends Control

const permittedBasePrefixes = ["human", "zdeep_dwarf", "zdeep_elf", "demigod", "dwarf", "elf", "gnome", "vampire"]

var permittedNames : Array[String] = []

var pristine : bool = true
func _ready() :
	setupCarousels(true)
	setupCarousels(false)
	setDefaultTextures()
	connectCarousels()
	setupLayout()
	applyPreset(fighterPreset)
	undoPreset = fighterPreset
	preUndoPreset = fighterPreset
	pristine = true
	
func setupCarousels(isBody : bool) :
	var children
	if (isBody) :
		children = $AppearanceContainer/Carousels/Body/PanelContainer/VBoxContainer.get_children()
	else :
		children = $AppearanceContainer/Carousels/Clothes/PanelContainer/VBoxContainer.get_children()
	for child in children :
		if (!(child is HBoxContainer)) :
			continue
		var type : String = child.name
		var count = 0
		if (type == "base") :
			count = setupBase(type)
		elif (type == "felids") :
			count = setupCat(type)
		else :
			count = setupOther(type)
		var tempOptions : Array[String] = []
		for index in range(0,count) :
			tempOptions.append(str(index))
		child.get_node("Carousel").setOptions(tempOptions)
	
func setupBase(type) -> int :
	var count = 0
	var values = MegaFile.getPlayerTextureDictionary("PlayerTexture_" + type).keys()
	for value in values :
		var permitted : bool = false
		for prefix in permittedBasePrefixes :
			if (value.find(prefix) != -1) :
				permitted = true
				break
		if (permitted) :
			permittedNames.append(value)
			count += 1
	return count
func setupCat(type) -> int :
	return MegaFile.getPlayerTextureDictionary("PlayerTexture_" + type).size()
func setupOther(type) -> int :
	return MegaFile.getPlayerTextureDictionary("PlayerTexture_" + type).size() + 1

func setDefaultTextures() :
	$Portrait.setTexture("Base", MegaFile.getPlayerTexture_base(permittedNames[0]), permittedNames[0])
	$Portrait.setTexture("Cat", MegaFile.getPlayerTexture_felids("cat_1"), "cat_1")
	for child in $Portrait.get_children() :
		if (child == $Portrait/Base || child == $Portrait/Cat) :
			pass
		else :
			child.texture = null

func connectCarousels() :
	for child in $AppearanceContainer/Carousels/Body/PanelContainer/VBoxContainer.get_children() :
		if !child.has_node("Carousel") : continue
		var carousel = child.get_node("Carousel")
		if (carousel != null) :
			carousel.connect("move", _on_carousel_move)
	for child in $AppearanceContainer/Carousels/Clothes/PanelContainer/VBoxContainer.get_children() :
		if !child.has_node("Carousel") : continue
		var carousel = child.get_node("Carousel")
		if (carousel != null) :
			carousel.connect("move", _on_carousel_move)
		
func _on_carousel_move(emitter : Node, currentPos, _currentOption) :
	pristine = false
	var type : String = emitter.get_parent().name
	if (type == "base") :
		$Portrait.setTexture("Base", MegaFile.getPlayerTexture_base(permittedNames[currentPos]), permittedNames[currentPos])
	elif (type == "felids") :
		$Portrait.setTexture("Cat", MegaFile.getPlayerTextureDictionary("PlayerTexture_" + type).values()[currentPos], MegaFile.getPlayerTextureDictionary("PlayerTexture_"+type).keys()[currentPos])
	elif (currentPos == 0) :
		$Portrait.setTexture(type, null, "null")
	else :
		$Portrait.setTexture(type, MegaFile.getPlayerTextureDictionary("PlayerTexture_" + type).values()[currentPos-1], MegaFile.getPlayerTextureDictionary("PlayerTexture_"+type).keys()[currentPos-1])
	if (!applyingPreset) :
		undoPreset = preUndoPreset
		preUndoPreset = getCurrentPreset()

func setupLayout() :
	var base = $AppearanceContainer/Carousels/Body/PanelContainer/VBoxContainer/base
	var children = $AppearanceContainer/Carousels/Body/PanelContainer/VBoxContainer.get_children()
	children.append_array($AppearanceContainer/Carousels/Clothes/PanelContainer/VBoxContainer.get_children())
	for child in children :
		if (!(child is HBoxContainer)) :
			continue
		if (child != base) :
			var newHome = base.get_node("TextureButton").duplicate()
			child.add_child(newHome)
			newHome.name = "TextureButton"
			var newButton = base.get_node("MyButton").duplicate()
			child.add_child(newButton)
			newButton.name = "MyButton"
			var newButton2 = base.get_node("MyButton2").duplicate()
			newButton2.name = "MyButton2"
			child.add_child(newButton2)
			child.move_child(newHome, 1)
			child.move_child(newButton,2)
			child.move_child(newButton2, 3)
		child.get_node("TextureButton").connect("myPressed",_on_home_pressed)
		child.get_node("MyButton").connect("myPressed", _on_button_pressed)
		child.get_node("MyButton2").connect("myPressed", _on_button2_pressed)

func _on_home_pressed(emitter : Node) :
	emitter.get_parent().get_node("Carousel").setPositionWithSignal(0, false)
func _on_button_pressed(emitter : Node) :
	emitter.get_parent().get_node("Carousel").setPositionWithSignal(-10, true)
func _on_button2_pressed(emitter : Node) :
	emitter.get_parent().get_node("Carousel").setPositionWithSignal(10,true)
	
signal characterDone
const popup = preload("res://Graphic Elements/popups/binary_decision.tscn")
func _on_my_button_pressed() -> void:
	var tempPop = popup.instantiate()
	add_child(tempPop)
	tempPop.setTitle("Are you sure?")
	tempPop.setText("Is this your true self?")
	tempPop.connect("binaryChosen", _on_binary_chosen)
	if (get_parent() is CanvasLayer) :
		tempPop.layer = get_parent().layer
		
const hyperLoader = preload("res://Graphic Elements/popups/hyperModePopup.tscn")
func _on_binary_chosen(chosen : int) :
	if (chosen == 0) :
		var myClass = $ClassContainer/CharacterCreator/VBoxContainer2/StatDescription.currentStats
		var myName = $VBoxContainer/LineEdit.text
		if (myName == "") :
			myName = " "
		var myCharacter : CharacterPacket = CharacterPacket.new()
		if (currentCharacterCache != null) :
			myCharacter.setClass(currentCharacterCache.getClass())
		else :
			myCharacter.setClass(myClass)
		myCharacter.setName(myName)
		myCharacter.setPortraitExtraSafe($Portrait)
		emit_signal("characterDone", myCharacter, false)
	elif (chosen == 1) :
		return

const softNotificationLoader = preload("res://Graphic Elements/soft_notification.tscn")
func _on_joke_cat_button_pressed() -> void:
	var newNotification = softNotificationLoader.instantiate()
	add_child(newNotification)
	newNotification.global_position = $JokeCatButton.global_position
	newNotification.initialiseAndRun("[color=red]DO NOT THE CAT[/color]", null, null, null, null)
	
func setLineEditText(val) :
	$VBoxContainer/LineEdit.text = val

const magePreset : Array[int] = [1,1,0,1,82,95,0,0,13,1,23]
const fighterPreset : Array[int] = [0,8,0,3,63,102,26,10,13,49,89]
const roguePreset : Array[int] = [5,7,0,2,0,88,20,0,11,16,187]
const nullPreset : Array[int] = [0,0,0,0,0,0,0,0,0,0,0]
var undoPreset : Array[int] = nullPreset
var preUndoPreset : Array[int] = nullPreset
var applyingPreset : bool = false
func _on_preset_button_pressed(emitter) -> void:
	applyingPreset = true
	var tempPreset
	tempPreset = getCurrentPreset()
	if (emitter.name as String == "Mage") :
		applyPreset(magePreset)
	elif (emitter.name as String == "Fighter") :
		applyPreset(fighterPreset)
	elif (emitter.name as String == "Rogue") :
		applyPreset(roguePreset)
	elif (emitter.name as String == "Reset All") :
		applyPreset(nullPreset)
	elif (emitter.name as String == "Undo") :
		applyPreset(undoPreset)
	else :
		return
	await get_tree().process_frame
	undoPreset = tempPreset
	preUndoPreset = getCurrentPreset()
	applyingPreset = false
	
func disableClassSelection() :
	$ClassContainer/CharacterCreator.visible = false
func addCancelOption() :
	$VBoxContainer.position.x = $Portrait.position.x/2.0-$VBoxContainer.size.x/2.0
	$VBoxContainer.position.y = $Portrait.position.y + $Portrait.size.y/2.0-$VBoxContainer.size.y/2.0
	#$VBoxContainer.position.x += 30
	$VBoxContainer.offset_bottom = 0
	var newButton = $VBoxContainer/MyButton.duplicate()
	$VBoxContainer.add_child(newButton)
	newButton.disconnect("pressed", _on_my_button_pressed)
	newButton.text = " Cancel "
	newButton.connect("pressed", _on_cancel)
	
var currentCharacterCache = null
var currentCharacterName
func initialiseAppearanceChange(currentCharacter : CharacterPacket) :
	$ClassContainer/Title.visible = false
	currentCharacterCache = currentCharacter
	disableClassSelection()
	addCancelOption()
	$Portrait.onLoad(currentCharacter.getPortraitDictionary())
	$VBoxContainer/LineEdit.text = currentCharacter.getName()
	var textures = currentCharacter.getPortraitDictionary()["textures"]
	for index in range(0,textures.size()) :
		var key = textures.keys()[index]
		if (textures[key] == null || textures[key] == "null") :
			continue
		var tempKey
		if (key == "Base") :
			tempKey = "base"
		elif (key == "Cat") :
			tempKey = "felids"
		else :
			tempKey = key
		var isBody : bool = false
		for child in $AppearanceContainer/Carousels/Body/PanelContainer/VBoxContainer.get_children() :
			if (child.name as String == tempKey) :
				isBody = true
				break
		var pos
		if (key == "Base") :
			pos = permittedNames.find(textures[key])
		else :
			pos = MegaFile.getPlayerTextureDictionary("PlayerTexture_"+tempKey).keys().find(textures[key])
		if (isBody) :
			if (key != "Cat" && key != "Base") :
				pos += 1
			$AppearanceContainer/Carousels/Body/PanelContainer/VBoxContainer.get_node(tempKey).get_node("Carousel").setPositionNoSignal(pos)
		else :
			pos += 1
			$AppearanceContainer/Carousels/Clothes/PanelContainer/VBoxContainer.get_node(tempKey).get_node("Carousel").setPositionNoSignal(pos)
		
func _on_cancel() :
	emit_signal("characterDone", currentCharacterCache, false)
	
func applyPreset(val : Array[int]) :
	var wasPristine = pristine
	var bodyCarousels = $AppearanceContainer/Carousels/Body/PanelContainer/VBoxContainer.get_children()
	var clothesCarousels = $AppearanceContainer/Carousels/Clothes/PanelContainer/VBoxContainer.get_children()
	#var bodyNames : Array[String] = []
	#for car in bodyCarousels :
		#bodyNames.append(car.name as String)
	#var clothesNames : Array[String] = []
	#for car in clothesCarousels :
		#clothesNames.append(car.name as String)
	for index in range(0, val.size()) :
		if (index < bodyCarousels.size()) :
			bodyCarousels[index].get_node("Carousel").setPositionWithSignal(val[index], false)
		else :
			clothesCarousels[index-bodyCarousels.size()].get_node("Carousel").setPositionWithSignal(val[index], false)
	pristine = wasPristine
			
func getCurrentPreset() -> Array[int] :
	var retVal : Array[int] = []
	for car in $AppearanceContainer/Carousels/Body/PanelContainer/VBoxContainer.get_children() :
		retVal.append(car.get_node("Carousel").getPosition())
	for car in $AppearanceContainer/Carousels/Clothes/PanelContainer/VBoxContainer.get_children() :
		retVal.append(car.get_node("Carousel").getPosition())
	return retVal


func _on_character_creator_class_moved(newPos) -> void:
	if (pristine) :
		var tempPreset = getCurrentPreset()
		if (newPos == 0) :
			applyPreset(fighterPreset)
		elif (newPos == 1) :
			applyPreset(magePreset)
		elif (newPos == 2) :
			applyPreset(roguePreset)
		undoPreset = tempPreset
