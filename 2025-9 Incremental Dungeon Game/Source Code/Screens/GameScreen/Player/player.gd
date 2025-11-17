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
	derivedStatObjects[Definitions.baseStatEnum.AR].setPostbonus("Weapon Attack", equippedWeapon.attackBonus)
	var combatStats_attributes_primaryWeaponAttribute = equippedWeapon.primaryAttribute
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
		if (key == Definitions.baseStatEnum.AR) :
			args = finalAttributes[combatStats_attributes_primaryWeaponAttribute]
			var string = Encyclopedia.getFormula(shortName, Encyclopedia.formulaAction.getString_full, null)
			var value = Encyclopedia.getFormula(shortName, Encyclopedia.formulaAction.getCalculation_full, args)
			derivedStatObjects[key].setPrebonus(string, value)
		elif (key == Definitions.baseStatEnum.MAXHP) :
			args = finalAttributes[Definitions.attributeEnum.DUR]
			var string = Encyclopedia.getFormula(shortName, Encyclopedia.formulaAction.getString_full, null)
			var value = Encyclopedia.getFormula(shortName, Encyclopedia.formulaAction.getCalculation_full, args)
			derivedStatObjects[key].setPrebonus(string, value)
		else :
			for subkey in Definitions.attributeDictionary.keys() :
				var temp : Array = []
				temp.append(subkey)
				temp.append(finalAttributes[subkey])
				args = temp
				var string = Encyclopedia.getFormula(shortName, Encyclopedia.formulaAction.getString_partial, subkey)
				var value = Encyclopedia.getFormula(shortName, Encyclopedia.formulaAction.getCalculation_partial, args)
				derivedStatObjects[key].setPrebonus(string, value)
		core.setStat(key, derivedStatObjects[key].getFinal())
#########################################
## Setters
#Takes ownership of class struct
func setClass(character : CharacterClass) :
	characterClass = character
	for key in Definitions.attributeDictionary.keys() :
		attributeObjects[key].setPrebonus("Class", character.getBaseAttribute(key))
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
	return tempDict
	
var myReady : bool = false
func _ready() :
	initialiseNumberObjects()
	$VBoxContainer/CombatStatPanel.initialise(derivedStatObjects)
	$VBoxContainer/AttributePanel.initialise(attributeObjects)
	myReady = true
	
func beforeLoad(_newSave : bool) :
	var temp = load("res://Screens/GameScreen/Tabs/Combat/Actors/human.tres")
	core = temp.duplicate()
	
func onLoad(loadDict) -> void :
	setClass(load(loadDict["playerClass"]))
	setName(loadDict["playerName"])
