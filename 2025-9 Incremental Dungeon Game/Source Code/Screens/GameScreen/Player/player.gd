extends Node

var core : ActorPreset
var characterClass : CharacterClass = null
var characterName : String = "Undefined"
var attributes : Array[float]

func _ready() :
	var temp = load("res://Screens/GameScreen/Tabs/Combat/Actors/human.tres")
	core = temp.duplicate()
	for key in Definitions.attributeDictionary.keys() :
		attributes.append(0)

func updateAttributes(newAttributes) :
	for key in Definitions.attributeDictionary.keys() :
		var baseVal = characterClass.getBaseAttribute(key)
		var scaling = characterClass.getAttributeScaling(key)
		attributes[key] = baseVal + scaling * newAttributes[key]
	$VBoxContainer/VBoxContainer/AttributeDisplay.update(attributes.duplicate())

func updateDerivedStats() :
	var oldHP = core.MAXHP
	updateMAXHP()
	var newHP = core.MAXHP
	core.HP += newHP-oldHP
	updatePHYSATK()
	updatePHYSDEF()
	updateMAGATK()
	updateMAGDEF()
	$VBoxContainer/VBoxContainer2/DerivedStatDisplay.update(core.duplicate())

func updateMAXHP() -> void :
	const baseMAXHP = 0
	const DURGrowth = 1
	core.MAXHP = baseMAXHP + DURGrowth * attributes[Definitions.attributeEnum.DUR]
	
func updatePHYSATK() -> void :
	const basePHYSATK = 0
	const STRGrowth = 0.25
	core.PHYSATK = basePHYSATK + STRGrowth * attributes[Definitions.attributeEnum.STR]

func updatePHYSDEF() -> void :
	const basePHYSDEF = 0
	const DURGrowth = 0.1
	const STRGrowth = 0.05
	const SKIGrowth = 0.1
	core.PHYSDEF = basePHYSDEF + DURGrowth * attributes[Definitions.attributeEnum.DUR] + STRGrowth * attributes[Definitions.attributeEnum.STR] + SKIGrowth * attributes[Definitions.attributeEnum.SKI]
	
func updateMAGATK() -> void :
	const baseMAGATK = 0
	const INTGrowth = 0.25
	core.MAGATK = baseMAGATK + INTGrowth * attributes[Definitions.attributeEnum.INT]
	
func updateMAGDEF() -> void :
	const baseMAGDEF = 0
	const DURGrowth = 0.1
	const INTGrowth = 0.05
	const SKIGrowth = 0.1
	core.MAGDEF = baseMAGDEF + DURGrowth * attributes[Definitions.attributeEnum.DUR] + INTGrowth * attributes[Definitions.attributeEnum.INT] + SKIGrowth * attributes[Definitions.attributeEnum.SKI]
	
#Takes ownership of class struct
func setClass(character : CharacterClass) :
	characterClass = character
	$VBoxContainer/ClassLabel.text = "Class: " + character.text
	
func setName(val) :
	characterName = val
	$VBoxContainer/NameLabel.text = val
	
func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	var key : String = "playerClass"
	tempDict[key] = characterClass.resource_path
	key = "playerName"
	tempDict[key] = characterName
	return tempDict
	
func setFromSaveDictionary(loadDict) -> void :
	characterClass = load(loadDict["playerClass"])
	setName(loadDict["playerName"])
	
