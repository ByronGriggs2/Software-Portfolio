extends "res://Graphic Elements/Shop/shop_window.gd"

var playerClass_comm : CharacterClass = null
var waitingForPlayerClass : bool = false
signal playerClassRequested
signal playerClassReceived
func getPlayerClass() -> CharacterClass:
	waitingForPlayerClass = true
	emit_signal("playerClassRequested", self)
	if (waitingForPlayerClass) :
		await playerClassReceived
	return playerClass_comm
func providePlayerClass(val : CharacterClass) :
	playerClass_comm = val
	waitingForPlayerClass = false
	emit_signal("playerClassReceived")
	
signal playerSubclassRequested
signal playerSubclassReceived
var waitingForPlayerSubclass : bool = false
var playerSubclass_comm = -1
func getPlayerSubclass() -> Definitions.subclass :
	waitingForPlayerSubclass = true
	emit_signal("playerSubclassRequested", self)
	if (waitingForPlayerSubclass) :
		await playerSubclassReceived
	return playerSubclass_comm
func providePlayerSubclass(val) :
	playerSubclass_comm = val
	waitingForPlayerSubclass = false
	emit_signal("playerSubclassReceived")

func setFromDetails(det : ShopDetails) :
	var playerClass = await getPlayerClass()
	var acceptableNames : Array[String] = [Shopping.soulPurchasableDictionary[Shopping.soulPurchasable.respec]]
	#if (Shopping.subclassPurchased) :
		#var subclass = await getPlayerSubclass()
		#acceptableNames.append(Shopping.soulPurchasableDictionary[subclass as Shopping.soulPurchasable])
	#else :
	if (playerClass.classEnum == Definitions.classEnum.fighter) :
		acceptableNames.append(Shopping.soulPurchasableDictionary[Shopping.soulPurchasable.fighterSubclass_1])
		acceptableNames.append(Shopping.soulPurchasableDictionary[Shopping.soulPurchasable.fighterSubclass_2])
	elif (playerClass.classEnum == Definitions.classEnum.rogue) :
		acceptableNames.append(Shopping.soulPurchasableDictionary[Shopping.soulPurchasable.rogueSubclass_1])
		acceptableNames.append(Shopping.soulPurchasableDictionary[Shopping.soulPurchasable.rogueSubclass_2])
	elif (playerClass.classEnum == Definitions.classEnum.mage) :
		acceptableNames.append(Shopping.soulPurchasableDictionary[Shopping.soulPurchasable.mageSubclass_1])
		acceptableNames.append(Shopping.soulPurchasableDictionary[Shopping.soulPurchasable.mageSubclass_2])
	var newDet = det
	#var col1 : ShopColumn = newDet.shopContents[0]
	#var purchasables = col1.purchasables
	var index = 0
	while (index < newDet.shopContents[0].purchasables.size()) :
		var purchasable = newDet.shopContents[0].purchasables[index]
		if (acceptableNames.find(purchasable.purchasableName) == -1) :
			newDet.shopContents[0].purchasables.remove_at(index)
		else :
			index += 1
	if (Shopping.subclassPurchased) :
		var chosenSubclass = (await getPlayerSubclass()) %2
		var otherSubclass = (chosenSubclass+1) % 2
		var oldName1 = newDet.shopContents[0].purchasables[chosenSubclass].purchasableName
		var newName1 = "[Purchased] " + oldName1
		newDet.shopContents[0].purchasables[chosenSubclass].purchasableName = newName1
		var oldName2 = newDet.shopContents[0].purchasables[otherSubclass].purchasableName
		var newName2 = "[Locked] " + oldName2
		newDet.shopContents[0].purchasables[otherSubclass].purchasableName = newName2
	super(newDet)
	if (Shopping.subclassPurchased) :
		$VBoxContainer/Shop.get_child(0).get_child(0).get_child(2).set_disabled(true)
		$VBoxContainer/Shop.get_child(0).get_child(0).get_child(3).set_disabled(true)
	if (playerClass.classEnum == Definitions.classEnum.mage) :
		var soul = $SoulCollectorPic.duplicate()
		#remove_child(soul)
		$VBoxContainer/Shop.get_child(0).get_child(0).get_child(3).get_child(1).add_child(soul)
		soul.size_flags_horizontal = Control.SIZE_SHRINK_END
		soul.size_flags_vertical = Control.SIZE_SHRINK_BEGIN
		soul.visible = true

func onSubclassPurchased() :
	#var chosenSubclass = (await getPlayerSubclass()) % 2
	#var chosenIndex = 3-chosenSubclass
	#var otherIndex = 2+chosenSubclass
	##if (chosenSubclass == 0) :
		##otherSubclass = $VBoxContainer/Shop.get_child(0).get_child(0).get_child(3)
	##else :
		##otherSubclass = $VBoxContainer/Shop.get_child(0).get_child(0).get_child(2)
	##$VBoxContainer/Shop.get_child(0).get_child(0).remove_child(otherSubclass)
	##otherSubclass.queue_free()
	##otherSubclass = null
	##await get_tree().process_frame
	#$VBoxContainer/Shop.get_child(0).get_child(0).get_child(chosenIndex).set_disabled(true)
	#$VBoxContainer/Shop.get_child(0).get_child(0).get_child(chosenIndex).set_disabled(true)
	#$VBoxContainer/Shop.get_child(0).get_child(0).get_child(otherIndex).set_disabled(true)
	#var newDet : ShopDetails = shopDetails
	#var notChosenSubclass
	#if (chosenSubclass == 0) :
		#notChosenSubclass = 1
	#else :
		#notChosenSubclass = 0
	#newDet.shopContents[0].purchasables.remove_at(notChosenSubclass)
	reset()

func reset() :
	for child in $VBoxContainer/Shop.get_children() :
		$VBoxContainer/Shop.remove_child(child)
		child.queue_free()
	await get_tree().process_frame
	var newDet = Shopping.createSoulShop()
	setFromDetails(newDet)
