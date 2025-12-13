extends Resource

class_name EquipmentGroups

@export var isEligible : bool = true
enum qualityEnum {common,uncommon,rare,epic,legendary}
const qualityDictionary = {
	qualityEnum.common : "common",
	qualityEnum.uncommon : "uncommon",
	qualityEnum.rare : "rare",
	qualityEnum.epic : "epic",
	qualityEnum.legendary : "legendary"
}
@export var quality : qualityEnum = qualityEnum.common
enum technologyEnum {natural,crude,advanced,superior}
##Natural is stuff made in nature (duh). On average you can expect natural equipment to be worse, but there are exceptions
##such as... well... any dragon body part really. 
## Crude is basically anything made without artificial magic or metallurgy, including leather armor, slings, dragonbone armor, etc.
## Advanced is early middle ages metallurgy and smaller metal weapons, like chainmail, shortswords, etc.
## Superior is anything made with artificial magic, including enchanted equipment (as opposed to equipment made from magic materials, which could be crude),
## as well as large, heavy equipment, top quality steel, and middle to late midievable age weapons. This includes arming swords, rapiers, full plate, greatswords, crossbows, etc.

##Usage : An animal cannot drop anything that isn't natural, an Orc can't drop superior but can drop advanced, a death knight will only drop superior, etc.
@export var technology : technologyEnum = technologyEnum.natural

enum weaponClassEnum {melee, ranged, na}
@export var weaponClass : weaponClassEnum = weaponClassEnum.na
enum armorClassEnum {light,medium,heavy,na}
@export var armorClass : armorClassEnum = armorClassEnum.na

## Environment whitelists: If an item has one of these properties it will not show up in the rewards unless
## in a specific environment
@export var isFire : bool = false
@export var isIce : bool = false
@export var isEarth : bool = false
@export var isWater : bool = false
@export var isMagical : bool = false
