extends Resource

class_name EquipmentGroups

@export var isEligible : bool = true
enum qualityEnum {common,uncommon,rare,epic,legendary}
const qualityDictionary = {
	qualityEnum.common : "Common",
	qualityEnum.uncommon : "Uncommon",
	qualityEnum.rare : "Rare",
	qualityEnum.epic : "Epic",
	qualityEnum.legendary : "Legendary"
}
static func getColouredQualityString(inQuality : qualityEnum, lightBackground : bool) :
	return colourText(inQuality, qualityDictionary[inQuality], lightBackground)
static func colourText(inQuality : qualityEnum, myText : String, lightBackground : bool) -> String :
	const colours = ["#202020", "57a968", "#4566c3", "#a149cc", "#d4941c"]
	const colours_alt = ["#e5e5e5", "4c945b", "4d70d1", "a149cc", "#efa71f"]
	var colourStr
	if (lightBackground) :
		colourStr = colours[inQuality]
	else :
		colourStr = colours_alt[inQuality]
	return "[color=" + colourStr + "]" + myText + "[/color]"
@export var quality : qualityEnum = qualityEnum.common
enum technologyEnum {perennial,natural,crude,advanced,superior}
const technologyDictionary = {
	technologyEnum.perennial : "Perennial",
	technologyEnum.natural : "Natural",
	technologyEnum.crude : "Crude",
	technologyEnum.advanced : "Advanced",
	technologyEnum.superior : "Superior"
}
##Natural is stuff made in nature (duh). On average you can expect natural equipment to be worse, but there are exceptions
##such as... well... any dragon body part really. 
## Crude is basically anything made without artificial magic or metallurgy, including leather armor, slings, dragonbone armor, etc.
## Advanced is early middle ages metallurgy and smaller metal weapons, like chainmail, shortswords, etc.
## Superior is anything made with artificial magic, including enchanted equipment (as opposed to equipment made from magic materials, which could be crude),
## as well as large, heavy equipment, top quality steel, and middle to late midievable age weapons. This includes arming swords, rapiers, full plate, greatswords, crossbows, etc.

##Usage : An animal cannot drop anything that isn't natural, an Orc can't drop superior but can drop advanced, a death knight will only drop superior, etc.
@export var technology : technologyEnum = technologyEnum.natural
enum weaponClassEnum {melee, ranged}
@export var weaponClass : weaponClassEnum = -1
enum armorClassEnum {light,medium,heavy}
@export var armorClass : armorClassEnum = -1

## Environment whitelists: If an item has one of these properties it will not show up in the rewards unless
## in a specific environment
@export var isFire : bool = false
@export var isIce : bool = false
@export var isEarth : bool = false
@export var isWater : bool = false
@export var isSignature : bool = false

static func elementMatch(a : EquipmentGroups, b: EquipmentGroups, element : Definitions.elementEnum) -> bool :
	if (element == Definitions.elementEnum.earth) :
		return a.isEarth && b.isEarth
	if (element == Definitions.elementEnum.fire) :
		return a.isFire && b.isFire
	if (element == Definitions.elementEnum.water) :
		return a.isWater && b.isWater
	if (element == Definitions.elementEnum.ice) :
		return a.isIce && b.isIce
	return false

static func getMatchingElementCount(a : EquipmentGroups, b: EquipmentGroups) :
	var matches = 0
	for key in Definitions.elementDictionary.keys() :
		if (elementMatch(a,b,key)) :
			matches += 1
	return matches

func isElemental() -> bool :
	return isIce || isFire || isEarth || isWater

## unofficial enum
func getElements() -> Array[int] :
	var retVal : Array[int] = []
	if (isEarth) :
		retVal.append(0)
	if (isFire) :
		retVal.append(1)
	if (isIce) :
		retVal.append(2)
	if (isWater) :
		retVal.append(3)
	if (retVal.size() == 0) :
		retVal.append(4)
	return retVal
