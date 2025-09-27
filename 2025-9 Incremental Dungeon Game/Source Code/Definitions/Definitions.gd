extends Node
const baseStatDictionary = {
	"HP" : "Current HP",
	"MAXHP" : "Max HP",
	"PHYSATK" : "Physical Attack",
	"PHYSDEF" : "Physical Defense",
	"MAGATK": "Magic Attack",
	"MAGDEF" : "Magic Defense"
}
enum attributeEnum {
	NUL,DUR,SKI,STR,INT
}
const attributeDictionary = {
	attributeEnum.NUL : "NullAttribute",
	attributeEnum.DUR : "Durability",
	attributeEnum.SKI : "Skill",
	attributeEnum.STR : "Strength",
	attributeEnum.INT : "Intelligence"
}

enum classEnum {
	FIGHTER,MAGE,ROGUE
}
const classDictionary = {
	classEnum.FIGHTER : "Fighter",
	classEnum.MAGE : "Mage",
	classEnum.ROGUE : "Rogue"
}
