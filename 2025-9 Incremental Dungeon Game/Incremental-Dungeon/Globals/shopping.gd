extends Node
############################################################
## Saving
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	self.add_to_group("Saveable")
	myReady = true
	emit_signal("myReadySignal")
	
var lastBought = {
	"boughtRoutine" : null,
	"upgradedRoutine" : null,
	"equipmentBought" : null,
	"equipmentReforged" : null,
	"statsBought" : null
}
func getLastUnlockedRoutine() -> AttributeTraining :
	if (lastBought["boughtRoutine"] == null) :
		return AttributeTraining.new()
	return lastBought["boughtRoutine"]
func getLastUpgradedRoutine() -> AttributeTraining :
	if (lastBought["upgradedRoutine"] == null) :
		return AttributeTraining.new()
	return lastBought["upgradedRoutine"]
func getLastCreatedItem() -> Equipment :
	if (lastBought["equipmentBought"] == null) :
		return Equipment.new()
	return lastBought["equipmentBought"]
func getLastReforgedItem() -> Equipment :
	if (lastBought["equipmentReforged"] == null) :
		return Equipment.new()
	return lastBought["equipmentReforged"]
func getLastUpgradedString() -> Array[String] :
	if (lastBought["statsBought"] == null) :
		return [""]
	else :
		return lastBought["statsBought"]
	
func getSaveDictionary() -> Dictionary :
	var retVal = {}
	retVal["allRoutines"] = allRoutinesPurchased
	retVal["unlockedRoutines"] = unlockedRoutines
	retVal["itemPrices"] = itemPrices
	retVal["armorUnlocked"] = armorUnlocked
	retVal["weaponUnlocked"] = weaponUnlocked
	retVal["routineUnlocked"] = routineUnlocked
	retVal["soulUnlocked"] = soulUnlocked
	if (armorTimer.is_stopped()) :
		retVal["armorTime"] = "stopped"
	else :
		retVal["armorTime"] = armorTimer.time_left
	if (weaponTimer.is_stopped()) :
		retVal["weaponTime"] = "stopped"
	else :
		retVal["weaponTime"] = weaponTimer.time_left
	retVal["myArmor"] = []
	retVal["myArmorNames"] = []
	for armor in myArmor :
		if (armor == null) :
			retVal["myArmor"].append("null")
		else :
			retVal["myArmor"].append(armor.getItemName())
			retVal["myArmorNames"].append(armor.getName())
	retVal["armorScaling"] = armorScaling
	retVal["myWeapons"] = []
	retVal["myWeaponNames"] = []
	for weapon in myWeapons :
		if (weapon == null) :
			retVal["myWeapons"].append("null")
		else :
			retVal["myWeapons"].append(weapon.getItemName())
			retVal["myWeaponNames"].append(weapon.getName())
	retVal["weaponScaling"] = weaponScaling
	retVal["subclassPurchased"] = subclassPurchased
	return retVal

func beforeLoad(newGame) :
	myReady = false
	resetSaveSpecificVariants()
	setupDescriptions()
	setupTimers()
	if (newGame) :
		#var routineList = MegaFile.Routine_FilesDictionary.keys()
		unlockedRoutines.append("lift_weights")
		unlockedRoutines.append("pickpocket_goblins")
		unlockedRoutines.append("hug_cacti")
		unlockedRoutines.append("read_novels")
		unlockedRoutines.append("spar")
	myReady = true
	emit_signal("myReadySignal")
	if (newGame) :
		doneLoading = true
		emit_signal("doneLoadingSignal")

var armorUnlocked : bool = false
var weaponUnlocked : bool = false
func onLoad(loadDict) :
	myReady = false
	updateToVersion105(loadDict)
	updateToVersion107(loadDict)
	allRoutinesPurchased = loadDict["allRoutines"]
	if (loadDict.get("unlockedRoutines") != null) :
		unlockedRoutines = loadDict["unlockedRoutines"]
	if (loadDict.get("itemPrices") != null) :
		itemPrices = loadDict["itemPrices"]
	var tempArmor
	var tempWeapon
	if (!(loadDict["armorTime"] is String && loadDict["armorTime"] == "stopped")) :
		tempArmor = loadDict["armorTime"]
	else :
		tempArmor = -1
	if (!(loadDict["weaponTime"] is String && loadDict["weaponTime"] == "stopped")) :
		tempWeapon = loadDict["weaponTime"]
	else :
		tempWeapon = -1
	armorUnlocked = loadDict["armorUnlocked"]
	weaponUnlocked = loadDict["weaponUnlocked"]
	routineUnlocked = loadDict["routineUnlocked"]
	soulUnlocked = loadDict["soulUnlocked"]
	if (armorUnlocked) :
		armorTimer.start(tempArmor)
	if (weaponUnlocked) :
		weaponTimer.start(tempWeapon)
	weaponScaling = loadDict["weaponScaling"]
	armorScaling = loadDict["armorScaling"]
	var weapons = loadDict["myWeapons"]
	for index in range(0, weapons.size()) :
		var weapon = weapons[index]
		if (weapon == "null") :
			myWeapons.append(null)
		else :
			myWeapons.append(EquipmentDatabase.getEquipment(weapon).getAdjustedCopy(weaponScaling))
			if (loadDict.get("myWeaponNames") != null) :
				myWeapons.back().setTitle(loadDict["myWeaponNames"][index])
	var armors = loadDict["myArmor"]
	for index in range(0,armors.size()) :
		var armor = armors[index]
		if (armor == "null") :
			myArmor.append(null)
		else :
			myArmor.append(EquipmentDatabase.getEquipment(armor).getAdjustedCopy(armorScaling))
			if (loadDict.get("myArmorNames") != null) :
				myArmor.back().setTitle(loadDict["myArmorNames"][index])
	subclassPurchased = loadDict["subclassPurchased"]
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")
		
func updateToVersion105(loadDict : Dictionary) :
	var oldBase = 130000
	var oldFactor = 10
	var currentPrice = loadDict["itemPrices"]["routine"][routinePurchasable.mixed]
	var magnitude = log(currentPrice/oldBase)/log(10)
	if (is_equal_approx(magnitude, floor(magnitude)) || is_equal_approx(magnitude, ceil(magnitude))) :
		loadDict["itemPrices"]["routine"][routinePurchasable.mixed] = itemPriceBase["routine"][routinePurchasable.mixed] * pow(10,magnitude)
		
func updateToVersion107(loadDict : Dictionary) :
	for index in range(0,loadDict["unlockedRoutines"].size()) :
		if (loadDict["unlockedRoutines"][index].find("Resource#") != -1) :
			loadDict["unlockedRoutines"][index] = "spar_herophile"
	if (loadDict["allRoutines"]) :
		return
	var routineList = MegaFile.getAllRoutine()
	var mySet : bool = true
	var herophileUnlocked : bool = false
	for routine in routineList :
		if (routine.getResourceName() == "spar_herophile" && loadDict["unlockedRoutines"].find("spar_herophile") != -1) :
			herophileUnlocked = true
			continue
		if (loadDict["unlockedRoutines"].find(routine.getResourceName()) == -1) :
			mySet = false
			break
	if (mySet) :
		loadDict["allRoutines"] = true
		loadDict["unlockedRoutines"] = []
		for routine in routineList :
			if (routine == MegaFile.getRoutine("spar_herophile") && !herophileUnlocked) :
				continue
			loadDict["unlockedRoutines"].append(routine.getResourceName())
###################################################################################
## General
var subclassPurchased : bool = false
func resetSubclass() :
	subclassPurchased = false
func getSubclassPurchased() :
	return subclassPurchased
var routineUnlocked : bool = false
var soulUnlocked : bool = false
func resetSaveSpecificVariants() :
	allRoutinesPurchased = false
	armorUnlocked = false
	weaponUnlocked = false
	routineUnlocked = false
	soulUnlocked = false
	waitingOnEquipmentScaling = false
	waitingOnCurrencyScaling = false
	awaitingConfirmation = false
	waitingForCurrencyAmount = false
	waitingForRandomItem = false
	resetItemPrices()
	unlockedRoutines = []
	if (armorTimer != null) :
		armorTimer.queue_free()
		armorTimer = null
	if (weaponTimer != null) :
		weaponTimer.queue_free()
		weaponTimer = null
	myArmor = []
	myWeapons = []
	armorScaling = 0
	weaponScaling = 0
	routineDescriptionDictionary = {}
	subclassPurchased = false
	
signal removeCurrencyRequested
const requestPurchase_currencyNames = ["gold_coin", "ore", "soul"]
func purchaseItem(type : String, item : int, purchase : Purchasable) -> bool:
	var currency : Currency
	if (type == "routine") :
		currency = EquipmentDatabase.getEquipment(requestPurchase_currencyNames[0])
	elif (type == "armor" || type == "weapon") :
		currency = EquipmentDatabase.getEquipment(requestPurchase_currencyNames[1])
	elif (type == "soul") :
		currency = EquipmentDatabase.getEquipment(requestPurchase_currencyNames[2])
	else :
		return false
	var currencyCount = await getCurrencyAmount(currency)
	if (currencyCount < purchase.purchasablePrice) :
		AudioHandler.playMenuSfx(AudioHandler.menuSfx.warning)
		return false
	if (await givePurchaseBenefit(type, item, purchase)) :
		if (type == "soul" && (item >= soulPurchasable.fighterSubclass_1 && item <= soulPurchasable.mageSubclass_2)) :
			AudioHandler.playMenuSfx(AudioHandler.menuSfx.save)
		else :
			AudioHandler.playMenuSfx(AudioHandler.menuSfx.select)
		emit_signal("removeCurrencyRequested", currency, purchase.purchasablePrice)
	else :
		return false
	var price = getNextItemPrice(type, item)
	if (price < 9*pow(10,18)) :
		itemPrices[type][item] = int(price)
	else :
		itemPrices[type][item] = price
	return true
	
func givePurchaseBenefit(type : String, item : int, purchase : Purchasable) -> bool:
	confirmed = false
	awaitingConfirmation = true
	if (type == "routine") :
		givePurchaseBenefit_routine(item)
	elif (type == "armor") :
		givePurchaseBenefit_armor(item, purchase)
	elif (type == "weapon") :
		givePurchaseBenefit_weapon(item, purchase)
	elif (type == "soul") :
		givePurchaseBenefitSoul(item, purchase)
	else :
		return false
	if (awaitingConfirmation) :
		await confirmationReceived
	return confirmed

func getNextItemPrice(type, item) -> float :
	var currentVal = itemPrices[type][item]
	if (currentVal == -1) :
		return -1
	if (type == "routine") :
		return getNextItemPrice_routine(item)
	elif (type == "armor") :
		return getNextItemPrice_armor(item) 
	elif (type == "weapon") :
		return getNextItemPrice_weapon(item)
	elif (type == "soul") :
		return getNextItemPrice_soul(item)
	else :
		return -1

func getNextItemPrice_routine(item) -> float :
	var standardScale = pow(2,0.25)
	var currentVal = itemPrices["routine"][item]
	if (item == routinePurchasable.speed) :
		if (currentVal == 6) :
			return round(11.14)
		elif (currentVal == 11) :
			return round(21.43)
		else :
			return currentVal*standardScale
	elif (item == routinePurchasable.effect) :
		if (currentVal == 8) :
			return round(14.86)
		elif (currentVal == 15) :
			return round(28.57)
		else :
			return currentVal * standardScale
	elif (item == routinePurchasable.mixed) :
		return round(currentVal * 100.0)
	## It's 25x more expensive (5000) at the end of floor 5, after 9 purchases
	elif (item == routinePurchasable.randomRoutine) :
		return round(currentVal * 1.43)
	## Buy once per floor ad infinitum
	elif (item == routinePurchasable.upgradeRoutine) :
		return round(currentVal * 2.3784)
	else :
		return -1
		
func getNextItemPrice_armor(item) -> float :
	var currentVal = itemPrices["armor"][item]
	if (item == armorPurchasable.premadeArmor) :
		## One time purchase
		return currentVal
	elif (item == armorPurchasable.newArmor) :
		## Arbitrary
		return round(currentVal * 1.2)
	elif (item == armorPurchasable.reforge) :
		return round(currentVal * 1.5)
	elif (item == armorPurchasable.statUpgrade_phys || item == armorPurchasable.statUpgrade_mag) :
		## Intended to purchase 7-8 times throughout the game. The 9th time will cost 20 minutes of grinding.
		return round(currentVal * 2.484)
	else :
		return -1
func getNextItemPrice_weapon(item) -> float :
	var currentVal = itemPrices["weapon"][item]
	if (item == weaponPurchasable.premadeWeapon) :
		## One time purchase
		return currentVal
	elif (item == weaponPurchasable.newWeapon) :
		## Arbitrary
		return round(currentVal * 1.2)
	elif (item == weaponPurchasable.reforge) :
		return round(currentVal * 1.5)
	elif (item == weaponPurchasable.statUpgrade_DR) :
		## Intended to purchase 7-8 times throughout the game
		return round(currentVal * 2.484)
	else :
		return -1
		
func getNextItemPrice_soul(item) -> float :
	var currentVal = itemPrices["soul"][item]
	if ((soulPurchasable.fighterSubclass_1 as int) <= (item as int) && (item as int) <= (soulPurchasable.mageSubclass_2 as int)) :
		return currentVal
	elif (item == soulPurchasable.respec) :
		return currentVal
	## arbitrary
	elif (item == soulPurchasable.inventorySpace) :
		return round(currentVal * 1.5)
	## Sould be able to use it 12 times before the final boss.
	elif (item == soulPurchasable.randomStat) :
		return round(currentVal * 1.59)
	else :
		return -1
		
##################################################################################
## Communication
signal equipmentScalingRequested
signal equipmentScalingReceived
var waitingOnEquipmentScaling : bool = false
var equipmentScaling_comm : float = 0
func getEquipmentScaling() :
	waitingOnEquipmentScaling = true
	emit_signal("equipmentScalingRequested")
	if (waitingOnEquipmentScaling) :
		await equipmentScalingReceived
	return equipmentScaling_comm
func provideEquipmentScaling(val) :
	equipmentScaling_comm = val
	waitingOnEquipmentScaling = false
	emit_signal("equipmentScalingReceived")
	
signal currencyScalingRequested
signal currencyScalingReceived
var waitingOnCurrencyScaling : bool = false
var currencyScaling_comm : float = 0
func getCurrencyScaling(type) :
	waitingOnCurrencyScaling = true
	emit_signal("currencyScalingRequested", type)
	if (waitingOnCurrencyScaling) :
		await currencyScalingReceived
	return currencyScaling_comm
func provideCurrencyScaling(val) :
	currencyScaling_comm = val
	waitingOnCurrencyScaling = false
	emit_signal("currencyScalingReceived")

signal addPermanentModifierRequested
signal confirmationReceived
var awaitingConfirmation : bool = false
var confirmed : bool = false
func provideConfirmation(val) :
	confirmed = val
	awaitingConfirmation = false
	emit_signal("confirmationReceived")

signal currencyAmountRequested
signal currencyAmountReceived
var currencyAmount_comm : float
var waitingForCurrencyAmount : bool = false
func getCurrencyAmount(item : Currency) -> float :
	waitingForCurrencyAmount = true
	emit_signal("currencyAmountRequested", item, self)
	if (waitingForCurrencyAmount) :
		await currencyAmountReceived
	return currencyAmount_comm
func provideCurrencyAmount(val) :
	currencyAmount_comm = val
	waitingForCurrencyAmount = false
	emit_signal("currencyAmountReceived")
	
func addNewShop(emitter : Node) :
	if (!emitter.has_connections("purchaseRequested")) :
		emitter.connect("purchaseRequested", _on_purchase_requested)
	
func _on_purchase_requested(item, _price, myCurrency, emitter, purchase : Purchasable) :
	var type = shopNames.find_key(emitter.getName())
	var index
	if (type == "routine") :
		index = routinePurchasableDictionary.find_key(item)
	elif (type == "armor") :
		index = armorPurchasableDictionary.find_key(item)
	elif (type == "weapon") :
		index = weaponPurchasableDictionary.find_key(item)
	elif (type == "soul") :
		index = soulPurchasableDictionary.find_key(item)
	else :
		return
	if (await purchaseItem(type, index, purchase)) :
		emitter.setCurrencyAmount(await getCurrencyAmount(myCurrency))
		if (purchase.equipment_optional != null) :
			emitter.onEquipmentSold()
			onEquipmentSold(type, purchase)
		elif (type == "soul" && soulPurchasable.fighterSubclass_1 <= index && index <= soulPurchasable.mageSubclass_2) :
			subclassPurchased = true
			emitter.onSubclassPurchased()
		elif (type == "soul" && index == soulPurchasable.respec) :
			emitter.reset()
		elif (type == "routine" && index == routinePurchasable.randomRoutine) :
			var threshold
			if (unlockedRoutines.find("spar_herophile")) :
				threshold = 16
			else :
				threshold = 15
			if (unlockedRoutines.size() >= threshold) :
				allRoutinesPurchased = true
			emitter.refreshPrice(item, itemPrices[type][index])
		else :
			emitter.refreshPrice(item, itemPrices[type][index])
		emitter.softNotification(purchase)

var allRoutinesPurchased : bool = false
func getAllRoutinesPurchased() :
	return allRoutinesPurchased
			
func onEquipmentSold(type, purchase : Purchasable) :
	if (type == "armor") :
		for index in range(0, myArmor.size()) :
			if (myArmor[index] != null && myArmor[index].getItemName() == purchase.equipment_optional.getItemName()) :
				myArmor[index] = null
				break
	elif (type == "weapon") :
		for index in range(0, myWeapons.size()) :
			if (myWeapons[index] != null && myWeapons[index].getItemName() == purchase.equipment_optional.getItemName()) :
				myWeapons[index] = null
				break
			
func onEquippedItem(type, reforges) :
	var currencyScaling = await getCurrencyScaling("armor")
	if (type == "armor") :
		currencyScaling /= pow(2,5.0/4.0)
		itemPrices["armor"][armorPurchasable.reforge]= itemPriceBase.duplicate(true)["armor"][armorPurchasable.reforge] * currencyScaling * pow(1.5,reforges)
	elif (type == "weapon") :
		currencyScaling /= pow(2,5.0/4.0)
		itemPrices["weapon"][weaponPurchasable.reforge]= itemPriceBase.duplicate(true)["weapon"][weaponPurchasable.reforge] * currencyScaling * pow(1.5,reforges)
	else :
		return
	
func onUnequippedItem(type) :
	if (type == "armor") :
		itemPrices["armor"][armorPurchasable.reforge] = 0
	elif (type == "weapon") :
		itemPrices["weapon"][weaponPurchasable.reforge] = 0
	else :
		return
		
signal randomItemRequested
var waitingForRandomItem : bool = false
signal randomItemReceived
var randomItem_comm : Equipment = null
func createRandomItem(type : Definitions.equipmentTypeEnum) :
	waitingForRandomItem = true
	emit_signal("randomItemRequested", type)
	if (waitingForRandomItem) :
		await randomItemReceived
	return randomItem_comm
func provideRandomItem(val : Equipment) :
	randomItem_comm = val
	waitingForRandomItem = false
	emit_signal("randomItemReceived")
func createRandomArmor() :
	return await createRandomItem(Definitions.equipmentTypeEnum.armor)
func createRandomWeapon() :
	return await createRandomItem(Definitions.equipmentTypeEnum.weapon)
	
############################################################################################################
## Hardcoded or default values
enum routinePurchasable {
	speed,
	effect,
	mixed,
	randomRoutine,
	upgradeRoutine
}
const routinePurchasableDictionary = {
	routinePurchasable.speed : "Improved Form",
	routinePurchasable.effect : "Improved Equipment",
	routinePurchasable.mixed : "Refined Fundamentals",
	routinePurchasable.randomRoutine : "Obtain random routine",
	routinePurchasable.upgradeRoutine : "Upgrade random routine"
}
var routineDescriptionDictionary = {}
enum armorPurchasable {
	premadeArmor,
	newArmor,
	reforge,
	statUpgrade_phys,
	statUpgrade_mag
}
const armorPurchasableDictionary = {
	armorPurchasable.premadeArmor : "premade_int",
	armorPurchasable.newArmor : "Buy Random Armor",
	armorPurchasable.reforge : "Reforge",
	armorPurchasable.statUpgrade_phys : "Upgrade PHYSDEF",
	armorPurchasable.statUpgrade_mag : "Upgrade MAGDEF"
}
enum weaponPurchasable {
	premadeWeapon,
	newWeapon,
	reforge,
	statUpgrade_DR
}
const weaponPurchasableDictionary = {
	weaponPurchasable.premadeWeapon : "premade_int",
	weaponPurchasable.newWeapon : "Buy Random Weapon",
	weaponPurchasable.reforge : "Reforge",
	weaponPurchasable.statUpgrade_DR : "Upgrade DR"
}
const itemPriceBase : Dictionary = {
	"routine" : {
		##10 minutes of farming at node 5 yields 2 of each (40)
		routinePurchasable.speed : 6,
		routinePurchasable.effect : 8,
		routinePurchasable.mixed : 300000,
		routinePurchasable.randomRoutine : 40, 
		## 8 routines
		routinePurchasable.upgradeRoutine : 1000
	},
	## Currency is 1x ore
	## Expected currency/node at start is 48
	"armor" : {
		armorPurchasable.premadeArmor: 24,
		armorPurchasable.newArmor : 18,
		armorPurchasable.reforge : 24,
		armorPurchasable.statUpgrade_phys : 40,
		armorPurchasable.statUpgrade_mag : 40
	},
	"weapon" : {
		weaponPurchasable.premadeWeapon : 24,
		weaponPurchasable.newWeapon : 24,
		weaponPurchasable.reforge : 16,
		weaponPurchasable.statUpgrade_DR : 95
	},
	"soul" : {
		soulPurchasable.fighterSubclass_1 : 1,
		soulPurchasable.fighterSubclass_2 : 1,
		soulPurchasable.rogueSubclass_1 : 1,
		soulPurchasable.rogueSubclass_2 : 1,
		soulPurchasable.mageSubclass_1 : 1,
		soulPurchasable.mageSubclass_2 : 1,
		soulPurchasable.respec : -1,
		soulPurchasable.randomStat : 80,
		soulPurchasable.inventorySpace : 20
	}
}
enum soulPurchasable{
	fighterSubclass_1,
	fighterSubclass_2,
	rogueSubclass_1,
	rogueSubclass_2,
	mageSubclass_1,
	mageSubclass_2,
	inventorySpace,
	respec,
	randomStat,
}
const soulPurchasableDictionary = {
	soulPurchasable.fighterSubclass_1 : "Subclass: Barbarian",
	soulPurchasable.fighterSubclass_2 : "Subclass: Knight",
	soulPurchasable.rogueSubclass_1 : "Subclass: Munitions Expert",
	soulPurchasable.rogueSubclass_2 : "Subclass: Whirlwind",
	soulPurchasable.mageSubclass_1 : "Subclass: Enchanter",
	soulPurchasable.mageSubclass_2 : "Subclass: Soul Collector",
	soulPurchasable.inventorySpace : "Inventory Space",
	soulPurchasable.respec : "Reset Class",
	#soulPurchasable.magicFind : "Increase Magic Find",
	soulPurchasable.randomStat : "Stat Roulette"
}
func getEquipmentTime(type : Definitions.equipmentTypeEnum) -> float :
	if (type == Definitions.equipmentTypeEnum.armor) :
		return armorTimer.time_left
	elif (type == Definitions.equipmentTypeEnum.weapon) :
		return weaponTimer.time_left
	else :
		return -1
		
func getReforgePrice(type : Definitions.equipmentTypeEnum) -> int :
	if (type == Definitions.equipmentTypeEnum.armor) :
		return itemPrices["armor"][armorPurchasable.reforge]
	elif (type == Definitions.equipmentTypeEnum.weapon) :
		return itemPrices["weapon"][weaponPurchasable.reforge]
	else :
		return 0
	
func _on_equipment_shop_launched(type : Definitions.equipmentTypeEnum) :
	if (type == Definitions.equipmentTypeEnum.armor) :
		if (!armorUnlocked) :
			armorUnlocked = true
			_on_armor_timeout()
			armorTimer.start()
	elif (type == Definitions.equipmentTypeEnum.weapon) :
		if (!weaponUnlocked) :
			weaponUnlocked = true
			_on_weapon_timeout()
			weaponTimer.start()
	else :
		return
		
var itemPrices : Dictionary = {}
func resetItemPrices() :
	for key in itemPriceBase.keys() :
		itemPrices[key] = itemPriceBase.duplicate(true)[key]
const shopNames = {
	"routine" : "Training Instructor",
	"armor" : "Armory",
	"weapon" : "Weaponsmith",
	"soul" : "Soul Trader"
}
func createShop(shopType) -> ShopDetails :
	if (shopType == "routine") :
		return createRoutineShop()
	elif (shopType == "armor") :
		return await createArmorShop()
	elif (shopType == "weapon") :
		return await createWeaponShop()
	elif (shopType == "soul") :
		return await createSoulShop()
	else :
		return ShopDetails.new()
func createRoutineShop() -> ShopDetails :
	var retVal = ShopDetails.new()
	retVal.shopName = shopNames["routine"]
	retVal.shopCurrency = EquipmentDatabase.getEquipment("gold_coin")
	retVal.shopCurrencyTexture = SceneLoader.createEquipmentScene("gold_coin").getTextureCopy()
	var column1 : ShopColumn = ShopColumn.new()
	column1.columnName = "Enhance Routines"
	column1.purchasables = []
	for key in range(routinePurchasable.speed, routinePurchasable.randomRoutine) :
		var newItem : Purchasable = Purchasable.new()
		newItem.purchasableName = routinePurchasableDictionary[key]
		newItem.description = routineDescriptionDictionary[key]
		newItem.purchasablePrice = itemPrices["routine"][key]
		newItem.equipment_optional = null
		column1.purchasables.append(newItem)
	var column2 : ShopColumn = ShopColumn.new()
	column2.columnName = "New Routines"
	column2.purchasables = []
	for key in range(routinePurchasable.randomRoutine, routinePurchasable.upgradeRoutine + 1) :
		var newItem : Purchasable = Purchasable.new()
		newItem.purchasableName = routinePurchasableDictionary[key]
		newItem.description = routineDescriptionDictionary[key]
		newItem.purchasablePrice = itemPrices["routine"][key]
		newItem.equipment_optional = null
		column2.purchasables.append(newItem)
	var columnArr : Array[ShopColumn] = [column1, column2]
	retVal.shopContents = columnArr
	return retVal

func createArmorShop() :
	if (!armorUnlocked) :
		_on_equipment_shop_launched(Definitions.equipmentTypeEnum.armor)
	var retVal = ShopDetails.new()
	retVal.shopName = shopNames["armor"]
	retVal.shopCurrency = EquipmentDatabase.getEquipment("ore")
	retVal.shopCurrencyTexture = SceneLoader.createEquipmentScene("ore").getTextureCopy()
	var column1 : ShopColumn = ShopColumn.new()
	column1.columnName = "Armor for Sale"
	column1.purchasables = []
	for index in range(0,myArmor.size()) :
		var newItem : Purchasable = Purchasable.new()
		if (myArmor[index] == null) :
			continue
		newItem.equipment_optional = myArmor[index]
		newItem.purchasableName = armorPurchasableDictionary[armorPurchasable.premadeArmor]
		newItem.description = ""
		newItem.purchasablePrice = itemPrices["armor"][armorPurchasable.premadeArmor] * (1 + 0.1 * (newItem.equipment_optional.equipmentGroups.quality as int))
		column1.purchasables.append(newItem)
		
	var column2 : ShopColumn = ShopColumn.new()
	column2.columnName = "Forge Armor"
	column2.purchasables = []
	var col2Item : Purchasable = Purchasable.new()
	col2Item.equipment_optional = null
	col2Item.purchasableName = armorPurchasableDictionary[armorPurchasable.newArmor]
	col2Item.description = "Get a random armor! Price increases each purchase, but is reset on shop refresh. Elemental equipment has a 0.5x drop rate penalty."
	col2Item.purchasablePrice = itemPrices["armor"][armorPurchasable.newArmor]
	column2.purchasables.append(col2Item)

	var col3Item : Purchasable = Purchasable.new()
	col3Item.equipment_optional = null
	col3Item.purchasableName = armorPurchasableDictionary[armorPurchasable.reforge]
	col3Item.description = "Upgrade the stats of your currently equipped Armor to be as if dropped by your strongest defeated enemy! Price increases per-item with every use."
	col3Item.purchasablePrice = itemPrices["armor"][armorPurchasable.reforge]
	column2.purchasables.append(col3Item)
	
	var column4 : ShopColumn = ShopColumn.new()
	column4.columnName = "Stat Upgrades"
	column4.purchasables = []
	var col4Phys : Purchasable = Purchasable.new()
	col4Phys.equipment_optional = null
	col4Phys.purchasableName = armorPurchasableDictionary[armorPurchasable.statUpgrade_phys]
	col4Phys.description = "Permanently increase your PHYSDEF Standard Multiplier by +0.05"
	col4Phys.purchasablePrice = itemPrices["armor"][armorPurchasable.statUpgrade_phys]
	column4.purchasables.append(col4Phys)
	var col4Mag : Purchasable = Purchasable.new()
	col4Mag.equipment_optional = null
	col4Mag.purchasableName = armorPurchasableDictionary[armorPurchasable.statUpgrade_mag]
	col4Mag.description = "Permanently increase your MAGDEF Standard Multiplier by +0.05"
	col4Mag.purchasablePrice = itemPrices["armor"][armorPurchasable.statUpgrade_mag]
	column4.purchasables.append(col4Mag)
	
	var columnArr : Array[ShopColumn] = [column1, column2, column4]
	retVal.shopContents = columnArr
	return retVal
	
func createWeaponShop():
	if (!weaponUnlocked) :
		_on_equipment_shop_launched(Definitions.equipmentTypeEnum.weapon)
	var retVal = ShopDetails.new()
	retVal.shopName = shopNames["weapon"]
	retVal.shopCurrency = EquipmentDatabase.getEquipment("ore")
	retVal.shopCurrencyTexture = SceneLoader.createEquipmentScene("ore").getTextureCopy()
	var column1 : ShopColumn = ShopColumn.new()
	column1.columnName = "Weapons for Sale"
	column1.purchasables = []
	for index in range(0,myWeapons.size()) :
		var newItem : Purchasable = Purchasable.new()
		if (myWeapons[index] == null) :
			continue
		newItem.equipment_optional = myWeapons[index]
		newItem.purchasableName = weaponPurchasableDictionary[weaponPurchasable.premadeWeapon]
		newItem.description = ""
		newItem.purchasablePrice = itemPrices["weapon"][weaponPurchasable.premadeWeapon] * (1 + 0.1 * (newItem.equipment_optional.equipmentGroups.quality as int))
		column1.purchasables.append(newItem)
		
	var column2 : ShopColumn = ShopColumn.new()
	column2.columnName = "Forge Weapons"
	column2.purchasables = []
	var col2Item : Purchasable = Purchasable.new()
	col2Item.equipment_optional = null
	col2Item.purchasableName = weaponPurchasableDictionary[weaponPurchasable.newWeapon]
	col2Item.description = "Get a random weapon! Price increases each purchase, but is reset on shop refresh. Elemental equipment has a 0.5x drop rate penalty."
	col2Item.purchasablePrice = itemPrices["weapon"][weaponPurchasable.newWeapon]
	column2.purchasables.append(col2Item)
	
	var col3Item : Purchasable = Purchasable.new()
	col3Item.equipment_optional = null
	col3Item.purchasableName = weaponPurchasableDictionary[weaponPurchasable.reforge]
	col3Item.description = "Upgrade the stats of your currently equipped Weapon to be as if dropped by your strongest defeated enemy! Price increases per-item with every use."
	col3Item.purchasablePrice = itemPrices["weapon"][weaponPurchasable.reforge]
	column2.purchasables.append(col3Item)
	
	var column4 : ShopColumn = ShopColumn.new()
	column4.columnName = "Stat Upgrades"
	column4.purchasables = []
	var col4Phys : Purchasable = Purchasable.new()
	col4Phys.equipment_optional = null
	col4Phys.purchasableName = weaponPurchasableDictionary[weaponPurchasable.statUpgrade_DR]
	col4Phys.description = "Permanently increase your DR Standard Multiplier by +0.05"
	col4Phys.purchasablePrice = itemPrices["weapon"][weaponPurchasable.statUpgrade_DR]
	column4.purchasables.append(col4Phys)
	
	var columnArr : Array[ShopColumn] = [column1, column2, column4]
	retVal.shopContents = columnArr
	return retVal

func createSoulShop() :
	var retVal = ShopDetails.new()
	retVal.shopName = shopNames["soul"]
	retVal.shopCurrency = EquipmentDatabase.getEquipment("soul")
	retVal.shopCurrencyTexture = SceneLoader.createEquipmentScene("soul").getTextureCopy()
	
	var column1 : ShopColumn = ShopColumn.new()
	column1.columnName = "Unlock Your Potential"
	column1.purchasables = []
	for index in range(soulPurchasable.fighterSubclass_1 as int, (soulPurchasable.mageSubclass_2 as int)+1):
		var newItem = Purchasable.new()
		newItem.equipment_optional = null
		newItem.purchasableName = soulPurchasableDictionary[index]
		newItem.description = Definitions.subclassDescriptions[index as Definitions.subclass]
		newItem.purchasablePrice = 1
		column1.purchasables.append(newItem)
	var respecItem = Purchasable.new()
	respecItem.equipment_optional = null
	respecItem.purchasableName = soulPurchasableDictionary[soulPurchasable.respec]
	respecItem.description = "Remove your subclass (if any) and choose a new class! [color=red]You will lose 25% of your [/color]Cumulative Routine Levels[color=red].[/color]"
	respecItem.purchasablePrice = -1
	column1.purchasables.append(respecItem)
	
	var column2 : ShopColumn = ShopColumn.new()
	column2.columnName = "Upgrade Your Soul [font_size=20](or your backpack)[/font_size]"
	column2.purchasables = []
	var spaceItem = Purchasable.new()
	spaceItem.equipment_optional = null
	spaceItem.purchasableName = soulPurchasableDictionary[soulPurchasable.inventorySpace]
	spaceItem.description = "Get +1 inventory slot."
	spaceItem.purchasablePrice = itemPrices["soul"][soulPurchasable.inventorySpace]
	column2.purchasables.append(spaceItem)
	
	var rouletteItem = Purchasable.new()
	rouletteItem.equipment_optional = null
	rouletteItem.purchasableName = soulPurchasableDictionary[soulPurchasable.randomStat]
	rouletteItem.description = "Spend an outrageous sum to permanently increase a random stat! No refunds."
	rouletteItem.purchasablePrice = itemPrices["soul"][soulPurchasable.randomStat]
	column2.purchasables.append(rouletteItem)
	
	var columnArr : Array[ShopColumn] = [column1, column2]
	retVal.shopContents = columnArr
	return retVal
	
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
	
################################################################################
## Purchase benefits
signal unlockRoutineRequested
signal upgradeRoutineRequested
var unlockedRoutines : Array = []
func givePurchaseBenefit_routine(item : routinePurchasable) :
	var value : Array[float]
	var type : String
	var statEnum : Array[int]
	var isMultiplicative : bool
	var source = routinePurchasableDictionary[item]
	var modType = "Premultiplier"
	
	if (item == routinePurchasable.speed) :
		value = [pow(2,0.125)]
		type = "otherStat"
		statEnum = [Definitions.otherStatEnum.routineSpeed_5]
		isMultiplicative = true
		awaitingConfirmation = true
		emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)
	elif (item == routinePurchasable.effect) :
		value = [pow(2,0.125)]
		type = "otherStat"
		statEnum = [Definitions.otherStatEnum.routineEffect]
		isMultiplicative = true
		awaitingConfirmation = true
		emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)
	elif (item == routinePurchasable.mixed) :
		value = [0.1, 10.9]
		type = "otherStat"
		statEnum = [Definitions.otherStatEnum.routineSpeed_5, Definitions.otherStatEnum.routineMultiplicity]
		isMultiplicative = true
		awaitingConfirmation = true
		emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)

	elif (item == routinePurchasable.randomRoutine) :
		if (unlockedRoutines.size() == 5) :
			var sublist = ["fence_swordfish","fire","ice"]
			var playerClass = (await getPlayerClass()).classEnum
			if (playerClass == Definitions.classEnum.fighter) :
				sublist.remove_at(2)
			elif (playerClass == Definitions.classEnum.mage) :
				sublist.remove_at(0)
			else :
				sublist.remove_at(1)
			var chosenRoutine = sublist[randi_range(0,1)]
			unlockedRoutines.append(chosenRoutine)
			lastBought["boughtRoutine"] = MegaFile.getRoutine(chosenRoutine)
			emit_signal("unlockRoutineRequested", self, lastBought["boughtRoutine"])
			return
		
		
		var routineList = MegaFile.getAllRoutine()
		var optionCount = routineList.size()-1-unlockedRoutines.size()
		var roll = randi_range(0, optionCount-1)
		var count = 0
		for index in range(0,routineList.size()) :
			if (unlockedRoutines.find(routineList[index].getResourceName()) != -1) :
				continue
			elif (count != roll) :
				count += 1
			elif (routineList[index] == MegaFile.getRoutine("spar_herophile")) :
				unlockedRoutines.append(routineList[index+1].getResourceName())
			else :
				unlockedRoutines.append(routineList[index].getResourceName())
				break
		lastBought["boughtRoutine"] = MegaFile.getRoutine(unlockedRoutines.back())
		awaitingConfirmation = true
		emit_signal("unlockRoutineRequested", self, lastBought["boughtRoutine"])
	elif (item == routinePurchasable.upgradeRoutine) :
		var routineList = MegaFile.getAllRoutine()
		var upgraded = routineList[randi_range(0,routineList.size()-1)]
		if (unlockedRoutines.find("spar_herophile") == -1) :
			while (upgraded == MegaFile.getRoutine("spar_herophile")) :
				upgraded = routineList[randi_range(0,routineList.size()-1)]
		lastBought["upgradedRoutine"] = upgraded
		awaitingConfirmation = true
		emit_signal("upgradeRoutineRequested", upgraded)
	else :
		return
		
var herophileUnlocked : bool = false
func unlockHerophile() :
	if (unlockedRoutines.find("spar_herophile") == -1) :
		unlockedRoutines.append("spar_herophile")
	var settings = SaveManager.getGlobalSettings()
	settings["herophile"] = true
	SaveManager.saveGlobalSettings(settings)
	SaveManager.queueSaveGame(Definitions.saveSlots.current)
signal addToInventoryRequested
signal reforgeItemRequested
func givePurchaseBenefit_armor(item : armorPurchasable, purchase : Purchasable) :
	if (item == armorPurchasable.premadeArmor) :
		awaitingConfirmation = true
		emit_signal("addToInventoryRequested", purchase.equipment_optional)
	elif (item == armorPurchasable.newArmor) :
		var newArmor = await createRandomArmor()
		lastBought["equipmentBought"] = newArmor
		awaitingConfirmation = true
		emit_signal("addToInventoryRequested", newArmor)
	elif (item == armorPurchasable.reforge) :
		##Inventory keeps track of what's dragged off
		awaitingConfirmation = true
		emit_signal("reforgeItemRequested", Definitions.equipmentTypeEnum.armor)
	elif (item == armorPurchasable.statUpgrade_phys || item == armorPurchasable.statUpgrade_mag) :
		var value : Array[float] = [0.05]
		var type = "stat"
		var statEnum : Array[int]
		if (item == armorPurchasable.statUpgrade_phys) :
			statEnum = [Definitions.baseStatEnum.PHYSDEF]
		elif (item == armorPurchasable.statUpgrade_mag) :
			statEnum = [Definitions.baseStatEnum.MAGDEF]
		else :
			return
		var isMultiplicative = false
		var source = "Armory Upgrade"
		var modType = "Postmultiplier"
		awaitingConfirmation = true
		emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)
	else :
		return

func givePurchaseBenefit_weapon(item : weaponPurchasable, purchase : Purchasable) :
	if (item == weaponPurchasable.premadeWeapon) :
		awaitingConfirmation = true
		emit_signal("addToInventoryRequested", purchase.equipment_optional)
	elif (item == weaponPurchasable.newWeapon) :
		var newWeapon = await createRandomWeapon()
		awaitingConfirmation = true
		lastBought["equipmentBought"] = newWeapon
		emit_signal("addToInventoryRequested", newWeapon)
	elif (item == weaponPurchasable.reforge) :
		##Inventory keeps track of what's dragged off
		awaitingConfirmation = true
		emit_signal("reforgeItemRequested", Definitions.equipmentTypeEnum.weapon)
	elif (item == weaponPurchasable.statUpgrade_DR) :
		var value : Array[float] = [0.05]
		var type = "stat"
		var statEnum : Array[int] = [Definitions.baseStatEnum.DR]
		var isMultiplicative = false
		var source = "Weaponsmith Upgrade"
		var modType = "Postmultiplier"
		awaitingConfirmation = true
		emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)
	else :
		return

signal setSubclassRequested
signal respecRequested
signal increaseInventorySizeRequested
func givePurchaseBenefitSoul(item : soulPurchasable, _purchase : Purchasable) :
	if ((soulPurchasable.fighterSubclass_1 as int) <= (item as int) && (item as int) <= (soulPurchasable.mageSubclass_2 as int)) :
		awaitingConfirmation = true
		emit_signal("setSubclassRequested", item as Definitions.subclass)
	elif (item == soulPurchasable.respec) :
		awaitingConfirmation = true
		emit_signal("respecRequested")
	elif (item == soulPurchasable.randomStat) :
		var value : Array[float] = []
		var type
		var statEnum : Array[int] = []
		var source = soulPurchasableDictionary[item]
		var modType = "Postmultiplier"
		var isMultiplicative = false
		
		var attrCount = Definitions.attributeDictionary.keys().size()
		var statCount = Definitions.baseStatDictionary.keys().size()
		var otherCount = Definitions.otherStatDictionary.keys().size()
		var totalCount = attrCount + statCount + otherCount
		
		var allUpgradedRoll = randi_range(0,99)
		if (allUpgradedRoll == 0): 
			upgradeAllStats()
			return
		var typeRoll = randi_range(0,totalCount-1)
		if (typeRoll < attrCount-1) :
			type = "attribute"
			var roll = randi_range(0,attrCount-1)
			value.append(0.02)
			statEnum.append(roll)
		elif (typeRoll < attrCount + statCount-1) :
			type = "stat"
			var roll = randi_range(0,statCount-1)
			value.append(0.04)
			statEnum.append(roll)
		else :
			type = "otherStat"
			var roll = randi_range(0,otherCount-1)
			while (roll == Definitions.otherStatEnum.physicalConversion || roll == Definitions.otherStatEnum.magicConversion || (roll >= Definitions.otherStatEnum.routineSpeed_0 && roll < Definitions.otherStatEnum.routineSpeed_5)) :
				roll = randi_range(0,otherCount-1)
			value.append(getOtherStatUpgrade(roll as Definitions.otherStatEnum, false))
			statEnum.append(roll)
		awaitingConfirmation = true
		var statName
		if (type == "attribute") :
			statName = Definitions.attributeDictionary[statEnum[0]]
		elif (type == "stat") :
			statName = Definitions.baseStatDictionary[statEnum[0]]
		else :
			statName = Definitions.otherStatDictionary[statEnum[0]]
		var symbol
		if (value[0] < 0) :
			symbol = ""
		else :
			symbol = "+"
		var ret : Array[String] = [statName + " Mult " + symbol + str(value[0])]
		lastBought["statsBought"] = ret
		emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)
	elif (item == soulPurchasable.inventorySpace) :
		awaitingConfirmation = true
		emit_signal("increaseInventorySizeRequested")
	else :
		return

func upgradeAllStats() :
	var value : Array[float] = []
	var type
	var statEnum : Array[int] = []
	var source = soulPurchasableDictionary[soulPurchasable.randomStat]
	var modType = "Postmultiplier"
	var isMultiplicative = false
	var statName
	var temp : Array[String] = []
	lastBought["statsBought"] = temp
	type = "attribute"
	for key in Definitions.attributeDictionary.keys() :
		value.append(0.01)
		statEnum.append(key)
		statName = Definitions.attributeDictionary[key]
		var temp_2 : String = statName + " Mult +0.01"
		lastBought["statsBought"].append(temp_2)
	awaitingConfirmation = true
	emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)
	if (awaitingConfirmation) :
		await confirmationReceived
	if (!confirmed) :
		return
	value = []
	statEnum = []
	type = "stat"
	for key in Definitions.baseStatDictionary.keys() :
		value.append(0.02)
		statEnum.append(key)
		statName = Definitions.baseStatDictionary[key]
		var temp_2 : String = statName + " Mult +0.02"
		lastBought["statsBought"].append(temp_2)
	awaitingConfirmation = true
	emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)
	if (awaitingConfirmation) :
		await confirmationReceived
	if (!confirmed) :
		return
	value = []
	statEnum = []
	type = "otherStat"
	for key in Definitions.otherStatDictionary.keys() :
		if (key == Definitions.otherStatEnum.physicalConversion || key == Definitions.otherStatEnum.magicConversion) :
			continue
		value.append(getOtherStatUpgrade(key, true))
		statEnum.append(key)
		statName = Definitions.otherStatDictionary[key]
		var symbol
		if (value.back() < 0) :
			symbol = ""
		else :
			symbol = "+"
		var temp2 : String = statName + " Mult " + symbol + str(value.back())
		lastBought["statsBought"].append(temp2)
	awaitingConfirmation = true
	emit_signal("addPermanentModifierRequested", value, type, statEnum, source, modType, isMultiplicative, false)
	
func getOtherStatUpgrade(key : Definitions.otherStatEnum, isAllStat : bool) :
	if (key == Definitions.otherStatEnum.magicConversion || key == Definitions.otherStatEnum.physicalConversion || (key >= Definitions.otherStatEnum.routineSpeed_0 && key < Definitions.otherStatEnum.routineSpeed_5)) :
		return 0
	elif (key == Definitions.otherStatEnum.routineSpeed_5 || key == Definitions.otherStatEnum.routineEffect) :
		if (isAllStat) :
			return 0.02
		else :
			return 0.04
	elif (key == Definitions.otherStatEnum.magicDamageDealt || key == Definitions.otherStatEnum.physicalDamageDealt) :
		if (isAllStat) :
			return 0.01
		else :
			return 0.02
	elif (key == Definitions.otherStatEnum.physicalDamageTaken || key == Definitions.otherStatEnum.magicDamageTaken) :
		if (isAllStat) :
			return -0.01
		else :
			return -0.02
	else :
		return 0.02

func setupDescriptions() :
	var standardScaling = pow(2,0.125)
	var standardScalingString = str(Helpers.myRound(standardScaling, 3))
	var dict1 = routineDescriptionDictionary
	dict1[routinePurchasable.speed] = Definitions.otherStatDictionary[Definitions.otherStatEnum.routineSpeed_5] + " Multiplier [color=green]x" + standardScalingString + "[/color]."
	dict1[routinePurchasable.effect] = Definitions.otherStatDictionary[Definitions.otherStatEnum.routineEffect] + " Multiplier [color=green]x" + standardScalingString + "[/color]."
	dict1[routinePurchasable.mixed] = Definitions.otherStatDictionary[Definitions.otherStatEnum.routineSpeed_5] + " Multiplier [color=red]x" + "0.1" + "[/color]." + "\n"
	dict1[routinePurchasable.mixed] += Definitions.otherStatDictionary[Definitions.otherStatEnum.routineEffect] + " Multiplier [color=green]x" + "10.9" + "[/color]."
	dict1[routinePurchasable.randomRoutine] = "Unlock a random new Routine!"
	dict1[str(routinePurchasable.randomRoutine)+"alt"] = "All routines unlocked!"
	dict1[routinePurchasable.upgradeRoutine] = "Upgrade a random routine! For a single routine, all RGR Standard Multiplier Bonuses [color=green]+" + "0.25" + "[/color]."
	

var armorTimer : Timer
var weaponTimer : Timer
func setupTimers() :
	if (armorTimer != null) :
		armorTimer.queue_free()
		armorTimer = null
	if (weaponTimer != null) :
		weaponTimer.queue_free()
		weaponTimer = null
	armorTimer = Timer.new()
	add_child(armorTimer)
	armorTimer.one_shot = false
	armorTimer.wait_time = 30*60
	armorTimer.connect("timeout", _on_armor_timeout)
	weaponTimer = Timer.new()
	add_child(weaponTimer)
	weaponTimer.one_shot = false
	weaponTimer.wait_time = 30*60
	weaponTimer.connect("timeout", _on_weapon_timeout)
	
var myArmor : Array[Armor] = []
var armorScaling : float = 0
func _on_armor_timeout() :
	armorTimer.wait_time = 30*60
	armorTimer.start()
	armorScaling = await getEquipmentScaling()
	var currencyScaling = await getCurrencyScaling("armor")
	currencyScaling /= pow(2,5.0/4.0)
	var var1 = itemPriceBase.duplicate(true)["armor"][armorPurchasable.premadeArmor]
	var var2 = itemPriceBase.duplicate(true)["armor"][armorPurchasable.newArmor]
	var var3= itemPriceBase.duplicate(true)["armor"][armorPurchasable.reforge]
	itemPrices["armor"][armorPurchasable.premadeArmor] = var1 * currencyScaling
	itemPrices["armor"][armorPurchasable.newArmor] = var2 * currencyScaling
	itemPrices["armor"][armorPurchasable.reforge] = var3 * currencyScaling
	myArmor = []
	for index in range(0, 3) :
		myArmor.append(await createRandomArmor())

var myWeapons : Array[Weapon] = []
var weaponScaling : float = 0
func _on_weapon_timeout() :
	weaponTimer.wait_time = 30*60
	weaponTimer.start()
	weaponScaling = await getEquipmentScaling()
	var currencyScaling = await getCurrencyScaling("weapon")
	currencyScaling /= pow(2,5.0/4.0)
	var var1 = itemPriceBase.duplicate(true)["weapon"][weaponPurchasable.premadeWeapon]
	var var2 = itemPriceBase.duplicate(true)["weapon"][weaponPurchasable.newWeapon]
	var var3= itemPriceBase.duplicate(true)["weapon"][weaponPurchasable.reforge]
	itemPrices["weapon"][weaponPurchasable.premadeWeapon] = var1 * currencyScaling
	itemPrices["weapon"][weaponPurchasable.newWeapon] = var2 * currencyScaling
	itemPrices["weapon"][weaponPurchasable.reforge] = var3 * currencyScaling
	myWeapons = []
	for index in range(0, 3) :
		myWeapons.append(await createRandomWeapon())
	
