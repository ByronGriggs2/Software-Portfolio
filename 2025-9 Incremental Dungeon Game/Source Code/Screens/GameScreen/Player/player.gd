extends Node

var core : ActorPreset = null
var characterClass : CharacterClass = null
var characterName : String = "Undefined"
var frameAttributes : Dictionary = {}
var frameStatBonus : Dictionary = {}
var frameWeapon : Weapon = null
var frameArmor : Armor = null

func updateAttributes(newAttributes) :
	for key in Definitions.attributeDictionary.keys() :
		var baseVal = characterClass.getBaseAttribute(key)
		var scaling = characterClass.getAttributeScaling(key)
		frameAttributes[key] = baseVal + scaling * newAttributes[key]
	$VBoxContainer/VBoxContainer/AttributeDisplay.update(frameAttributes.duplicate())

func updateDerivedStats() :
	updateMAXHP()
	updateAR()
	updatePHYSDEF()
	updateMAGDEF()
	for key in frameStatBonus.keys() :
		core.setStat(key, core.getStat(key) + frameStatBonus[key])
	$VBoxContainer/VBoxContainer2/DerivedStatDisplay.update(core.duplicate())

func updateMAXHP() -> void :
	const baseMAXHP = 0
	const DURGrowth = 1
	core.MAXHP = baseMAXHP + DURGrowth * frameAttributes[Definitions.attributeEnum.DUR]
	core.HP = core.MAXHP
	
func updateAR() -> void :
	const baseAR = 0
	const damageRatios : Dictionary = {
		Definitions.attributeEnum.DEX : 0.25,
		Definitions.attributeEnum.INT : 0.25,
		Definitions.attributeEnum.STR : 0.25
	}
	var weaponAttack = frameWeapon.attackBonus
	var scalingType = frameWeapon.primaryAttribute
	core.AR = baseAR + damageRatios[scalingType] * frameAttributes[scalingType] + weaponAttack

func updatePHYSDEF() -> void :
	const basePHYSDEF = 0
	const DURGrowth = 0.1
	const STRGrowth = 0.05
	const DEXGrowth = 0.025
	core.PHYSDEF = basePHYSDEF + DURGrowth * frameAttributes[Definitions.attributeEnum.DUR] + STRGrowth * frameAttributes[Definitions.attributeEnum.STR] + DEXGrowth * frameAttributes[Definitions.attributeEnum.DEX]
	if (frameArmor != null) :
		core.PHYSDEF += frameArmor.PHYSDEF
	
func updateMAGDEF() -> void :
	const baseMAGDEF = 0
	const DURGrowth = 0.1
	const INTGrowth = 0.05
	const DEXGrowth = 0.05
	const STRGrowth = 0.025
	core.MAGDEF = baseMAGDEF + DURGrowth * frameAttributes[Definitions.attributeEnum.DUR] + INTGrowth * frameAttributes[Definitions.attributeEnum.INT] + DEXGrowth * frameAttributes[Definitions.attributeEnum.DEX] + STRGrowth * frameAttributes[Definitions.attributeEnum.STR]
	if (frameArmor != null) :
		core.MAGDEF += frameArmor.MAGDEF

#Takes ownership of class struct
func setClass(character : CharacterClass) :
	characterClass = character
	$VBoxContainer/ClassLabel.text = "Class: " + character.getText()
	Definitions.setPlayerClass(character.classEnum)
	
func getClassReference() :
	return characterClass
	
func setName(val) :
	characterName = val
	$VBoxContainer/NameLabel.text = val
	core.text = val
	
func freshSaveInit(myClass : CharacterClass, myName : String) :
	var temp = load("res://Screens/GameScreen/Tabs/Combat/Actors/human.tres")
	core = temp.duplicate()
	setClass(myClass)
	setName(myName)
	
func updateEquipment(attributeBonus : Dictionary, statBonus : Dictionary) :
	for key in frameAttributes.keys() :
		frameAttributes[key] += attributeBonus[key] * characterClass.getAttributeScaling(key)
	for key in statBonus.keys() :
		frameStatBonus[key] += statBonus[key]
	
func startFrame() :
	for key in Definitions.attributeDictionary.keys() :
		frameAttributes[key] = 0
	for key in Definitions.baseStatDictionary.keys() :
		frameStatBonus[key] = 0

func updateWeapon(weaponSceneRef) :
	if (weaponSceneRef != null) :
		frameWeapon = weaponSceneRef.core.duplicate()
		core.actions = [frameWeapon.basicAttack]
	else :
		frameWeapon = null
func updateArmor(armorSceneRef) :
	if (armorSceneRef != null) :
		frameArmor = armorSceneRef.core.duplicate()
	else :
		frameArmor = null
func getClass() :
	return characterClass.duplicate()
	
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var key : String = "playerClass"
	tempDict[key] = characterClass.resource_path
	key = "playerName"
	tempDict[key] = characterName
	return tempDict
	
var myReady : bool = false
func _ready() :
	myReady = true
	
func beforeLoad(newSave : bool) :
	if (!newSave) :
		var temp = load("res://Screens/GameScreen/Tabs/Combat/Actors/human.tres")
		core = temp.duplicate()
	startFrame()
	
func onLoad(loadDict) -> void :
	setClass(load(loadDict["playerClass"]))
	setName(loadDict["playerName"])
	
func afterLoad() :
	pass
