extends Node
const currentVersion : String = "V0.2 release"

enum baseStatEnum {
	MAXHP,HP,PHYSATK,PHYSDEF,MAGATK,MAGDEF
}
const baseStatDictionary = {
	baseStatEnum.HP : "Current HP",
	baseStatEnum.MAXHP : "Max HP",
	baseStatEnum.PHYSATK : "Physical Attack",
	baseStatEnum.PHYSDEF : "Physical Defense",
	baseStatEnum.MAGATK : "Magic Attack",
	baseStatEnum.MAGDEF : "Magic Defense"
}
enum attributeEnum {
	DUR,INT,SKI,STR
}
const attributeDictionary = {
	attributeEnum.DUR : "Durability",
	attributeEnum.INT : "Intelligence",
	attributeEnum.SKI : "Skill",
	attributeEnum.STR : "Strength"
}
enum classEnum {
	FIGHTER,MAGE,ROGUE
}
const classDictionary = {
	classEnum.FIGHTER : "Fighter",
	classEnum.MAGE : "Mage",
	classEnum.ROGUE : "Rogue"
}
