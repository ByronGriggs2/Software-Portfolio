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
## Direct Modifiers (not explicitly ackgnowledged in Player.gd)
#Equipment
## Specific Modifiers
var unarmedWeapon : Node = null

var equippedWeapon : Weapon = null
var equippedArmor : Armor = null
var trainingLevels : Array[int] = []

###############################
## Initialisation
func initialiseNumberObjects() :
	for key in Definitions.attributeDictionary.keys() :
		attributeObjects.append(NumberClass.new())
	for key in Definitions.baseStatDictionary.keys() :
		derivedStatObjects.append(NumberClass.new())
	for key in Definitions.otherStatDictionary.keys() :
		otherStatObjects.append(NumberClass.new())
	otherStatObjects[Definitions.otherStatEnum.magicFind].setPrebonus("Base", 1.0)
###############################
## Specific Modifiers
func updateTrainingLevels(newLevels : Array[int]) :
	trainingLevels = newLevels
func updateWeapon(val : Weapon) :
	if (val == null) :
		equippedWeapon = unarmedWeapon.core
	else :
		equippedWeapon = val
	core.actions[0] = equippedWeapon.basicAttack
func updateArmor(val : Armor) :
	equippedArmor = val
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
########################################
func myUpdate() :
	## Attributes
	#equippedWeapon
	pass
	#equippedArmor
	pass
	#trainingLevels
	for key in Definitions.attributeDictionary.keys() :
		attributeObjects[key].setPrebonus("Training", trainingLevels[key])
	#Final
	var finalAttributes : Array[float]
	for key in Definitions.attributeDictionary.keys() :
		finalAttributes.append(attributeObjects[key].getFinal())
	## Combat Stats
	#equippedWeapon
	derivedStatObjects[Definitions.baseStatEnum.DR].setPostbonus("Weapon Attack", equippedWeapon.attackBonus)
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
	derivedStatObjects[Definitions.baseStatEnum.PHYSDEF].setPostbonus("Equipped Armor", PHYSDEF_armor)
	derivedStatObjects[Definitions.baseStatEnum.MAGDEF].setPostbonus("Equipped Armor", MAGDEF_armor)
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
	pass
	##Derived stats
	updateDerivedStats()
	
func setTypicalEnemyDefense(val) :
	typicalEnemyDefense = val
var typicalEnemyDefense : float = 10
func updateDerivedStats() :
	getDerivedStatList().get_node("AttackSpeed").get_node("Number").text = str(Helpers.myRound(10.0/equippedWeapon.basicAttack.getWarmup(),3))
	var averageAttackRating = (derivedStatObjects[Definitions.baseStatEnum.AR].getFinal()*derivedStatObjects[Definitions.baseStatEnum.DR].getFinal()) / typicalEnemyDefense
	getDerivedStatList().get_node("DamagePerHit").get_node("Number").text = str(Helpers.myRound(averageAttackRating * equippedWeapon.basicAttack.getPower(),3))
	
func getDerivedStatList() :
	return $VBoxContainer/DerivedStatPanel/PanelContainer/OtherStatDisplay
	
#########################################
## Setters
#Takes ownership of class struct
func setClass(character : CharacterClass) :
	characterClass = character
	for key in Definitions.attributeDictionary.keys() :
		attributeObjects[key].setPrebonus("Class", round(character.getBaseAttribute(key)/character.getAttributeScaling(key)*100.0)/100.0)
		attributeObjects[key].setPremultiplier("Class", character.getAttributeScaling(key))
	$VBoxContainer/AttributePanel/VBoxContainer/ClassLabel.text = "Class: " + character.getText()
	unarmedWeapon = SceneLoader.createEquipmentScene("unarmed_" + Definitions.classDictionary[characterClass.classEnum])
func setName(val) :
	characterName = val
	$VBoxContainer/NameLabel.text = val
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
func getOtherStat(key : Definitions.otherStatEnum) :
	return otherStatObjects[key].getFinal()
##################################################
## Saving
const myLoadDependencyName = Definitions.loadDependencyName.player
const myLoadDependencies : Array = []
func afterDependencyLoaded(_dependency : Definitions.loadDependencyName) :
	pass
	
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var key : String = "playerClass"
	tempDict[key] = characterClass.resource_path
	key = "playerName"
	tempDict[key] = characterName
	tempDict["typicalEnemyDefense"] = typicalEnemyDefense
	return tempDict
	
var myReady : bool = false
func _ready() :
	initialiseNumberObjects()
	$CustomMouseover.initialise($VBoxContainer, $VBoxContainer/DerivedStatPanel/StatTitle, $VBoxContainer/DerivedStatPanel/PanelContainer/OtherStatDisplay/AttackSpeed/Number, $VBoxContainer/DerivedStatPanel/PanelContainer/OtherStatDisplay/DamagePerHit/Number)
	$VBoxContainer/CombatStatPanel.initialise(derivedStatObjects)
	$VBoxContainer/AttributePanel.initialise(attributeObjects)
	$VBoxContainer/OtherStatPanel.initialise(otherStatObjects)
	myReady = true
	
func beforeLoad(_newSave : bool) :
	var temp = load("res://Screens/GameScreen/Tabs/Combat/Actors/human.tres")
	core = temp.duplicate()
	
func onLoad(loadDict) -> void :	
	setClass(load(loadDict["playerClass"]))
	setName(loadDict["playerName"])
	if (loadDict.get(typicalEnemyDefense) != null) :
		typicalEnemyDefense = loadDict["typicalEnemyDefense"]
