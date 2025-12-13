extends Resource

class_name ModifierPacket
const StandardModifier : Dictionary = {
	"Prebonus" : 0 as float,
	"Premultiplier" : 1 as float,
	"Postbonus" : 0 as float,
	"Postmultiplier" : 0 as float
}
@export var attributeMods : Dictionary = {}
@export var statMods : Dictionary = {}
@export var otherMods : Dictionary = {}
func _init() :
	for key in Definitions.attributeDictionary.keys() :
		attributeMods[Definitions.attributeDictionary[key]] = StandardModifier.duplicate()
	for key in Definitions.baseStatDictionary.keys() :
		statMods[Definitions.baseStatDictionary[key]] = StandardModifier.duplicate()
	for key in Definitions.otherStatDictionary.keys() :
		otherMods[Definitions.otherStatDictionary[key]] = StandardModifier.duplicate()
static func add(left : ModifierPacket, right : ModifierPacket) -> ModifierPacket :
	var newPacket = ModifierPacket.new()
	for key in left.attributeMods.keys() :
		for subkey in StandardModifier.keys() :
			if (subkey == "Premultiplier") :
				newPacket.attributeMods[key][subkey] = left.attributeMods[key][subkey] * right.attributeMods[key][subkey]
			else :
				newPacket.attributeMods[key][subkey] = left.attributeMods[key][subkey] + right.attributeMods[key][subkey]
	for key in left.statMods.keys() :
		for subkey in StandardModifier.keys() :
			if (subkey == "Premultiplier") :
				newPacket.statMods[key][subkey] = left.statMods[key][subkey]*right.statMods[key][subkey]
			else :
				newPacket.statMods[key][subkey] = left.statMods[key][subkey] + right.statMods[key][subkey]
	for key in left.otherMods.keys() :
		for subkey in StandardModifier.keys() :
			if (subkey == "Premultiplier") :
				newPacket.otherMods[key][subkey] = left.otherMods[key][subkey]*right.otherMods[key][subkey]
			else :
				newPacket.otherMods[key][subkey] = left.otherMods[key][subkey] + right.otherMods[key][subkey]
	return newPacket

func getModStringOrNull_attr(attribute : Definitions.attributeEnum, tag : String) :
	return internalGetStrOrNull(internalCallType.attribute, tag, Definitions.attributeDictionary[attribute])
func getModStringOrNull_combatStat(combatStat : Definitions.baseStatEnum, tag : String) :
	return internalGetStrOrNull(internalCallType.stat, tag, Definitions.baseStatDictionary[combatStat])
func getModStringOrNull_otherStat(otherStat : Definitions.otherStatEnum, tag : String) :
	return internalGetStrOrNull(internalCallType.other, tag, Definitions.otherStatDictionary[otherStat])
	
enum internalCallType{attribute, stat, other}
func internalGetStrOrNull(type : internalCallType, tag : String, key : String,) :
	var val : float
	if (type == internalCallType.attribute) :
		val = attributeMods[key][tag]
	elif (type == internalCallType.stat) :
		val = statMods[key][tag]
	elif (type == internalCallType.other) :
		val = otherMods[key][tag]
	else :
		return null
	if (key != "") :
		key += " "
	return getStrOrNull_static(tag, val, key, true)
	
static func getStrOrNull_static(tag : String, val : float, prefix : String, specifyType : bool) :
	var tempStr : String = ""
	var symbol
	if (tag == "Premultiplier") :
		symbol = "x"
	else :
		symbol = "+"
	var typeString : String
	if (specifyType) :
		typeString = tag + " "
	else :
		typeString = ""
	if (tag == "Premultiplier") :
		if (val == 1.0) :
			return null
		elif (val < 1.0) :
			tempStr += "[color=red]"
		else :
			tempStr += "[color=green]"
		tempStr += prefix + typeString + "x" + str(Helpers.myRound(val,3))
		tempStr += "[/color]"
	else :
		if (val == 0.0) :
			return null
		elif (val < 0.0) :
			tempStr += "[color=red]"
		else :
			tempStr += "[color=green]"
		tempStr += prefix + typeString + symbol + str(Helpers.myRound(val,3))
		tempStr += "[/color]"
	return tempStr
