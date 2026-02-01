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
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["attribute"] = attributeMods
	retVal["stat"] = statMods
	retVal["other"] = otherMods
	return retVal
## Do not make this virtual
static func createFromSaveDictionary(loadDict) -> ModifierPacket :
	var retVal = ModifierPacket.new()
	retVal.attributeMods = loadDict["attribute"]
	retVal.statMods = loadDict["stat"]
	retVal.otherMods = loadDict["other"]
	return retVal
static func add(left : ModifierPacket, right : ModifierPacket) -> ModifierPacket :
	return add_array([left, right])
	
static func add_array(val : Array[ModifierPacket]) -> ModifierPacket :
	if (val.size() == 0) :
		return ModifierPacket.new()
	var retVal : ModifierPacket = val[0].duplicate(true)
	for index in range(1, val.size()) :
		add_nonconst(retVal,val[index])
	return retVal
	
static func add_nonconst(left : ModifierPacket, right : ModifierPacket) -> void :
	for key in left.attributeMods.keys() :
		for subkey in StandardModifier.keys() :
			if (subkey == "Premultiplier") :
				left.attributeMods[key][subkey] *= right.attributeMods[key][subkey]
			else :
				left.attributeMods[key][subkey] += right.attributeMods[key][subkey]
	for key in left.statMods.keys() :
		for subkey in StandardModifier.keys() :
			if (subkey == "Premultiplier") :
				left.statMods[key][subkey] *= right.statMods[key][subkey]
			else :
				left.statMods[key][subkey] += right.statMods[key][subkey]
	for key in left.otherMods.keys() :
		for subkey in StandardModifier.keys() :
			if (subkey == "Premultiplier") :
				left.otherMods[key][subkey] *= right.otherMods[key][subkey]
			else :
				left.otherMods[key][subkey] += right.otherMods[key][subkey]

func getModStringOrNull_attr(attribute : Definitions.attributeEnum, tag : String) :
	return internalGetStrOrNull(internalCallType.attribute, tag, Definitions.attributeDictionary[attribute], Definitions.attributeDictionaryShort[attribute])
func getModStringOrNull_combatStat(combatStat : Definitions.baseStatEnum, tag : String) :
	return internalGetStrOrNull(internalCallType.stat, tag, Definitions.baseStatDictionary[combatStat], Definitions.baseStatDictionaryShort[combatStat])
func getModStringOrNull_otherStat(otherStat : Definitions.otherStatEnum, tag : String) :
	var retVal = internalGetStrOrNull(internalCallType.other, tag, Definitions.otherStatDictionary[otherStat],Definitions.otherStatDictionary[otherStat])
	if (retVal == null) :
		return retVal
	elif (otherStat == Definitions.otherStatEnum.physicalDamageTaken || otherStat == Definitions.otherStatEnum.magicDamageTaken) :
		if (retVal.find("red") != -1) :
			retVal = retVal.replace("red","green")
		else :
			retVal = retVal.replace("green","red")
	return retVal
	
enum internalCallType{attribute, stat, other}
func internalGetStrOrNull(type : internalCallType, tag : String, key : String,otherKey : String) :
	var val
	if (type == internalCallType.attribute) :
		val = attributeMods.get(key)
	elif (type == internalCallType.stat) :
		val = statMods.get(key)
	elif (type == internalCallType.other) :
		val = otherMods.get(key)
	else :
		return null
	if (val == null) :
		return null
	var callKey = otherKey
	if (callKey != "") :
		callKey += " "
	return getStrOrNull_static(tag, val[tag], callKey, true)
	
static func getStrOrNull_static(tag : String, val : float, prefix : String, specifyType : bool) :
	var tempStr : String = ""
	var symbol
	if (tag == "Premultiplier") :
		symbol = "x"
	elif (val < 0) :
		symbol = ""
	else :
		symbol = "+"
	var typeString : String
	if (specifyType) :
		if (tag == "Premultiplier") :
			typeString = "Multiplier "
		elif (tag == "Prebonus") :
			typeString = "Base Bonus "
		elif (tag == "Postmultiplier") :
			typeString = "Standard Multiplier Bonus "
	else :
		typeString = ""
	tempStr += prefix + typeString
	if (tag == "Premultiplier") :
		if (val == 1.0) :
			return null
		elif (val < 1.0) :
			tempStr += "[color=red]"
		else :
			tempStr += "[color=green]"
	else :
		if (val == 0.0) :
			return null
		elif (val < 0.0) :
			tempStr += "[color=red]"
		else :
			tempStr += "[color=green]"
	tempStr += symbol + Helpers.engineeringRound(val,3)
	tempStr += "[/color]"
	return tempStr
		
func addMod(statType, statIndex, modType, val : float) :
	var parsedStatType = parseStatType(statType)
	var parsedStatIndex = parseStatIndex(parsedStatType, statIndex)
	var parsedModType = parseModType(modType)
	var oldVal = getMod(statType, statIndex, modType)
	var newVal
	if (parsedModType == "Premultiplier") :
		newVal = oldVal * val
	else :
		newVal = oldVal + val
	if (parsedStatType == "attr") :
		attributeMods[parsedStatIndex][parsedModType] = newVal
	elif (parsedStatType == "stat") :
		statMods[parsedStatIndex][parsedModType] = newVal 
	elif (parsedStatType == "otherStat") :
		otherMods[parsedStatIndex][parsedModType] = newVal
		
func getMod(statType, statIndex, modType) -> float :
	var parsedStatType = parseStatType(statType)
	var parsedStatIndex = parseStatIndex(parsedStatType, statIndex)
	var parsedModType = parseModType(modType)
	if (parsedStatType == "attr") :
		return attributeMods[parsedStatIndex][parsedModType]
	elif (parsedStatType == "stat") :
		return statMods[parsedStatIndex][parsedModType]
	elif (parsedStatType == "otherStat") :
		return otherMods[parsedStatIndex][parsedModType]
	else :
		return 0
	
func parseStatType(statType) -> String :
	var parsedStatType
	if (statType == "attribute") :
		parsedStatType = "attr"
	else :
		parsedStatType = statType
	return parsedStatType
func parseStatIndex(type : String, val) -> String :
	if (val is String) :
		return val
	else :
		if (type == "attr") :
			return Definitions.attributeDictionary[val]
		elif (type == "stat") :
			return Definitions.baseStatDictionary[val]
		elif (type == "otherStat") :
			return Definitions.otherStatDictionary[val]
		else :
			return ""
			
func parseModType(val) -> String :
	if (val is String) :
		return val
	elif (val == 0) :
		return "Prebonus"
	elif (val == 1) :
		return "Premultiplier"
	elif (val == 2) :
		return "Postbonus"
	elif (val == 3) :
		return "Postmultiplier"
	else :
		return ""
