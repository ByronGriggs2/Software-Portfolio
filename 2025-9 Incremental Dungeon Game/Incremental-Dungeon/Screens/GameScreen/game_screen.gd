extends Panel

const optionsMenuLoader = preload("res://Graphic Elements/popups/actual_in_game_options.tscn")
const saveMenuLoader = preload("res://Graphic Elements/popups/in_game_options.tscn")
const binaryPopup = preload("res://Graphic Elements/popups/binary_decision.tscn")
	
##########################################
## Other
var last = Vector2.ZERO
var firstProcess : bool = true
func _process(_delta) :
	
	if (!doneLoading) :
		return
	if (!myReady) :
		return
	if (firstProcess) :
		var numberRefs = $Player.getRoutineSpeedByReference()
		var numberRefs2 = $Player.getRoutineMultiplicityByReference()
		$MyTabContainer/InnerContainer/Training.initialiseNumberRefs(numberRefs, numberRefs2)
		firstProcess = false

	#var temp = get_global_mouse_position()
	#Input.warp_mouse(temp+motion)
	#motion = Vector2(0,0)
	#var p = get_global_mouse_position()
	#if p == last:
		#print("Mouse frozen frame")
	#last = p


	var attributeMods = $Player.getAttributeMods()
	#var modDictionary = $Player.getModifierDictionary()
	#var routineSpeeds : Array[float] = []
	#for index in range(Definitions.otherStatEnum.routineSpeed_0, Definitions.otherStatEnum.routineSpeed_4+1) :
		#routineSpeeds.append(modDictionary["otherStat"][Definitions.otherStatDictionary[index]])
	$MyTabContainer/InnerContainer/Training.setPlayerMods(attributeMods)
	updatePlayer()

func updatePlayer() :
	## GameScreen needs to know all of the data and where to get it that Player needs each frame, but not how it's created or how it's used.
	## This is the complete list of "direct" and "specific" modifiers detailed in Player.gd.
	## Player.gd does not actually need to know the complete list of direct modifiers but I will add it there as comments for clarity
	## Direct
	# Permanent
	for key in permanentMods.keys() :
		$Player.updateDirectModifier(key, permanentMods[key])
	#Equipment
	var equipmentDirect : ModifierPacket = $MyTabContainer/InnerContainer/Equipment.getDirectModifiers()
	$Player.updateDirectModifier("Equipment", equipmentDirect)
	var elementDirect : ModifierPacket = $MyTabContainer/InnerContainer/Equipment.getElementalDirectModifiers($Player.getSubclass())
	$Player.updateDirectModifier("Element Synergy", elementDirect)
	## Specific
	#Training
	var newTrainingLevels : Array[int] = $MyTabContainer/InnerContainer/Training.getAttributeLevels()
	$Player.updateTrainingLevels(newTrainingLevels)
	#Weapon
	var newWeapon : Weapon = $MyTabContainer/InnerContainer/Equipment.getCurrentWeapon()
	$Player.updateWeapon(newWeapon)
	var newArmor : Armor = $MyTabContainer/InnerContainer/Equipment.getCurrentArmor()
	$Player.updateArmor(newArmor)
	var newAccessory : Accessory = $MyTabContainer/InnerContainer/Equipment.getCurrentAccessory()
	$Player.updateAccessory(newAccessory)
	$Player.myUpdate()
#############################################
## New game initialisation
func initialisePlayerCharacter(val) :
	$Player.setFromCharacter(val)
######################################
## Signals
func _item_list_inspection(itemList : Array[Node], emitter : Node) :
	var directUpgradeSettings = IGOptions.getIGOptionsCopy()["filter"]["directDowngrade"]
	var discardFromCombatRewards = directUpgradeSettings[0]
	var discardFromInventory = directUpgradeSettings[1]
	var discardFromEquipped = directUpgradeSettings[2]
	var discardLegendaries = directUpgradeSettings[3]
	var discardSignatures = directUpgradeSettings[4]
	if !discardFromCombatRewards && !discardFromInventory && !discardFromEquipped :
		emitter.waitingForResponse = false
		emitter.emit_signal("responseReceived")
		return
	if (discardFromCombatRewards) :
		var deleteList : Array[Node] = []
		for index in range(0, itemList.size()) :
			if ($MyTabContainer/InnerContainer/Equipment.hasDirectUpgrade(itemList[index].getItemSceneRef().core)) :
				var groups : EquipmentGroups = itemList[index].getItemSceneRef().core.equipmentGroups
				if ((groups.quality == EquipmentGroups.qualityEnum.legendary&&!discardLegendaries) || (groups.isSignature && !discardSignatures)) :
					continue
				deleteList.append(itemList[index].getItemSceneRef())
		await emitter.removeItemsFromList_internal(deleteList)
		itemList = emitter.getItemList()
	if (discardFromInventory || discardFromEquipped) :
		var deleteList : Array[Node] = []
		for index in range(0,itemList.size()) :
			var replaced = $MyTabContainer/InnerContainer/Equipment.replaceDirectDowngrades(itemList[index].getItemSceneRef().core, discardFromInventory, discardFromEquipped, discardLegendaries, discardSignatures)
			if (replaced) :
				deleteList.append(itemList[index].getItemSceneRef())
		await emitter.removeItemsFromList_internal(deleteList)
		itemList = emitter.getItemList()

var permanentMods : Dictionary = {}
func _shopping_permanent_modifier(value : Array[float], type : String, statEnum : Array[int], source : String, modType, isMultiplicative : bool, recursiveCall : bool) :
	if (permanentMods.get(source) == null) :
		permanentMods[source] = ModifierPacket.new()
	for index in range(0,value.size()) :
		if (type == "otherStat" && statEnum[index] == Definitions.otherStatEnum.routineSpeed_5) :
			var newValue : Array[float] = []
			var newEnum : Array[int] = []
			for index2 in range(Definitions.otherStatEnum.routineSpeed_0, Definitions.otherStatEnum.routineSpeed_4+1) :
				newValue.append(value[index])
				newEnum.append(index2)
			_shopping_permanent_modifier(newValue, type, newEnum, source, modType, isMultiplicative, true)
			if (isMultiplicative) :
				permanentMods[source].otherMods[Definitions.otherStatDictionary[Definitions.otherStatEnum.routineSpeed_5]][modType] *= value[index]
			else :
				permanentMods[source].otherMods[Definitions.otherStatDictionary[Definitions.otherStatEnum.routineSpeed_5]] += value[index]
		elif (type == "attribute" && isMultiplicative) :
			permanentMods[source].attributeMods[Definitions.attributeDictionary[statEnum[index]]][modType] *= value[index]
		elif (type == "attribute" && !isMultiplicative) :
			permanentMods[source].attributeMods[Definitions.attributeDictionary[statEnum[index]]][modType] += value[index]
		elif (type == "stat" && isMultiplicative) :
			permanentMods[source].statMods[Definitions.baseStatDictionary[statEnum[index]]][modType] *= value[index]
		elif (type == "stat" && !isMultiplicative) :
			permanentMods[source].statMods[Definitions.baseStatDictionary[statEnum[index]]][modType] += value[index]
		elif (type == "otherStat" && isMultiplicative) :
			permanentMods[source].otherMods[Definitions.otherStatDictionary[statEnum[index]]][modType] *= value[index]
		elif (type == "otherStat" && !isMultiplicative) :
			permanentMods[source].otherMods[Definitions.otherStatDictionary[statEnum[index]]][modType] += value[index]
		else :
			if (!recursiveCall) :
				Shopping.provideConfirmation(false)
	if (!recursiveCall) :
		Shopping.provideConfirmation(true)
	
func _on_equip_item(itemSceneRef) :
	var type
	if (itemSceneRef.getType() == Definitions.equipmentTypeEnum.armor) :
		type = "armor"
	elif (itemSceneRef.getType() == Definitions.equipmentTypeEnum.weapon) :
		type = "weapon"
	else :
		return
	var reforges = itemSceneRef.getReforges()
	Shopping.onEquippedItem(type, reforges)
	
func _on_unequip_item(itemSceneRef) :
	var type
	if (itemSceneRef.getType() == Definitions.equipmentTypeEnum.armor) :
		type = "armor"
	elif (itemSceneRef.getType() == Definitions.equipmentTypeEnum.weapon) :
		type = "weapon"
	else :
		return
	Shopping.onUnequippedItem(type)
	
func _shopping_equipment_scaling_requested() :
	Shopping.provideEquipmentScaling($MyTabContainer/InnerContainer/Combat.getMostRecentEquipmentScaling())
	
func _shopping_currency_scaling_requested(type) :
	Shopping.provideCurrencyScaling($MyTabContainer/InnerContainer/Combat.getMostRecentCurrencyScaling(type))
	
func _shopping_random_item_requested(type : Definitions.equipmentTypeEnum) :
	var item = $MyTabContainer/InnerContainer/Combat.createRandomShopItem(type)
	Shopping.provideRandomItem(item)
	
func _shopping_add_to_inventory(item : Equipment) :
	if ($MyTabContainer/InnerContainer/Equipment.isInventoryFull()) :
		Shopping.provideConfirmation(false)
	else :
		var itemName = item.getItemName()
		var itemSceneRef = SceneLoader.createEquipmentScene(itemName)
		itemSceneRef.core = item
		$MyTabContainer/InnerContainer/Equipment.addItemToInventory(itemSceneRef)
		Shopping.provideConfirmation(true)

func _shopping_reforge_item_requested(type : Definitions.equipmentTypeEnum) :
	if (type == Definitions.equipmentTypeEnum.weapon) :
		Shopping.lastBought["equipmentReforged"] = $MyTabContainer/InnerContainer/Equipment.getCurrentWeapon()
	else :
		Shopping.lastBought["equipmentReforged"] = $MyTabContainer/InnerContainer/Equipment.getCurrentArmor()
	var newScalingVal = $MyTabContainer/InnerContainer/Combat.getMostRecentEquipmentScaling()
	var rows = $MyTabContainer/InnerContainer/Combat.getScalingRows()
	Shopping.provideConfirmation($MyTabContainer/InnerContainer/Equipment.reforge(type, newScalingVal, rows))
	
func _shopping_set_subclass_requested(subclass : Definitions.subclass) :
	var confirmationPopup = binaryPopupLoader.instantiate()
	add_child(confirmationPopup)
	confirmationPopup.setTitle("Purchasing Subclass: " + Definitions.subclassDictionary[subclass])
	confirmationPopup.setText("Are you sure you want to choose this subclass? You cannot change it later without respeccing (which is expensive).")
	confirmationPopup.setButton0Name(" GIVE ME THE STATS ")
	confirmationPopup.setButton1Name(" Never mind ")
	var choice = await confirmationPopup.binaryChosen
	if (choice == 0) :
		$Player.setSubclass(subclass)
		Shopping.provideConfirmation(true)
	else :
		Shopping.provideConfirmation(false)
	remove_child(confirmationPopup)
	confirmationPopup.queue_free()
	
func _shopping_inventory_size_requested() :
	$MyTabContainer/InnerContainer/Equipment.expandInventory(1)
	Shopping.provideConfirmation(true)
	
const binaryPopupLoader = preload("res://Graphic Elements/popups/binary_decision.tscn")
const popupLoader2 = preload("res://Graphic Elements/popups/my_popup.tscn")
const fighterClass = preload("res://Globals/Definitions/Classes/fighter.tres")
const rogueClass = preload("res://Globals/Definitions/Classes/rogue.tres")
const mageClass = preload("res://Globals/Definitions/Classes/mage.tres")
func _shopping_respec_requested() :
	var confirmationPopup = binaryPopupLoader.instantiate()
	add_child(confirmationPopup)
	confirmationPopup.setTitle("Change Class")
	confirmationPopup.setText("Are you sure you want to change your class? You will lose half your Cumulative Training Levels, but you may select a different class or subclass.")
	confirmationPopup.setButton0Name(" I'm ready ")
	confirmationPopup.setButton1Name(" I changed my mind ")
	var choice = await confirmationPopup.binaryChosen
	if (choice == 1) :
		Shopping.provideConfirmation(false)
		return
	var classPopup = popupLoader2.instantiate()
	add_child(classPopup)
	classPopup.setText("")
	classPopup.setTitle("Choose Your Class")
	classPopup.addButtonContainer()
	classPopup.addButton("Fighter")
	classPopup.addButton("Rogue")
	classPopup.addButton("Mage")
	classPopup.addButton("Cancel")
	choice = await classPopup.buttonPressed
	if (choice == 3) :
		Shopping.provideConfirmation(false)
	else :
		$MyTabContainer/InnerContainer/Training.onRespec()
		var newClass
		if (choice == 0) :
			newClass = fighterClass
		elif (choice == 1) :
			newClass = rogueClass
		else :
			newClass = mageClass
		$Player.setClass(newClass)
		$Player.setSubclass(-1)
		Shopping.resetSubclass()
		Shopping.provideConfirmation(true)
	classPopup.queue_free()
	
func _shopping_upgrade_routine(routine : AttributeTraining) :
	$MyTabContainer/InnerContainer/Training.upgradeRoutine(routine)
	Shopping.provideConfirmation(true)
	
func _shopping_remove_currency(item : Currency, val : int) :
	var currentAmount = $TopRibbon/Ribbon/Currency.getCurrencyAmount(item)
	var newAmount = max(currentAmount-val,0)
	$TopRibbon/Ribbon/Currency.setCurrencyAmount(item, newAmount)

func _on_tab_pressed(emitter : Button) :
	for child in $TabContainer.get_children() :
		if (child.name == emitter.name) :
			child.visible = true
		else :
			child.visible = false
			

func _on_options_button_pressed(_emitter) -> void:
	optionsMenuRef = optionsMenuLoader.instantiate()
	add_child(optionsMenuRef)
	
const encyclopediaLoader = preload("res://Graphic Elements/popups/encyclopedia.tscn")
func _on_encyclopedia_button_pressed(_emitter) :
	encyclopediaRef = encyclopediaLoader.instantiate()
	add_child(encyclopediaRef)
	
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
func _on_combat_add_to_inventory_requested(itemSceneRef) -> void:
	if (itemSceneRef.core is Currency) :
		$TopRibbon/Ribbon/Currency.addToCurrency(itemSceneRef, itemSceneRef.getCount())
		$MyTabContainer/InnerContainer/Combat.removeCombatRewardEntry(itemSceneRef)
	elif (myInventoryFull()) :
		#if (isAutomatic && IGOptions_inventoryBehaviour == inventoryBehaviour_local.discard) :
			#$MyTabContainer/InnerContainer/Combat.removeCombatRewardEntry(itemSceneRef)
		## (!isAutomatic || isAutomatic && IGOptions_inventoryBehaviour == wait
		#else :
		displayInventoryFull()
		$MyTabContainer/InnerContainer/Combat.denyAddToInventory()
	else :
		#Does not account for randomly generated bonuses if you ever add that sorry
		## I did add that thank you
		var newItem = SceneLoader.createEquipmentScene(itemSceneRef.core.getItemName())
		newItem.core = itemSceneRef.core
		$MyTabContainer/InnerContainer/Equipment.addItemToInventory(newItem)
		$MyTabContainer/InnerContainer/Combat.removeCombatRewardEntry(itemSceneRef)
		
func _on_tutorial_requested(tutorialName : Encyclopedia.tutorialName, tutorialPos : Vector2) :
	if (tutorialName == Encyclopedia.tutorialName.tutorialFloorEnd) :
		$Player.enableLearningCurve()
	if (tutorialName == Encyclopedia.tutorialName.tutorialFloor3 && !tabsUnlocked[1]) :
		$MyTabContainer.revealChild($MyTabContainer/InnerContainer/Training)
		tabsUnlocked[1] = true
	elif (tutorialName == Encyclopedia.tutorialName.equipment && !tabsUnlocked[2]) :
		$MyTabContainer.revealChild($MyTabContainer/InnerContainer/Equipment)
		tabsUnlocked[2] = true
	elif (tutorialName == Encyclopedia.tutorialName.tutorialFloorEnd && !tabsUnlocked[1]) :
		$MyTabContainer.revealChild($MyTabContainer/InnerContainer/Training)
		tabsUnlocked[1] = true
	elif (tutorialName == Encyclopedia.tutorialName.herophile) :
		_on_routine_unlock_requested(self, MegaFile.getRoutine("spar_herophile"))
		Shopping.unlockHerophile()
	$TutorialManager.queueTutorial(tutorialName, tutorialPos)
	
func _on_player_core_requested(emitter) -> void:
	emitter.providePlayerCore($Player.getCore())
func _on_player_class_requested(emitter) -> void:
	emitter.providePlayerClass($Player.getClass())
func _on_player_subclass_requested(emitter) -> void :
	emitter.providePlayerSubclass($Player.getSubclass())
func _on_weapon_resource_requested(emitter) -> void :
	emitter.provideWeaponResource(EquipmentDatabase.getEquipment($MyTabContainer/InnerContainer/Equipment.getCurrentWeapon().getItemName()))
func _on_player_portrait_requested(emitter) -> void :
	emitter.provideplayerPortrait($Player.getPortrait())
func _on_routine_unlock_requested(emitter, routine : AttributeTraining) :
	$MyTabContainer/InnerContainer/Training.unlockRoutine(routine)
	if (emitter == Shopping) :
		Shopping.provideConfirmation(true)
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
const routineShopLoader = preload("res://Graphic Elements/Shop/routine_shop.tscn")
const equipmentShopLoader = preload("res://Graphic Elements/Shop/equipment_shop.tscn")
const soulShopLoader = preload("res://Graphic Elements/Shop/soul_shop.tscn")


func getShopOrNull() :
	if (!$MyTabContainer/InnerContainer/Combat.has_node("Shop")) :
		return null
	return $MyTabContainer/InnerContainer/Combat.get_node("Shop")
func _on_shop_requested(details : ShopDetails) :
	var oldShopExists = $MyTabContainer/InnerContainer/Combat.has_node("Shop")
	if (oldShopExists) :
		var oldShop = getShopOrNull()
		$MyTabContainer/InnerContainer/Combat.remove_child(oldShop)
		oldShop.queue_free()
		await get_tree().process_frame
	var newShop
	if (details.shopName == Shopping.shopNames["armor"] || details.shopName == Shopping.shopNames["weapon"]) :
		newShop = equipmentShopLoader.instantiate()
	elif (details.shopName == Shopping.shopNames["soul"]) :
		newShop = soulShopLoader.instantiate()
	else :
		newShop = routineShopLoader.instantiate()
	$MyTabContainer/InnerContainer/Combat.add_child(newShop)
	newShop.name = "Shop"
	if (!newShop.myReady) :
		await newShop.myReadySignal
	newShop.connect("currencyAmountRequested", _on_currency_amount_requested)
	newShop.connect("finished", _on_shop_finished)
	if (newShop.has_signal("playerClassRequested")) :
		newShop.connect("playerClassRequested", _on_player_class_requested)
	if (newShop.has_signal("playerSubclassRequested")) :
		newShop.connect("playerSubclassRequested", _on_player_subclass_requested)
	if (newShop.has_signal("currentlyEquippedItemRequested")) :
		newShop.connect("currentlyEquippedItemRequested", _on_currently_equipped_item_requested)
	$MyTabContainer/InnerContainer/Combat.disableUI()
	if (details.shopName == Shopping.shopNames["routine"]) :
		Shopping.routineUnlocked = true
		$MyTabContainer/InnerContainer/Combat.enableShopShortcut("routine")
	elif (details.shopName == Shopping.shopNames["soul"]) :
		Shopping.soulUnlocked = true
		$MyTabContainer/InnerContainer/Combat.enableShopShortcut("soul")
	elif (details.shopName == Shopping.shopNames["weapon"]) :
		$MyTabContainer/InnerContainer/Combat.enableShopShortcut("weapon")
	elif (details.shopName == Shopping.shopNames["armor"]) :
		$MyTabContainer/InnerContainer/Combat.enableShopShortcut("armor")
	newShop.setFromDetails(details)
	
func _on_currently_equipped_item_requested(emitter, type) :
	var ret = $MyTabContainer/InnerContainer/Equipment.getEquippedItem(type)
	if (ret != null) :
		emitter.provideCurrentlyEquippedItem(ret.core)
	else :
		emitter.provideCurrentlyEquippedItem(null)

func _on_shop_shortcut_selected(type : String) :
	var details
	if (type == "routine") :
		details = Shopping.createRoutineShop()
	elif (type == "armor") :
		details = Shopping.createArmorShop()
	elif (type == "weapon") :
		details = Shopping.createWeaponShop()
	elif (type == "soul") :
		details = Shopping.createSoulShop()
	else :
		return 
	_on_shop_requested(details)
	
func _on_shop_finished() :
	$MyTabContainer/InnerContainer/Combat.enableUIIfPaused()
	
func _on_currency_amount_requested(item : Currency, emitter : Node) :
	emitter.provideCurrencyAmount($TopRibbon/Ribbon/Currency.getCurrencyAmount(item))
	
#func _on_reforge_hovered_requested(emitter) :
	#var shop = $MyTabContainer/InnerContainer/Combat.get_node("Shop")
	#if (shop == null) :
		#emitter.provideIsReforgeHovered(false)
	#elif (!shop.has_method("getReforgeHovered")) :
		#emitter.provideIsReforgeHovered(false)
	#else :
		#emitter.provideIsReforgeHovered(shop.getReforgeHovered())

func _on_magicFind_requested(emitter) :
	var magicFind = $Player.getOtherStat(Definitions.otherStatEnum.magicFind)
	emitter.provideMagicFind(magicFind)
	
func _on_add_child_requested(emitter, node : Node) :
	add_child(node)
	if (emitter.has_method("createBigEntry")) :
		enemyEntryBigPopup = node
	emitter.unlockAddChild()
func _on_player_modifier_dictionary_requested(emitter) :
	var currentAccessory = $MyTabContainer/InnerContainer/Equipment.getCurrentAccessory()
	emitter.providePlayerModifierDictionary($Player.getModifierDictionary(), currentAccessory != null && currentAccessory.getItemName() == "ring_authority")
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
	if (event.is_action("Options")) :
		accept_event()
		handleOptions(event)

var enemyEntryBigPopup : Node = null
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
			elif (encyclopediaRef != null) :
				if (encyclopediaRef.hasNestedPopup()) :
					encyclopediaRef.closeOutermostNestedPopup()
				else :
					encyclopediaRef.queue_free()
					encyclopediaRef = null
			elif (optionsMenuRef != null) :
				if (optionsMenuRef.hasNestedPopup()) :
					optionsMenuRef.closeOutermostNestedPopup()
				else :
					optionsMenuRef.queue_free()
					optionsMenuRef = null
			elif (enemyEntryBigPopup != null) :
				enemyEntryBigPopup.queue_free()
				enemyEntryBigPopup = null
			elif (getShopOrNull() != null) :
				getShopOrNull()._on_exit_pressed()
			else :
				_on_save_button_pressed(self)
		escWasPressed = false

var optionsWasPressed = false
func handleOptions(event) :
	if (event.is_pressed()) :
		optionsWasPressed = true
	else :
		if (optionsWasPressed) :
			if (optionsMenuRef != null) :
				optionsMenuRef.queue_free()
				optionsMenuRef = null
			else :
				_on_options_button_pressed(self)
		optionsWasPressed = false

var encyclopediaRef
func handleMenu(event) :
	if (event.is_pressed()) :
		menuWasPressed = true
	else :
		if (menuWasPressed) :
			if (encyclopediaRef != null) :
				encyclopediaRef.queue_free()
				encyclopediaRef = null
			else :
				_on_encyclopedia_button_pressed(self)
		menuWasPressed = false
			
########################################
## Saving
	
var tabsUnlocked : Array
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	tempDict["tabsUnlocked"] = tabsUnlocked
	tempDict["permanentMods"] = {}
	for key in permanentMods.keys() :
		tempDict["permanentMods"][key] = permanentMods[key].getSaveDictionary()
	tempDict["apophisKilled"] = apophisKilledOnThisFile
	return tempDict
		
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	#Input.set_use_accumulated_input(false)
	#Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
	#Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	myReady = true
	emit_signal("myReadySignal")
	
func beforeLoad(newSave) :
	myReady = false
	## Wait for annoying dependency
	if (!$MyTabContainer.myReady) :
		await $MyTabContainer.actuallyReady
	if (newSave) :
		$Player.setTypicalEnemyDefense($MyTabContainer/InnerContainer/Combat.getTypicalEnemyDefense(0))
	Shopping.connect("addPermanentModifierRequested", _shopping_permanent_modifier)
	Shopping.connect("upgradeRoutineRequested", _shopping_upgrade_routine)
	Shopping.connect("unlockRoutineRequested", _on_routine_unlock_requested)
	Shopping.connect("currencyAmountRequested", _on_currency_amount_requested)
	Shopping.connect("removeCurrencyRequested", _shopping_remove_currency)
	Shopping.connect("addToInventoryRequested", _shopping_add_to_inventory)
	Shopping.connect("randomItemRequested", _shopping_random_item_requested)
	Shopping.connect("reforgeItemRequested", _shopping_reforge_item_requested)
	Shopping.connect("equipmentScalingRequested", _shopping_equipment_scaling_requested)
	Shopping.connect("currencyScalingRequested", _shopping_currency_scaling_requested)
	Shopping.connect("setSubclassRequested", _shopping_set_subclass_requested)
	Shopping.connect("respecRequested", _shopping_respec_requested)
	Shopping.connect("increaseInventorySizeRequested", _shopping_inventory_size_requested)
	Shopping.connect("playerClassRequested", _on_player_class_requested)
	if (newSave) :
		permanentMods = {}
		Shopping.resetItemPrices()
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
		if (tab.has_signal("addChildRequested")) :
			tab.connect("addChildRequested", _on_add_child_requested)
		if (tab.has_signal("playerModifierDictionaryRequested")) :
			tab.connect("playerModifierDictionaryRequested", _on_player_modifier_dictionary_requested)
		if (tab.has_signal("equippedItem")) :
			tab.connect("equippedItem", _on_equip_item)
		if (tab.has_signal("unequippedItem")) :
			tab.connect("unequippedItem", _on_unequip_item)
		if (tab.has_signal("playerSubclassRequested")) :
			tab.connect("playerSubclassRequested", _on_player_subclass_requested)
		if (tab.has_signal("weaponResourceRequested")) :
			tab.connect("weaponResourceRequested", _on_weapon_resource_requested)
		if (tab.has_signal("playerPortraitRequested")) :
			tab.connect("playerPortraitRequested", _on_player_portrait_requested)
		if (tab.has_signal("apophisKilled")) :
			tab.connect("apophisKilled", createApophisScreen)
		if (tab.has_signal("rewardPending")) :
			tab.connect("rewardPending", _on_reward_pending)
		if (tab.has_signal("shopShortcutSelected")) :
			tab.connect("shopShortcutSelected", _on_shop_shortcut_selected)
		if (tab.has_signal("itemListForYourInspectionGoodSir")) :
			tab.connect("itemListForYourInspectionGoodSir", _item_list_inspection)
		if (tab.has_signal("currentlyEquippedItemRequested")) :
			tab.connect("currentlyEquippedItemRequested", _on_currently_equipped_item_requested)
		#if (tab.has_signal("isReforgedHoveredRequested")) :
			#tab.connect("isReforgedHoveredRequested", _on_reforge_hovered_requested)
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
	myReady = true
	emit_signal("myReadySignal")
	if (newSave) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
				
func onLoad(loadDict : Dictionary) -> void :
	myReady = false
	updateSaveToV105(loadDict)
	tabsUnlocked = loadDict["tabsUnlocked"]
	for index in range(0,tabsUnlocked.size()) :
		if (!tabsUnlocked[index]) :
			$MyTabContainer.hideChild($MyTabContainer/InnerContainer.get_child(index))
	for key in loadDict["permanentMods"].keys() :
		permanentMods[key] = ModifierPacket.createFromSaveDictionary(loadDict["permanentMods"][key])
	apophisKilledOnThisFile = loadDict["apophisKilled"]
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")

func updateSaveToV105(loadDict : Dictionary) :
	for key in loadDict["permanentMods"].keys() :
		if (key == Shopping.routinePurchasableDictionary[Shopping.routinePurchasable.mixed]) :
			continue
		var modPacket = loadDict["permanentMods"][key]
		var otherSection = modPacket["other"]
		var tempRoutineMultiplicity = otherSection.get(Definitions.otherStatDictionary[Definitions.otherStatEnum.routineMultiplicity])
		if (tempRoutineMultiplicity == null) :
			otherSection[Definitions.otherStatDictionary[Definitions.otherStatEnum.routineMultiplicity]] = ModifierPacket.StandardModifier.duplicate()
			
	var refinedFundamentals = loadDict["permanentMods"].get(Shopping.routinePurchasableDictionary[Shopping.routinePurchasable.mixed])
	if (refinedFundamentals == null) :
		return
	var routineMultiplicity = refinedFundamentals["other"].get(Definitions.otherStatDictionary[Definitions.otherStatEnum.routineMultiplicity])
	if (routineMultiplicity == null) :
		refinedFundamentals["other"][Definitions.otherStatDictionary[Definitions.otherStatEnum.routineMultiplicity]] = ModifierPacket.StandardModifier.duplicate()
		routineMultiplicity = refinedFundamentals["other"][Definitions.otherStatDictionary[Definitions.otherStatEnum.routineMultiplicity]]
	else :
		return
	var routineEffect = refinedFundamentals["other"].get(Definitions.otherStatDictionary[Definitions.otherStatEnum.routineEffect])
	if (routineEffect == null) :
		refinedFundamentals["other"][Definitions.otherStatDictionary[Definitions.otherStatEnum.routineEffect]] = ModifierPacket.StandardModifier.duplicate()
		routineEffect = refinedFundamentals["other"][Definitions.otherStatDictionary[Definitions.otherStatEnum.routineEffect]]
	for key in routineMultiplicity.keys() :
		routineMultiplicity[key] = routineEffect[key]
	refinedFundamentals["other"][Definitions.otherStatDictionary[Definitions.otherStatEnum.routineEffect]] = ModifierPacket.StandardModifier.duplicate()
	for index in range(Definitions.otherStatEnum.routineSpeed_0, Definitions.otherStatEnum.routineSpeed_4+1) :
		refinedFundamentals["other"][Definitions.otherStatDictionary[index]]["Premultiplier"] = refinedFundamentals["other"][Definitions.otherStatDictionary[Definitions.otherStatEnum.routineSpeed_5]]["Premultiplier"]

func onLoad_2() :
	var combat = $MyTabContainer/InnerContainer/Combat
	if (Shopping.routineUnlocked) :
		combat.enableShopShortcut("routine")
	if (Shopping.weaponUnlocked) :
		combat.enableShopShortcut("weapon")
	if (Shopping.armorUnlocked) :
		combat.enableShopShortcut("armor")
	if (Shopping.soulUnlocked) :
		combat.enableShopShortcut("soul")

func updateFromOptions(optionsCopy : Dictionary) :
	pass
	#IGOptions_inventoryBehaviour = optionsCopy[IGOptions.options.inventoryBehaviour] as inventoryBehaviour_local

var apophisKilledOnThisFile : bool = false
func createApophisScreen() :
	if (apophisKilledOnThisFile) :
		return
	var newPopup = binaryPopupLoader.instantiate()
	add_child(newPopup)
	newPopup.setTitle("The Demon King is Slain!")
	newPopup.setText("Congratulations, you've defeated the Demon King! The Surface world is saved! Or something. This game was going to have a more in depth story but game development is hard. In any case, there are actually 20 biomes, 25 bosses, and 10 factions in this game, and levels 1-9 are randomly generated! So I encourage you to check out endless mode or try one of the other classes. Do be aware that endless mode will have integer overflow issues eventually! Thanks for playing!")
	newPopup.setButton0Name(" Continue playing (begin endless mode) ")
	newPopup.setButton1Name(" Return to Main Menu ")
	var choice = await newPopup.binaryChosen
	if (choice == 1) :
		_exit_to_menu()
	elif (choice == 0) :
		apophisKilledOnThisFile = true
		SaveManager.queueSaveGame(SaveManager.currentSlot)

const characterCreationLoader = preload("res://Screens/character_creation_new.tscn")
func _on_change_appearance_button_pressed() -> void:
	var topLayer = CanvasLayer.new()
	add_child(topLayer)
	topLayer.layer = 99
	var newScreen = characterCreationLoader.instantiate()
	topLayer.add_child(newScreen)
	var currentChar : CharacterPacket = $Player.getCharacterPacket()
	newScreen.setLineEditText(currentChar.getName())
	newScreen.initialiseAppearanceChange($Player.getCharacterPacket())
	var ret = await newScreen.characterDone
	var character : CharacterPacket = ret[0]
	$Player.setFromCharacter(character)
	topLayer.queue_free()
	
const altTheme = preload("res://Graphic Elements/Themes/mainTab_red.tres")
func _on_reward_pending() :
	$MyTabContainer.setThemeOfButtonUntilActive($MyTabContainer/InnerContainer/Combat, altTheme)

#var motion = Vector2(0,0)
#func _input(event):
	#if (event is InputEventMouseMotion) :
		#motion += event.relative
	#if event is InputEventMouseMotion:
		#if Engine.get_frames_drawn() % 300 == 0:
			#print("delta:", event.relative.length())
