extends Panel

const optionsMenuLoader = preload("res://Graphic Elements/popups/actual_in_game_options.tscn")
const saveMenuLoader = preload("res://Graphic Elements/popups/in_game_options.tscn")
const binaryPopup = preload("res://Graphic Elements/popups/binary_decision.tscn")
	
##########################################
## Other
var firstProcess : bool = true
func _process(_delta) :
	$MyTabContainer/InnerContainer/Training.setPlayerMods($Player.getAttributeMods())
	updatePlayer()

func updatePlayer() :
	## GameScreen needs to know all of the data and where to get it that Player needs each frame, but not how it's created or how it's used.
	## This is the complete list of "direct" and "specific" modifiers detailed in Player.gd.
	## Player.gd does not actually need to know the complete list of direct modifiers but I will add it there as comments for clarity
	## Direct
	#Equipment
	var equipmentDirect : ModifierPacket = $MyTabContainer/InnerContainer/Equipment.getDirectModifiers()
	$Player.updateDirectModifier("Equipment", equipmentDirect)
	## Specific
	#Training
	var newTrainingLevels : Array[int] = $MyTabContainer/InnerContainer/Training.getAttributeLevels()
	$Player.updateTrainingLevels(newTrainingLevels)
	#Weapon
	var newWeapon : Weapon = $MyTabContainer/InnerContainer/Equipment.getCurrentWeapon()
	$Player.updateWeapon(newWeapon)
	var newArmor : Armor = $MyTabContainer/InnerContainer/Equipment.getCurrentArmor()
	$Player.updateArmor(newArmor)
	
	$Player.myUpdate()
#############################################
## New game initialisation
func initialisePlayerClass(val) :
	$Player.setClass(val)
func initialisePlayerName(val) :
	$Player.setName(val)
	
######################################
## Signals
func _on_tab_pressed(emitter : Button) :
	for child in $TabContainer.get_children() :
		if (child.name == emitter.name) :
			child.visible = true
		else :
			child.visible = false
			

func _on_options_button_pressed(_emitter) -> void:
	optionsMenuRef = optionsMenuLoader.instantiate()
	add_child(optionsMenuRef)
	
signal exitToMenu
func _on_save_button_pressed(_emitter) -> void:
	saveMenuRef = saveMenuLoader.instantiate()
	add_child(saveMenuRef)
	saveMenuRef.connect("exitToMenu", _exit_to_menu)
	saveMenuRef.connect("loadGameNow", _on_load_game_now)
	
func _exit_to_menu() :
	emit_signal("exitToMenu")
	
signal loadGameNow
func _on_load_game_now() :
	emit_signal("loadGameNow")
	
func myInventoryFull() -> bool :
	var inventory = $MyTabContainer/InnerContainer/Equipment
	return inventory.isInventoryFull()
	
func displayInventoryFull() :
	var inventoryFullPopup = popupLoader.instantiate()
	add_child(inventoryFullPopup)
	inventoryFullPopup.setTitle("Out of space!")
	inventoryFullPopup.setText("My inventory is full. I'll have to get rid of some things if I want to pick this up.")

enum inventoryBehaviour_local {wait,discard}
var IGOptions_inventoryBehaviour : inventoryBehaviour_local = inventoryBehaviour_local.wait
const popupLoader = preload("res://Graphic Elements/popups/my_popup_button.tscn")
func _on_combat_add_to_inventory_requested(itemSceneRef, isAutomatic : bool) -> void:
	if (itemSceneRef.core is Currency) :
		$TopRibbon/Ribbon/Currency.addToCurrency(itemSceneRef, itemSceneRef.getCount())
		$MyTabContainer/InnerContainer/Combat.removeCombatRewardEntry(itemSceneRef)
	elif (myInventoryFull()) :
		if (isAutomatic && IGOptions_inventoryBehaviour == inventoryBehaviour_local.discard) :
			$MyTabContainer/InnerContainer/Combat.removeCombatRewardEntry(itemSceneRef)
		## (!isAutomatic || isAutomatic && IGOptions_inventoryBehaviour == wait
		else :
			displayInventoryFull()
			$MyTabContainer/InnerContainer/Combat.denyAddToInventory()
	else :
		#Does not account for randomly generated bonuses if you ever add that sorry
		$MyTabContainer/InnerContainer/Equipment.addItemToInventory(SceneLoader.createEquipmentScene(itemSceneRef.getItemName()))
		$MyTabContainer/InnerContainer/Combat.removeCombatRewardEntry(itemSceneRef)
		
func _on_tutorial_requested(tutorialName : Encyclopedia.tutorialName, tutorialPos : Vector2) :
	if (tutorialName == Encyclopedia.tutorialName.tutorialFloor3 && !tabsUnlocked[1]) :
		$MyTabContainer.revealChild($MyTabContainer/InnerContainer/Training)
		tabsUnlocked[1] = true
	elif (tutorialName == Encyclopedia.tutorialName.equipment && !tabsUnlocked[2]) :
		$MyTabContainer.revealChild($MyTabContainer/InnerContainer/Equipment)
		tabsUnlocked[2] = true
	elif (tutorialName == Encyclopedia.tutorialName.tutorialFloorEnd && !tabsUnlocked[1]) :
		$MyTabContainer.revealChild($MyTabContainer/InnerContainer/Training)
		tabsUnlocked[1] = true
	$TutorialManager.queueTutorial(tutorialName, tutorialPos)
	
func _on_player_core_requested(emitter) -> void:
	emitter.providePlayerCore($Player.getCore())
func _on_player_class_requested(emitter) -> void:
	emitter.providePlayerClass($Player.getClass())
func _on_routine_unlock_requested(routine : AttributeTraining) :
	$MyTabContainer/InnerContainer/Training.unlockRoutine(routine)
func _on_floor_clear(typicalEnemyDefense) :
	$Player.setTypicalEnemyDefense(typicalEnemyDefense)
	
## details format
## "currencyTexture" : Texture2D
## "currencyReference" : Equipment
## "shopContents" : Dictionary ->
## "<column name 1>" : Dictionary ->
## "<purchasable 1>" : <cost>
## "<purchasable 2>" : <cost>
## ...
## "<column name 2>" : Dictionary ->
## ...
## ...
const shopLoader = preload("res://Graphic Elements/Shop/shop_window.tscn")
func _on_shop_requested(details : ShopDetails) :
	var newShop = shopLoader.instantiate()
	$MyTabContainer/InnerContainer/Combat.add_child(newShop)
	if (!newShop.myReady) :
		await newShop.myReadySignal
	newShop.connect("purchaseRequested", _on_shop_purchase_requested)
	newShop.connect("currencyAmountRequested", _on_currency_amount_requested)
	newShop.connect("finished", _on_shop_finished)
	$MyTabContainer/InnerContainer/Combat.disableUI()
	newShop.setFromDetails(details)

func _on_shop_finished() :
	$MyTabContainer/InnerContainer/Combat.enableUI()
	
func _on_currency_amount_requested(item : Currency, emitter : Node) :
	emitter.provideCurrencyAmount($TopRibbon/Ribbon/Currency.getCurrencyAmount(item))
	
func _on_shop_purchase_requested(item, itemPrice : int, currencyType : Currency, emitter) :
	var currencyAmount = $TopRibbon/Ribbon/Currency.getCurrencyAmount(currencyType)
	if (currencyAmount < itemPrice) :
		return
	##Special purchasables
	if (item is String) :
		if (item == "Inventory Space") :
			$MyTabContainer/InnerContainer/Equipment.expandInventory(1)
		else : 
			return
	##Equipment purchasables
	elif (item is Equipment) :
		if (myInventoryFull()) :
			displayInventoryFull()
			return
		$MyTabContainer/InnerContainer/Equipment.addItemToInventory(SceneLoader.createEquipmentScene(item.getItemName()))
	else : 
		return
	currencyAmount -= itemPrice
	$TopRibbon/Ribbon/Currency.setCurrencyAmount(currencyType, currencyAmount)
	emitter.setCurrencyAmount(currencyAmount)

func _on_magicFind_requested(emitter) :
	var magicFind = $Player.getOtherStat(Definitions.otherStatEnum.magicFind)
	emitter.provideMagicFind(magicFind)
		
#func _on_player_attribute_mods_requested(emitter) -> void:
	#emitter.providePlayerAttributeMods($Player.getAttributeMods())
	#
var saveMenuRef : Node = null
var optionsMenuRef : Node = null
var escWasPressed : bool = false
var menuWasPressed : bool = false
func _unhandled_input(event: InputEvent) -> void:
	if (event.is_action("ui_menu")) :
		accept_event()
		handleMenu(event)
	if (event.is_action("ui_cancel")) :
		accept_event()
		handleEsc(event)
		
			
func handleEsc(event) :
	if (event.is_pressed()) :
		escWasPressed = true
	else :
		if (escWasPressed) :
			if (saveMenuRef != null) :
				if (saveMenuRef.hasNestedPopup()) :
					saveMenuRef.closeOutermostNestedPopup()
				else :
					saveMenuRef.queue_free()
					saveMenuRef = null
			elif (optionsMenuRef != null) :
				if (optionsMenuRef.hasNestedPopup()) :
					optionsMenuRef.closeOutermostNestedPopup()
				else :
					optionsMenuRef.queue_free()
					optionsMenuRef = null
			else :
				_on_save_button_pressed(self)
		escWasPressed = false

func handleMenu(event) :
	if (event.is_pressed()) :
		menuWasPressed = true
	else :
		if (menuWasPressed) :
			if (optionsMenuRef != null) :
				optionsMenuRef.queue_free()
				optionsMenuRef = null
			else :
				_on_options_button_pressed(self)
		menuWasPressed = false
			
########################################
## Saving
	
var tabsUnlocked : Array
func getSaveDictionary() -> Dictionary :
	var tempDict = {}		
	tempDict["tabsUnlocked"] = tabsUnlocked
	return tempDict
		
var myReady : bool = false
func _ready() :
	myReady = true
	
func beforeLoad(newSave) :
	## Wait for annoying dependency
	if (!$MyTabContainer.myReady) :
		await $MyTabContainer.actuallyReady
	for tab in $MyTabContainer/InnerContainer.get_children() :
		if (tab.has_signal("tutorialRequested")) :
			tab.connect("tutorialRequested", _on_tutorial_requested)
		if (tab.has_signal("playerClassRequested")) :
			tab.connect("playerClassRequested", _on_player_class_requested)
		if (tab.has_signal("playerCoreRequested")) :
			tab.connect("playerCoreRequested", _on_player_core_requested)
		if (tab.has_signal("routineUnlockRequested")) :
			tab.connect("routineUnlockRequested", _on_routine_unlock_requested)
		if (tab.has_signal("shopRequested")) :
			tab.connect("shopRequested", _on_shop_requested)
		if (tab.has_signal("magicFindRequested")) :
			tab.connect("magicFindRequested", _on_magicFind_requested)
		#if (tab.has_signal("playerAttributeModsRequested")) :
			#tab.connect("playerAttributeModsRequested", _on_player_attribute_mods_requested)
	if (newSave) :
		$TutorialManager.queueTutorial(Encyclopedia.tutorialName.tutorial, Vector2(0,0))
		$TutorialManager.queueTutorial(Encyclopedia.tutorialName.tutorialFloor1, Vector2(0,0))
		for child in $MyTabContainer/InnerContainer.get_children() :
			if (child == $MyTabContainer/InnerContainer/Combat) :
				tabsUnlocked.append(true)
			else :
				tabsUnlocked.append(false)
				$MyTabContainer.hideChild(child)
		for index in range(0,tabsUnlocked.size()) :
			if (!tabsUnlocked[index]) :
				$MyTabContainer.hideChild($MyTabContainer/InnerContainer.get_child(index))
				
func onLoad(loadDict : Dictionary) -> void :
	tabsUnlocked = loadDict["tabsUnlocked"]
	for index in range(0,tabsUnlocked.size()) :
		if (!tabsUnlocked[index]) :
			$MyTabContainer.hideChild($MyTabContainer/InnerContainer.get_child(index))

func updateFromOptions(optionsCopy : Dictionary) :
	IGOptions_inventoryBehaviour = optionsCopy[IGOptions.options.inventoryBehaviour] as inventoryBehaviour_local
