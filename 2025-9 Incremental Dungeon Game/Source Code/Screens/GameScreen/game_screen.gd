extends Panel

const optionsMenu = preload("res://Screens/GameScreen/in_game_options.tscn")
const binaryPopup = preload("res://Graphic Elements/popups/binary_decision.tscn")

var tutorialsEnabled : bool = true
var tutorialPopupEnabled : bool = true
				
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
		attributeLevels.append($MyTabContainer/InnerContainer/Attributes.getAttributeFinal(key))
	$Player.startFrame()
	$Player.updateAttributes(attributeLevels)
	$Player.updateEquipment($MyTabContainer/InnerContainer/Equipment.getAttributeBonus(), $MyTabContainer/InnerContainer/Equipment.getStatBonus())
	$Player.updateWeapon($MyTabContainer/InnerContainer/Equipment.getCurrentWeapon())
	$Player.updateArmor($MyTabContainer/InnerContainer/Equipment.getCurrentArmor())
	$Player.updateDerivedStats()

func setPlayerClass(val) :
	$Player.setClass(val)
func setPlayerName(val) :
	$Player.setName(val)

signal exitToMenu
func _on_options_button_pressed() -> void:
	var options = optionsMenu.instantiate()
	add_child(options)
	options.connect("exitToMenu", _exit_to_menu)
	options.connect("loadGameNow", _on_load_game_now)
func _exit_to_menu() :
	emit_signal("exitToMenu")
signal loadGameNow
func _on_load_game_now() :
	emit_signal("loadGameNow")

const popupLoader = preload("res://Graphic Elements/popups/my_popup_button.tscn")
func _on_combat_add_to_inventory_requested(itemSceneRef) -> void:
	if ($MyTabContainer/InnerContainer/Equipment.isInventoryFull(itemSceneRef.getType())) :
		var inventoryFullPopup = popupLoader.instantiate()
		add_child(inventoryFullPopup)
		inventoryFullPopup.setTitle("Out of space!")
		inventoryFullPopup.setText("My inventory is full. I'll have to get rid of some things if I want to pick this up.")
	else :
		#Does not account for randomly generated bonuses if you ever add that sorry
		$MyTabContainer/InnerContainer/Equipment.addItemToInventory(SceneLoader.createEquipmentScene(itemSceneRef.getItemName()))
		$MyTabContainer/InnerContainer/Combat.removeCombatRewardEntry(itemSceneRef)
		
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	tempDict["tutorialsEnabled"] = tutorialsEnabled
	return tempDict
	
func game_screen_fresh_save_init(myClass : CharacterClass, myName : String) :
	$Player.freshSaveInit(myClass, myName)
		
var myReady : bool = false
func _ready() :
	myReady = true
	
func beforeLoad(newSave) :
	$MyTabContainer/InnerContainer/Combat.addPlayerReference($Player)
	$MyTabContainer/InnerContainer/Attributes.addPlayerReference($Player)
	if (newSave) :
		var tutorialDialog = binaryPopup.instantiate()
		add_child(tutorialDialog)
		tutorialDialog.setTitle("Enable Tutorials?")
		tutorialDialog.setText("Would you like to enable tutorial popups? (Recommended)")
		tutorialDialog.connect("binaryChosen", _on_tutorial_dialog_choice)
	
func onLoad(loadDict) -> void :
	tutorialsEnabled = loadDict["tutorialsEnabled"]
	
func afterLoad() :
	pass
