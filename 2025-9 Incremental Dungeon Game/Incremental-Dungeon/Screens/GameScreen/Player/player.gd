extends PanelContainer

## I'm having some trouble visualising what nodes need to know what, so let's specify it here:
## Define a "modifier" as a prebonus, postbonus, premultiplier or postmultiplier

## There are two types of modifiers, direct and specific. Direct modifiers are generic/generalised modifiers and must be passed into Player
## in the form of a dictionary and a tag. The tag specifies the origin of the modifiers so they can 
## be displayed in the player panel, though the origin CANNOT matter in calculation. The dictionary contains 2 dictionaries, each with 4 entries
## "preBonuses" : Array[float]
## "preMultipliers" : Array[float]
## "postBonuses" : Array[float]
## "postMultipliers" : Array[float]
## .......................
## Naturally because this dictionary must be used by totally independent nodes, it should be defined globally.
## Specific modifiers are variables that can take any form, though typically an array of floats. Player stores a 
## copy of each specific modifier which it updates each frame. Player has a specific update function that is to be
## called once all direct and more importantly specific modifiers have been given to it. Only Player knows how 
## the specific modifiers are used to calculate stats.

## So each frame goes like this: GameScreen has a list of all the direct and specific modifiers player uses and which of its children has them.
## It grabs the lists from each in turn with getter chains and then gives them to Player. It then calls the player's special
## update function. The direct modifiers can be set in the attributeObjects/derivedStatObjects as they come in, but the specific
## modifiers are used only in the update function

var core : ActorPreset = null
var characterClass : CharacterClass = null
var characterName : String = "Undefined"
## Numbers displayed on Player Panel
var attributeObjects : Array[NumberClass] = []
var derivedStatObjects : Array[NumberClass] = []
var otherStatObjects : Array[NumberClass] = []
var mySubclass : Definitions.subclass = -1
## Direct Modifiers (not explicitly ackgnowledged in Player.gd)
#Equipment
## Specific Modifiers
var unarmedWeapon : Node = null

var equippedWeapon : Weapon = null
var equippedArmor : Armor = null
var equippedAccessory : Accessory = null
var trainingLevels : Array[float] = []

###############################
## Initialisation
func initialiseNumberObjects() :
	for key in Definitions.attributeDictionary.keys() :
		attributeObjects.append(NumberClass.new())
	for key in Definitions.baseStatDictionary.keys() :
		derivedStatObjects.append(NumberClass.new())
	for key in Definitions.otherStatDictionary.keys() :
		otherStatObjects.append(NumberClass.new())
		otherStatObjects.back().enableReferenceMode()
	otherStatObjects[Definitions.otherStatEnum.magicFind].setPrebonus("Humanoid", 1.0)
	otherStatObjects[Definitions.otherStatEnum.physicalDamageDealt].setPrebonus("Humanoid",1.0)
	otherStatObjects[Definitions.otherStatEnum.physicalDamageTaken].setPrebonus("Humanoid",1.0)
	otherStatObjects[Definitions.otherStatEnum.magicDamageDealt].setPrebonus("Humanoid",1.0)
	otherStatObjects[Definitions.otherStatEnum.magicDamageTaken].setPrebonus("Humanoid",1.0)
	for index in range(0,6) :
		otherStatObjects[Definitions.otherStatEnum.routineSpeed_0 + index].setPrebonus("Humanoid",1.0)
	otherStatObjects[Definitions.otherStatEnum.routineEffect].setPrebonus("Humanoid",1.0)
	otherStatObjects[Definitions.otherStatEnum.routineMultiplicity].setPrebonus("Humanoid", 1.0)
###############################
## Specific Modifiers
func updateTrainingLevels(newLevels : Array[float]) :
	trainingLevels = newLevels
func updateWeapon(val : Weapon) :
	if (val == null) :
		equippedWeapon = unarmedWeapon.core
	else :
		equippedWeapon = val
	core.actions[0] = equippedWeapon.basicAttack
func updateArmor(val : Armor) :
	equippedArmor = val
func updateAccessory(val : Accessory) :
	equippedAccessory = val
func getPortrait() -> Dictionary :
	return myCharacter.getPortraitDictionary()
###############################
## Direct modifiers
func updateDirectModifier(origin : String, val : ModifierPacket) :
	for key in Definitions.attributeDictionary.keys() :
		var tag = Definitions.attributeDictionary[key]
		attributeObjects[key].setPrebonus(origin, val.attributeMods[tag]["Prebonus"])
		attributeObjects[key].setPremultiplier(origin, val.attributeMods[tag]["Premultiplier"])
		attributeObjects[key].setPostbonus(origin, val.attributeMods[tag]["Postbonus"])
		attributeObjects[key].setPostmultiplier(origin, val.attributeMods[tag]["Postmultiplier"])
	for key in Definitions.baseStatDictionary.keys() :
		var tag = Definitions.baseStatDictionary[key]
		derivedStatObjects[key].setPrebonus(origin, val.statMods[tag]["Prebonus"])
		derivedStatObjects[key].setPremultiplier(origin, val.statMods[tag]["Premultiplier"])
		derivedStatObjects[key].setPostbonus(origin, val.statMods[tag]["Postbonus"])
		derivedStatObjects[key].setPostmultiplier(origin, val.statMods[tag]["Postmultiplier"])
	for key in Definitions.otherStatDictionary.keys() :
		var tag = Definitions.otherStatDictionary[key]
		otherStatObjects[key].setPrebonus(origin, val.otherMods[tag]["Prebonus"])
		otherStatObjects[key].setPremultiplier(origin, val.otherMods[tag]["Premultiplier"])
		otherStatObjects[key].setPostbonus(origin, val.otherMods[tag]["Postbonus"])
		otherStatObjects[key].setPostmultiplier(origin, val.otherMods[tag]["Postmultiplier"])
		var statDisplay = $ScrollContainer/VBoxContainer/OtherStatPanel/PanelContainer/OtherStatDisplay
		
		if (key < Definitions.otherStatEnum.routineSpeed_0) :
			if (otherStatObjects[key].getPrebonuses().get("Humanoid") != null) :
				if (otherStatObjects[key].getFinal() == 1) :
					statDisplay.get_child(key as int).visible = false
				else :
					statDisplay.get_child(key as int).visible = true
			else :
				if (otherStatObjects[key].getFinal() == 0) :
					statDisplay.get_child(key as int).visible = false
				else :
					statDisplay.get_child(key as int).visible = true
				
func getSubclassModPacket() -> ModifierPacket :
	var subclassMod : ModifierPacket = ModifierPacket.new()
	if (mySubclass == -1) :
		return subclassMod
	elif (mySubclass == Definitions.subclass.barb) :
		subclassMod.addMod("stat",Definitions.baseStatEnum.PHYSDEF,"Premultiplier",0.8)
		subclassMod.addMod("stat",Definitions.baseStatEnum.MAGDEF,"Premultiplier", 0.8)
		subclassMod.addMod("stat",Definitions.baseStatEnum.DR,"Premultiplier", 1.25)
	elif (mySubclass == Definitions.subclass.knight) :
		subclassMod.addMod("stat",Definitions.baseStatEnum.DR,"Premultiplier", 0.75)
		var armorVal
		if (equippedArmor == null) :
			armorVal = 0
		else :
			armorVal = (equippedArmor.MAGDEF + equippedArmor.PHYSDEF)/2.0
		subclassMod.addMod("stat",Definitions.baseStatEnum.DR,"Prebonus", 1.9*armorVal)
	elif (mySubclass == Definitions.subclass.ammo) :
		subclassMod.addMod("stat",Definitions.baseStatEnum.MAXHP,"Premultiplier", 0.9)
		if (equippedWeapon != null && equippedWeapon.equipmentGroups.weaponClass != EquipmentGroups.weaponClassEnum.melee) :
			subclassMod.addMod("stat",Definitions.baseStatEnum.AR,"Premultiplier",  1.35)
	elif (mySubclass == Definitions.subclass.whirl) :
		if (equippedWeapon != null && equippedWeapon.equipmentGroups.weaponClass != EquipmentGroups.weaponClassEnum.ranged) :
			subclassMod.addMod("stat",Definitions.baseStatEnum.PHYSDEF,"Premultiplier",  1.08)
			subclassMod.addMod("stat",Definitions.baseStatEnum.MAGDEF,"Premultiplier",  1.08)
	elif (mySubclass == Definitions.subclass.enchant) :
		if (equippedWeapon != null && equippedWeapon.equipmentGroups.isElemental()) :
			subclassMod.addMod("stat",Definitions.baseStatEnum.DR,"Premultiplier",  1.1)
		if (equippedArmor != null && equippedArmor.equipmentGroups.isElemental()) :
			subclassMod.addMod("stat",Definitions.baseStatEnum.PHYSDEF,"Premultiplier",  1.1)
			subclassMod.addMod("stat",Definitions.baseStatEnum.MAGDEF,"Premultiplier",  1.1)
		if (equippedAccessory != null && equippedAccessory.equipmentGroups.isElemental()) :
			subclassMod.addMod("attr",Definitions.attributeEnum.SKI,"Premultiplier",  1.05)
	elif (mySubclass == Definitions.subclass.soul) :
		subclassMod.addMod("stat",Definitions.baseStatEnum.MAXHP,"Premultiplier",  0.85)
		var souls = EnemyDatabase.getSoulCount()
		var val = -4.0*pow(10.0,-5.0)*pow(souls,2.0)+0.0105*souls-0.25
		if (val < 0) :
			val = 0
		subclassMod.addMod("stat",Definitions.baseStatEnum.MAXHP,"Postmultiplier", val)
		subclassMod.addMod("stat",Definitions.baseStatEnum.DR,"Postmultiplier", val)
	return subclassMod
		
func getSubclassName() -> String :
	return Definitions.subclassDictionary[mySubclass]
func getSubclass() :
	return mySubclass

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
var subclassTooltip
func setSubclass(val : Definitions.subclass) :
	var currentSubclass = mySubclass
	if (currentSubclass != -1) :
		updateDirectModifier(Definitions.subclassDictionary[currentSubclass], ModifierPacket.new())
	mySubclass = val
	if (subclassTooltip != null) :
		$ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/ClassLabel.remove_child(subclassTooltip)
		subclassTooltip.queue_free()
		subclassTooltip = null
	if (mySubclass != -1) :
		$ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/ClassLabel.text = "Class: " + Definitions.subclassDictionary[mySubclass]
		var newTooltip = tooltipLoader.instantiate()
		$ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/ClassLabel.add_child(newTooltip)
		newTooltip.setTitle("Subclass: " + Definitions.subclassDictionary[mySubclass])
		newTooltip.setDesc(Definitions.subclassDescriptions[mySubclass])
		var upperLeft = Vector2(0,0)
		## might not work :(
		var bottomRight = $ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/ClassLabel.size
		newTooltip.setPos(upperLeft, bottomRight)
		subclassTooltip = newTooltip
	
########################################
func myUpdate() :
	###################################################
	## Learning Curve
	if (learningCurveEnabled) :
		var learningCurveMod = getLearningCurveMod()
		#var learningCurveMod = ModifierPacket.new()
		updateDirectModifier("Learning Curve", learningCurveMod)
	updateDirectModifier("Softcap", getSoftcapMod())
	####################################################
	## Subclass ##
	if (mySubclass != -1) :
		updateDirectModifier(Definitions.subclassDictionary[mySubclass], getSubclassModPacket())
		
	var capModifier : ModifierPacket = ModifierPacket.new()
	updateDirectModifier("Hard cap", capModifier)
	#capModifier.addMod("otherStat", Definitions.otherStatEnum.routineSpeed, "Premultiplier", 1)
	var changed : bool = false
	for index in range(0,6) :
		var routineSpeed = otherStatObjects[Definitions.otherStatEnum.routineSpeed_0 + index].getFinal()
		if (routineSpeed > 100) :
			capModifier.addMod("otherStat", Definitions.otherStatEnum.routineSpeed_0+index,"Premultiplier",100.0/routineSpeed)
			changed = true
	var magicFind = otherStatObjects[Definitions.otherStatEnum.magicFind].getFinal()
	if (magicFind >= 2.29665) :
		capModifier.addMod("otherStat", Definitions.otherStatEnum.magicFind, "Premultiplier",2.29665/magicFind)
		changed = true
	if changed :
		updateDirectModifier("Hard cap", capModifier)
	## Attributes
	#equippedWeapon
	pass
	#equippedArmor
	pass
	#trainingLevels
	for key in Definitions.attributeDictionary.keys() :
		attributeObjects[key].setPrebonus("CRL * RE", otherStatObjects[Definitions.otherStatEnum.routineEffect].getFinal()*trainingLevels[key])
	#Final
	var finalAttributes : Array[float]
	for key in Definitions.attributeDictionary.keys() :
		finalAttributes.append(attributeObjects[key].getFinal())
	## Combat Stats
	#equippedWeapon
	derivedStatObjects[Definitions.baseStatEnum.DR].setPrebonus("Equipped Weapon", equippedWeapon.attackBonus)
	var LOCAL_weaponArray = equippedWeapon.getScalingArray()
	#equippedArmor
	var PHYSDEF_armor
	var MAGDEF_armor
	if (equippedArmor == null) :
		PHYSDEF_armor = 0
		MAGDEF_armor = 0
	else :
		PHYSDEF_armor = equippedArmor.PHYSDEF
		MAGDEF_armor = equippedArmor.MAGDEF
	derivedStatObjects[Definitions.baseStatEnum.PHYSDEF].setPrebonus("Equipped Armor", PHYSDEF_armor)
	derivedStatObjects[Definitions.baseStatEnum.MAGDEF].setPrebonus("Equipped Armor", MAGDEF_armor)
	#trainingLevels
	pass
	#Attributes
	for key in Definitions.baseStatDictionary.keys() :
		var args
		var shortName = Definitions.baseStatDictionaryShort[key]
		if (key == Definitions.baseStatEnum.DR) :
			args = LOCAL_weaponArray
			args.append_array(finalAttributes)
		elif (key == Definitions.baseStatEnum.AR || key == Definitions.baseStatEnum.PHYSDEF || key == Definitions.baseStatEnum.MAGDEF) :
			args = finalAttributes
		elif (key == Definitions.baseStatEnum.MAXHP) :
			args = finalAttributes[Definitions.attributeEnum.DUR]
			
		var strings = Encyclopedia.getFormula(shortName, Encyclopedia.formulaAction.getString_array, args)
		var values = Encyclopedia.getFormula(shortName, Encyclopedia.formulaAction.getCalculation_array, args)
		for index in range(0,strings.size()) :
			derivedStatObjects[key].setPrebonus(strings[index], values[index])
		core.setStat(key, derivedStatObjects[key].getFinal())
	##Other stats

	##Derived stats
	updateDerivedStats()
	
func setTypicalEnemyDefense(val) :
	typicalEnemyDefense = val
var typicalEnemyDefense : float = 10
func updateDerivedStats() :
	getDerivedStatList().get_node("AttackSpeed").get_node("Number").text = str(Helpers.myRound(10.0/equippedWeapon.basicAttack.getWarmup(),3))
	var averageAttackRating = (derivedStatObjects[Definitions.baseStatEnum.AR].getFinal()*derivedStatObjects[Definitions.baseStatEnum.DR].getFinal()) / typicalEnemyDefense
	getDerivedStatList().get_node("DamagePerHit").get_node("Number").text = Helpers.engineeringRound(averageAttackRating * equippedWeapon.basicAttack.getPower(),3)
	
func getDerivedStatList() :
	return $ScrollContainer/VBoxContainer/DerivedStatPanel/PanelContainer/OtherStatDisplay
	
#########################################
## Setters
#Takes ownership of class struct
var myCharacter : CharacterPacket = null
func getCharacterPacket() :
	return myCharacter
func setFromCharacter(character : CharacterPacket) :
	myCharacter = character
	setClass(character.getClass())
	setName(character.getName())
func setClass(character : CharacterClass) :
	characterClass = character
	myCharacter.setClass(character)
	for key in Definitions.attributeDictionary.keys() :
		attributeObjects[key].setPrebonus("Class", round(character.getBaseAttribute(key)/character.getAttributeScaling(key)*100.0)/100.0)
		attributeObjects[key].setPremultiplier("Class", character.getAttributeScaling(key))
	if (mySubclass == -1) :
		$ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/ClassLabel.text = "Class: " + character.getText()
	unarmedWeapon = SceneLoader.createEquipmentScene("unarmed_" + Definitions.classDictionary[characterClass.classEnum])
	$ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/ClassLabel.visible = true
	$ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/AttributeLabel.visible = true
	$ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/NameLabel.visible = true
func setName(val) :
	characterName = val
	$ScrollContainer/VBoxContainer/AttributePanel/VBoxContainer/NameLabel.text = val
	core.text = val
###############################
## Getters
#func getClassReference() :
	#return characterClass
func getClass() :
	return characterClass.duplicate()
func getCore() :
	return core.duplicate()
func getAttributeMods() -> Array[NumberClass] :
	var retVal : Array[NumberClass] = []
	for attr in attributeObjects :
		var temp : NumberClass = attr.duplicate()
		temp.premultipliers = attr.premultipliers
		temp.postmultipliers = attr.postmultipliers
		temp.prebonuses = attr.prebonuses
		temp.postbonuses = attr.postbonuses
		retVal.append(temp)
	return retVal
func getRoutineSpeedByReference() -> Array[NumberClass] :
	var retVal : Array[NumberClass] = []
	for key in Definitions.otherStatDictionary.keys() :
		if (Definitions.otherStatEnum.routineSpeed_0 <= key && key <= Definitions.otherStatEnum.routineSpeed_4) :
			retVal.append(otherStatObjects[key])
	return retVal
func getRoutineMultiplicityByReference() -> NumberClass :
	return otherStatObjects[Definitions.otherStatEnum.routineMultiplicity]
func getOtherStat(key : Definitions.otherStatEnum) :
	return otherStatObjects[key].getFinal()
func getModifierDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["attribute"] = {}
	retVal["baseStat"] = {}
	retVal["otherStat"] = {}
	for key in Definitions.attributeDictionary.keys() :
		retVal["attribute"][Definitions.attributeDictionary[key]] = attributeObjects[key].getFinal()
	for key in Definitions.baseStatDictionary.keys() :
		retVal["baseStat"][Definitions.baseStatDictionary[key]] = derivedStatObjects[key].getFinal()
	for key in Definitions.otherStatDictionary.keys() :
		retVal["otherStat"][Definitions.otherStatDictionary[key]] = otherStatObjects[key].getFinal()
	return retVal
		
##################################################
## Saving
const myLoadDependencyName = Definitions.loadDependencyName.player
const myLoadDependencies : Array = []
func afterDependencyLoaded(_dependency : Definitions.loadDependencyName) :
	pass
	
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	#tempDict["playerClass"] = characterClass.resource_path
	#tempDict["playerName"] = characterName
	tempDict["typicalEnemyDefense"] = typicalEnemyDefense
	tempDict["subclass"] = mySubclass
	tempDict["learningCurveEnabled"] = learningCurveEnabled
	tempDict["myCharacter"] = myCharacter.getSaveDictionary()
	return tempDict
	
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	Helpers.highVisScroll($ScrollContainer)
	
	initialiseNumberObjects()
	$CustomMouseover.initialise($ScrollContainer/VBoxContainer, $ScrollContainer/VBoxContainer/DerivedStatPanel/StatTitle, $ScrollContainer/VBoxContainer/DerivedStatPanel/PanelContainer/OtherStatDisplay/AttackSpeed/Number, $ScrollContainer/VBoxContainer/DerivedStatPanel/PanelContainer/OtherStatDisplay/DamagePerHit/Number)
	$ScrollContainer/VBoxContainer/CombatStatPanel.initialise(derivedStatObjects)
	$ScrollContainer/VBoxContainer/AttributePanel.initialise(attributeObjects)
	$ScrollContainer/VBoxContainer/OtherStatPanel.initialise(otherStatObjects)
	myReady = true
	emit_signal("myReadySignal")
	
var humanMan = preload("res://Resources/OldActor/Misc/human.tres")
func beforeLoad(newSave : bool) :
	myReady = false
	core = humanMan.duplicate()
	myReady = true
	emit_signal("myReadySignal")
	if (newSave) :
		doneLoading = true
		emit_signal("doneLoadingSignal")
	
func onLoad(loadDict) -> void :	
	myReady = false
	#setClass(load(loadDict["playerClass"]))
	#setName(loadDict["playerName"])
	setFromCharacter(CharacterPacket.createFromSaveDictionary(loadDict["myCharacter"]))
	if (loadDict.get("typicalEnemyDefense") != null) :
		typicalEnemyDefense = loadDict["typicalEnemyDefense"]
	setSubclass(loadDict["subclass"])
	learningCurveEnabled = loadDict["learningCurveEnabled"]
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("doneLoadingSignal")
	
func getSoftcapMod() -> ModifierPacket :
	var retVal = ModifierPacket.new()
	## My original plan for the softcap would change the scaling from exponential to cubic, which I don't really want. I'll take it off for now, hoping that the exponential growth and random items encourage diversification.
	## I could also use df(x)/dx = x/(1+log10(x)) or x/(1+log10(x)^2.
	return retVal
	for key in Definitions.attributeDictionary.keys() :
		var allBonuses = attributeObjects[key].getPrebonusesRaw()
		var bonus = allBonuses.get("CRL * RE")
		if (bonus == null) :
			continue
		var thresholds = floor(log(bonus)/log(10))-1
		var mult
		if (thresholds > 0) :
			mult = pow(0.5,thresholds)
		else :
			mult = 1
		retVal.addMod("otherStat", Definitions.otherStatEnum.routineSpeed_0 + key, "Premultiplier",mult)
	return retVal
		

var learningCurveEnabled : bool = false
func enableLearningCurve() :
	learningCurveEnabled = true
func getLearningCurveMod() -> ModifierPacket :
	if (!learningCurveEnabled) :
		return ModifierPacket.new()
	const minMult = 1
	const maxMult = 3
	const minLevel = 7
	const maxLevel = 106
	var learningCurveMod = ModifierPacket.new()
	var disable = true
	for key in Definitions.attributeDictionary.keys() :
		var currentBonuses = attributeObjects[key].getPrebonuses()
		var attributeMult
		if (currentBonuses.get("CRL * RE") == null) :
			attributeMult = maxMult
			disable = false
		else :
			var currentLevel = currentBonuses["CRL * RE"] + currentBonuses["Class"]
			if (currentLevel <= minLevel) :
				attributeMult = maxMult
				disable = false
			elif (currentLevel >= maxLevel) :
				continue
			else :
				attributeMult = 1 + (maxMult-1)*(1 - (currentLevel-minLevel)/(maxLevel-minLevel))
				disable = false
		learningCurveMod.addMod("otherStat",Definitions.otherStatEnum.routineSpeed_0 + key,"Premultiplier",attributeMult)
	if (disable) :
		learningCurveEnabled = false
	return learningCurveMod
