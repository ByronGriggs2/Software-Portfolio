extends Panel

const optionsMenu = preload("res://Screens/GameScreen/in_game_options.tscn")
const binaryPopup = preload("res://Graphic Elements/Popups/binary_decision.tscn")

var tutorialsEnabled : bool = true
var tutorialPopupEnabled : bool = true

func _ready() :
	for child in $TabContainer.get_children() :
		var newButton : Button = Button.new()
		$TabButtonContainer/HBoxContainer.add_child(newButton)
		newButton.text = child.name
		newButton.name = child.name
		var c = Callable(self, "_on_tab_pressed").bind(newButton)
		newButton.pressed.connect(c)
	await get_tree().process_frame
	var maxSize = 180
	for child : Button in $TabButtonContainer/HBoxContainer.get_children() :
		if (child.size.x > maxSize) :
			maxSize = child.size.x
	for child : Button in $TabButtonContainer/HBoxContainer.get_children() :
		child.custom_minimum_size.x = maxSize
		$TabContainer/Combat.addPlayerReference($Player)
	#############################################################
	##Load game
	await get_tree().process_frame
	############################################################
	if (tutorialPopupEnabled) :
		var tutorialDialog = binaryPopup.instantiate()
		add_child(tutorialDialog)
		tutorialDialog.setTitle("Enable Tutorials?")
		tutorialDialog.setText("Would you like to enable tutorial popups? (Recommended)")
		tutorialDialog.connect("binaryChosen", _on_tutorial_dialog_choice)
	
func _on_tutorial_dialog_choice(choice : int) :
	if (choice == 0) :
		tutorialsEnabled = true
	elif (choice == 1) :
		tutorialsEnabled = false

func _on_tab_pressed(emitter : Button) :
	for child in $TabContainer.get_children() :
		if (child.name == emitter.name) :
			child.visible = true
		else :
			child.visible = false
	
func _process(_delta) :
	var attributeLevels : Array
	for key in Definitions.attributeDictionary :
		attributeLevels.append($TabContainer/Attributes.getAttributeLevel(key))
	$Player.updateAttributes(attributeLevels)
	$Player.updateDerivedStats()

func setPlayerClass(val) :
	$Player.setClass(val)
func setPlayerName(val) :
	$Player.setName(val)
	
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	tempDict["tutorialsEnabled"] = tutorialsEnabled
	return tempDict
	
func setFromSaveDictionary(loadDict) -> void :
	tutorialsEnabled = loadDict["tutorialsEnabled"]
	tutorialPopupEnabled = false

signal exitToMenu
func _on_options_button_pressed() -> void:
	var options = optionsMenu.instantiate()
	add_child(options)
	options.connect("exitToMenu", _exit_to_menu)
func _exit_to_menu() :
	emit_signal("exitToMenu")
