extends Node
func _ready() :
	addFormulasToDescriptions()
	addAlternativeKeywordsToDescriptions()
	
enum tutorialName {
	tutorial,
	tutorialFloor1,
	tutorialFloor2,
	tutorialFloor3,
	trainingTab,
	tutorialFloor4,
	equipment,
	tutorialFloorEnd
}
const tutorialTitles : Dictionary = {
	tutorialName.tutorial : "A Tutorial About Tutorials",
	tutorialName.tutorialFloor1 : "Combat Map 1",
	tutorialName.tutorialFloor2 : "Combat Map 2",
	tutorialName.tutorialFloor3 : "Combat Map 3",
	tutorialName.trainingTab : "Training Tab",
	tutorialName.tutorialFloor4 : "Forks in the Road",
	tutorialName.equipment : "Equipment",
	tutorialName.tutorialFloorEnd : "Floor Exit"
}
const tutorialDesc : Dictionary = {
	tutorialName.tutorial : "Tutorial popups can be disabled in the in-game options menu. There you will also find a list of all encountered tutorials, even if they were suppressed. Remember that tutorials may show up even in the late game!",
	tutorialName.tutorialFloor1 : "Welcome to the Combat Map. Here you can click to select a room to enter next. At the start of each map, only the entrance will be available to explore.",
	#fight rats
	tutorialName.tutorialFloor2 : "Nice! Not every room has a combat encounter, but most will. Try entering the dark corridor next.",
	#die to hobgoblin
	tutorialName.tutorialFloor3 : "That guy is tough! Oh well, you didn't think this would be easy. Click the training tab to begin increasing your strength.\n\n[i]The Training tab has been unlocked[/i]",
	#click on tab
	tutorialName.trainingTab : "Here you can choose a Routine to increase your Attributes over time. Each Routine improves different Attributes at different rates, and the best Routine will depend on your build.\nWhile they may seem slow at first, Routines can be sped up later on. Choose a routine and level it up until you can defeat the Hobgoblin! Remember that combat takes a snapshot of your stats when it starts, so increasing your Attributes won't help until the start of the next combat.",
	#kill hobgoblin
	tutorialName.tutorialFloor4 : "The primary goal of this game is to progress deeper into the dungeon. [b]North[/b] always leads deeper, but your adversaries will get very strong, very fast! You will need to explore side passages to stand a chance against them.",
	#First time rewards screen comes up (presumably before goblins)
	tutorialName.equipment : "You just found some equipment! Up until now you've been unarmed and unarmored. You can add the item to your inventory from the Combat Tab and equip it in the Equipment Tab. Try equipping it and see how it changes your Combat Stats.\n\n[i]The Equipment tab has been unlocked[/i]",
	#End of first floor
	tutorialName.tutorialFloorEnd : "Congratulations, you've beaten the tutorial floor. The dungeon from here on out will be more challenging, less railroaded, and less handholdy. Remember to explore the side paths and farm drops from weaker monsters. If you feel progress is too slow, get a cup of coffee or pull up a YouTube video. This is not meant to be a terribly active game. Alternatively, you could spend some time scrutinising your build to try and squeeze a bit more performance out of it!"
}
const tutorialPointers : Dictionary = {
}
const tutorialPointerPos : Dictionary = {
}
const oneOffTutorials : Array = [
	tutorialName.tutorial,
	tutorialName.tutorialFloor1,
	tutorialName.tutorialFloor2,
	tutorialName.tutorialFloor3,
	tutorialName.tutorialFloor4,
	tutorialName.equipment,
	tutorialName.tutorialFloorEnd
]

## I tried my damndest, but I could not figure out an elegant way to do this. Oh well, it's decent. All formulas in the game that are exposed to the player are in
## this dictionary. Both the strings for the formulas and the calculations are done via getFormula(). It is the responsibility of getFormula()'s caller to know
## what the valid arguments are for the function it's calling, which is the part I hate the most.
## So for example you can call a formula in this dictionary that uses parameters associated with multiple types of enums. No problem, but the caller needs to know
## what order the variables are expected in, and cannot rely on enum order. If it really bothers me I suppose I could use a temporary dictionary as a parameter to
## eliminate all ambiguity.
enum formulaAction {
	getString_full,
	getString_partial,
	getCalculation_full,
	getCalculation_partial
}
var formulas : Dictionary = {
	"AR" : {
		formulaAction.getString_full : func(_args) : return "0.25 * weapon primary Attribute",
		formulaAction.getString_partial : func(args) : return "0.25*" + Definitions.attributeDictionaryShort[args],
		formulaAction.getCalculation_full : func(args) : return 0.25*args,
		formulaAction.getCalculation_partial : func(_args) : return 0},
	"MAXHP" : {
		formulaAction.getString_full : func(_args) : return "1.00*DUR",
		formulaAction.getString_partial : func(_args) : return "",
		formulaAction.getCalculation_full : func(args) : return 1.00*args,
		formulaAction.getCalculation_partial : func(_args) : return 0},
	"PHYSDEF" : {
		formulaAction.getString_full : func(_args) : return "0.025*DEX + 0.1*DUR + 0.05*STR",
		formulaAction.getString_partial : func(args) :
			if (args == Definitions.attributeEnum.DEX) :
				return "0.025*DEX"
			elif (args == Definitions.attributeEnum.DUR) :
				return "0.1*DUR"
			elif (args == Definitions.attributeEnum.STR) :
				return "0.05*STR"
			else :
				return "",
		formulaAction.getCalculation_full : func(args) : return 0.025*args[Definitions.attributeEnum.DEX]+0.1*args[Definitions.attributeEnum.DUR]+0.05*args[Definitions.attributeEnum.STR],
		formulaAction.getCalculation_partial : func(args) : #[type : Definitions.attributeEnum, val : float]
			if (args[0] == Definitions.attributeEnum.DEX) :
				return 0.025*args[1]
			elif (args[0] == Definitions.attributeEnum.DUR) :
				return 0.1*args[1]
			elif (args[0] == Definitions.attributeEnum.STR) :
				return 0.05*args[1]
			else :
				return 0},
	"MAGDEF" : {
		formulaAction.getString_full : func(_args) : return "0.025*DEX + 0.1*DUR + 0.05*INT",
		formulaAction.getString_partial : func(args) :
			if (args == Definitions.attributeEnum.DEX) :
				return "0.025*DEX"
			elif (args == Definitions.attributeEnum.DUR) :
				return "0.1*DUR"
			elif (args == Definitions.attributeEnum.INT) :
				return "0.05*INT"
			else :
				return "",
		formulaAction.getCalculation_full : func(args) : return 0.025*args[Definitions.attributeEnum.DEX]+0.1*args[Definitions.attributeEnum.DUR]+0.05*args[Definitions.attributeEnum.INT],
		formulaAction.getCalculation_partial : func(args) : #[type : Definitions.attributeEnum, val : float]
			if (args[0] == Definitions.attributeEnum.DEX) :
				return 0.025*args[1]
			elif (args[0] == Definitions.attributeEnum.DUR) :
				return 0.1*args[1]
			elif (args[0] == Definitions.attributeEnum.INT) :
				return 0.05*args[1]
			else :
				return 0}
}
func getFormula(formulaKey : String, action : formulaAction, args) :
	return formulas[formulaKey][action].call(args)
## Because I've chosen to use enum keys 
	
const keywords : Array[String] = [
	"Class",
	"Attribute",
	"Dexterity",
	"Durability",
	"Intelligence",
	"Strength",
	"Combat Stat",
	"Max HP",
	"Attack Rating",
	"Physical Defense",
	"Magic Defense",
	"Weapon",
	"Base",
	"Final",
	"Prebonus",
	"Postbonus",
	"Premultiplier",
	"Postmultiplier",
	"Damage",
	"Action Power",
	"Number",
	"Routine",
		]
const keyword_alternates : Dictionary = {
	"Dexterity" : "DEX",
	"Durability" : "DUR",
	"Intelligence" : "INT",
	"Strength" : "STR",
	"Max HP" : "MAXHP",
	"Attack Rating" : "AR",
	"Physical Defense" : "PHYSDEF",
	"Magic Defense" : "MAGDEF"
}

var descriptions : Dictionary = {
	"Class" : "The Player's Class determines their starting Attributes, Attribute Premultipliers, and unarmed Weapon primary Attribute.",
	
	"Attribute" : "Attributes are a measure of the player's basic ability, and are shown in the Player Panel (left).\n\nAttributes are used to calculate their Combat Stats, which are vital for combat. The 4 Attributes are Dexterity, Durability, Intelligence, and Strength.",
	
	"Dexterity" : "Dexterity is an Attribute measuring the Player's skill and finesse, as well as their speed and wit. When a Weapon (including unarmed) is equipped that uses Dexterity as its primary Attribute, the Player gets a Prebonus to AR of <AR CONTRIBUTION>.\n\nAdditionally, Dexterity provides a Prebonus to Physical Defense and Magic Defense to reflect the Player's skill at dodging.",
	
	"Durability" : "Durability is an Attribute measuring the Player's health and resistance to damage. Durability provides a Prebonus to HP of <MAXHP FORMULA>.\n\nAdditionally, Durability provides a Prebonus to Physical Defense and Magic Defense to reflect the Player's tenacity and grit.",
	
	"Intelligence" : "Intelligence is an Attribute measuring the Player's quick thinking, learning capacity, and accumulated knowledge, and psyonic power. When a Weapon (including unarmed) is equipped that uses Intelligence as its primary, Attribute, the Player gets a Prebonus to AR of <AR CONTRIBUTION>.\n\nAdditionally, Intelligence provides a Prebonus to Magic Defense to reflect the fortitude of the Player's psyche.",
	
	"Strength" : "Strength is an Attribute measuring the Player's raw physical power. When a weapon (including unarmed) is equipped that uses Strength as its primary Attribute, the Player gets a Prebonus to AR of <AR CONTRIBUTION>.\n\nAdditionally, Strength provides a Prebonus to Base Physical Defense to reflect the Player's physical health and the hardness of their body.",
	
	"Combat Stat" : "Combat Statistics measure a combatant's prowess in certain elements of combat, and are shown in the Player Panel (left).\n\nWhile enemies typically have preset Combat Stats, the Player's stats can be improved in a variety of ways. Combat Stats can also be modified during combat by certain effects. The Combat Stats are Max HP, Attack Rating, Physical Defense, and Magic Defense.",
	
	"Max HP" : "Max HP is a Combat Statistic. A combatant always starts with HP = Max HP. When their HP hits 0 due to Damage, they die. Base Max HP = <MAXHP FORMULA>",
	
	"Attack Rating" : "A combatant's Attack Rating is a Combat Stat measuring of the power of their offense. Attack Actions deal Damage proportional to AR. Base AR = <AR FORMULA>",
	
	"Physical Defense" : "A combatant's Physical Defense is a Combat Stat that reduces the physical Damage they take.\nPhysical Damage taken = power of damage source * (damage source AR / defender's PHYSDEF). Base PHYSDEF = <PHYSDEF FORMULA>.",
	
	"Magic Defense" : "A combatant's Magic Defense is a Combat Stat that reduces the magic Damage they take.\nMagic Damage taken = power of damage source * (damage source AR / defender's MAGDEF). Base MAGDEF = <MAGDEF FORMULA>.",
	
	"Weapon" : "The Player's Weapon determines what Attribute their Attack Rating scales with, what their basic attack Action is, and provides a Prebonus to their Attack Rating.",
	
	"Number" : "Every number in this game is either a Prebonus, Postbonus, Premultiplier, Postmultiplier, Base value, or Final value. Prebonuses and Premultipliers are used to calculate Base values, and Base values, Postbonuses and Postmultipliers are used to calculate Final values.\n\nAll values are Final values unless otherwise specified. So in other words, you don't need to worry about the distinction unless you want to know how something is calculated!",
	
	"Base" : "A Base value is a Number that is calculated using Prebonuses and Premultipliers, and is used to calculate the Final value.\nBase = (sum of Prebonuses) * (product of Premultipliers).",
	
	"Final" : "A Final value is a Number that is calculated using a Base value, Postbonuses, and Postmultipliers.\nFinal = Base * (sum of Postmultipliers) + sum of Postbonuses. Unless otherwise specified, all Numbers are Final values.",
	
	"Prebonus" : "A Prebonus is an additive Number that is used to calculate a Base value.\nBase value = (sum of Prebonuses) * (product of Premultipliers).",
	
	"Premultiplier" : "A Premultiplier is a multiplicative Number that is used to calculate a Base value.\nBase value = (sum of Prebonuses) * (product of Premultipliers).",
	
	"Postbonus" : "A Postbonus is an additive Number that is used to calculate a Final value.\nFinal value = Base * (sum of Postmultipliers) + (sum of Postbonuses).",
	
	"Postmultiplier" : "A Postmultiplier is a multiplicative Number that is used to calculate a Final value.\nFinal value = Base * (sum of Postmultipliers) + (sum of Postbonuses).",
	
	"Damage" : "Damage dealt by an effect = power of damage source * (AR of damage source / PHYSDEF or MAGDEF of defender). For attack actions, the power of the damage source is the Action Power.",
	
	"Action Power" : "The fundamental strength of a combat action.. Typically used to calculate Damage dealt or healing provided. ",

	"Routine" : "Routines can be used to increase Attributes over time. Each Routine improves Attributes at different rates, and more Routines can be acquired from sources such as Quests. To view and use Routines, see the \"Training\" tab.",
}
#func getEncyclopediaEntries() -> Dictionary :
	#var tempDict : Dictionary = {}
	#for key in keywords :
		#if (keyword_alternates.get(key) != null) :
			#tempDict[keyword_alternates[key]] = descriptions[key]
		#else :
			#tempDict[key] = descriptions[key]
	#return tempDict
func addAlternativeKeywordsToDescriptions() :
	for key in keyword_alternates.keys() :
		if (keyword_alternates[key] is Array) :
			for val in keyword_alternates[key] :
				descriptions[val] = descriptions[key]
		else :
			descriptions[keyword_alternates[key]] = descriptions[key]
func addFormulasToDescriptions() :
	for key in Definitions.attributeDictionary.keys() :
		var ARContribution = getFormula("AR", formulaAction.getString_partial, key)
		descriptions[Definitions.attributeDictionary[key]] = descriptions[Definitions.attributeDictionary[key]].replace("<AR CONTRIBUTION>", ARContribution)
		var PHYSDEFContribution = getFormula("PHYSDEF", formulaAction.getString_partial, key)
		descriptions[Definitions.attributeDictionary[key]] = descriptions[Definitions.attributeDictionary[key]].replace("<PHYSDEF CONTRIBUTION>", PHYSDEFContribution)
		var MAGDEFContribution = getFormula("MAGDEF", formulaAction.getString_partial, key)
		descriptions[Definitions.attributeDictionary[key]] = descriptions[Definitions.attributeDictionary[key]].replace("<MAGDEF CONTRIBUTION>", MAGDEFContribution)
	for key in descriptions.keys() :
		descriptions[key] = descriptions[key].replace("<MAXHP FORMULA>", getFormula("MAXHP", formulaAction.getString_full, null))
		descriptions[key] = descriptions[key].replace("<AR FORMULA>", getFormula("AR", formulaAction.getString_full, null))
		descriptions[key] = descriptions[key].replace("<PHYSDEF FORMULA>", getFormula("PHYSDEF", formulaAction.getString_full, null))
		descriptions[key] = descriptions[key].replace("<MAGDEF FORMULA>", getFormula("MAGDEF", formulaAction.getString_full, null))
