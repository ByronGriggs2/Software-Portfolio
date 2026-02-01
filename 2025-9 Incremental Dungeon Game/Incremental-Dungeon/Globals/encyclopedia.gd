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
	tutorialFloorEnd,
	dropIntro,
	manticoreKill,
	herophile,
	row1,
	floor1,
	floor2
	#apophis
}
const tutorialTitles : Dictionary = {
	tutorialName.tutorial : "A Tutorial About Tutorials",
	tutorialName.tutorialFloor1 : "Combat Map 1",
	tutorialName.tutorialFloor2 : "Combat Map 2",
	tutorialName.tutorialFloor3 : "Combat Map 3",
	tutorialName.trainingTab : "Training Tab",
	tutorialName.tutorialFloor4 : "Forks in the Road",
	tutorialName.equipment : "Equipment",
	tutorialName.tutorialFloorEnd : "Floor Exit",
	tutorialName.dropIntro : "Drops Introduction",
	tutorialName.manticoreKill : "Manticore Defeated",
	tutorialName.herophile : "Training with Herophile",
	tutorialName.row1 : "Beastiary and Item Drops",
	tutorialName.floor1 : "Combat Rewards Filter",
	tutorialName.floor2 : "Elements"
	#tutorialName.apophis : "The Demon King is Slain!"
}
const tutorialDesc : Dictionary = {
	tutorialName.tutorial : "Tutorial popups can be disabled in the in-game options (F2) menu. You can find a list of all encountered tutorials, even if they were suppressed, in the Encyclopedia (F1). Remember that tutorials may show up even in the late game!",
	tutorialName.tutorialFloor1 : "Welcome to the Combat Map. Here you can click to select a room to enter next. At the start of each map, only the entrance will be available to explore.",
	#fight rats
	tutorialName.tutorialFloor2 : "Nice! Not every room has a combat encounter, but most will. Try entering the newly discovered room next.",
	#die to hobgoblin
	tutorialName.tutorialFloor3 : "That guy is tough! Oh well, you didn't think this would be easy. Click the training tab to begin increasing your strength.\n\n[i]The Training tab has been unlocked[/i]",
	#click on tab
	tutorialName.trainingTab : "Here you can choose a Routine to increase your Attributes over time. Each Routine improves different Attributes at different rates, and the best Routine will depend on your build.\n\nWhile they may seem slow at first, Routines can be sped up later on. Choose a routine and level it up until you can defeat the Hobgoblin! Remember that combat takes a snapshot of your stats when it starts, so increasing your Attributes won't help until the start of the next combat.",
	#kill hobgoblin
	tutorialName.tutorialFloor4 : "The primary goal of this game is to progress deeper into the dungeon. [b]North[/b] always leads deeper, but your adversaries will get very strong, very fast! You will need to explore side passages to stand a chance against them.",
	#First time rewards screen comes up (presumably before goblins)
	tutorialName.equipment : "You just found some equipment! Up until now you've been unarmed and unarmored. You can add the item to your inventory from the loot screen and equip it in the Equipment Tab. Try equipping it and see how it changes your Combat Stats.\n\n[i]The Equipment tab has been unlocked[/i]",
	#End of first floor
	tutorialName.tutorialFloorEnd : "Congratulations, you've beaten the tutorial floor. The dungeon from here on out will be a bit slower and more challenging.\n\nRemember to explore the side paths and farm drops from weaker monsters. If you feel progress is too slow, get a cup of coffee or pull up a YouTube video. This is not meant to be a terribly active game.\n\nAlternatively, you could spend some time scrutinising your build to try and squeeze a bit more performance out of it!\n\nOh, and take this Routine Speed buff, you're gonna need it.\n\n[i]The \"Learning Curve\" Routine Speed buff has been added[/i]",
	
	tutorialName.manticoreKill : "Congratulations on defeating the Manticore! Unfortunately this is the end of content in the current version. I hope you enjoyed the game, and I'd be happy to hear any feedback you have!",
	tutorialName.herophile : "\"I am no match for you. I yield to your strength.\" Herophile says, kneeling. You turn towards the exit.\n\n\"However\"\n\nYou turn back towards the demigoddess, eyebrow raised.\n\n\"Your skill is clearly lacking.\" she says, grinning. \"I believe a mutually beneficial arrangement can be made.\"\n\n[i]The \"Spar with Herophile\" Routine has been unlocked![/i]",
	#tutorialName.dropIntro : "From floor 1 onwards, enemies have a chance to drop items on death. Once a room has been entered, you can CTRL-click the enemies' names on the Combat Map to view their beastiary entries, which contain hints about what items they can drop. You can also access any of the beastiary entries in the Encyclopedia (F1), but only if the enemy has been defeated at least once.",
	tutorialName.row1 : "From floor 1 onwards, enemies have a chance to drop items on death, but not all enemies drop from the same item pool! You can view the item pool Tags of an enemy on the Combat Map if you CTRL-Click their name.\n\nYou can also view the same entries in the Beastiary, found in the Encyclopedia (F1), provided you've won a battle against them at least once.",
	tutorialName.floor1 : "Tired of getting irrelevant items? You can filter rewards to be auto-discarded in the in-game options menu (F2). There, you can also choose to have your items auto-replaced when an upgrade drops!",
	tutorialName.floor2 : "Have you been taking advantage of elemental synergies? Some equipment has one or more Elements. Although nonelemental equipment is 15% stronger, elemental equipment grants a boost of 25% when the synergy is activated!"
	#tutorialName.apophis : "Congratulations, you've defeated the Demon King! The Surface world is saved! Or something. This game was going to have a more in depth story but game development is hard. In any case, there are actually 20 biomes, 25 bosses, and 10 factions in this game, and levels 1-9 are randomly generated! So I encourage you to check out endless mode or try one of the other classes. Thanks for playing!"
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
	tutorialName.tutorialFloorEnd,
	tutorialName.dropIntro,
	tutorialName.manticoreKill,
	tutorialName.herophile,
	tutorialName.row1,
	tutorialName.floor1,
	tutorialName.floor2
	#tutorialName.apophis
]

## I tried my damndest, but I could not figure out an elegant way to do this. Oh well, it's decent. All formulas in the game that are exposed to the player are in
## this dictionary. Both the strings for the formulas and the calculations are done via getFormula(). It is the responsibility of getFormula()'s caller to know
## what the valid arguments are for the function it's calling, which is the part I hate the most.
## So for example you can call a formula in this dictionary that uses parameters associated with multiple types of enums. No problem, but the caller needs to know
## what order the variables are expected in, and cannot rely on enum order. If it really bothers me I suppose I could use a temporary dictionary as a parameter to
## eliminate all ambiguity.
enum formulaAction {
	getString_full,
	getString_array,
	getString_partial,
	getCalculation_full,
	getCalculation_array,
	getCalculation_partial
}
var formulas : Dictionary = {
	"AR" : {
		"between" : "",
		formulaAction.getString_array : func(_args) : return ["0.6`*SKI"],
		formulaAction.getString_partial : func(_args) : 
			return "0.6`*SKI",
		formulaAction.getCalculation_array : func(args) : #args = attributes
			var retVal : Array[float] = []
			retVal.append(0.6*args[Definitions.attributeEnum.SKI])
			return retVal,
		formulaAction.getCalculation_partial : func(args) : #args = attributes
			return 0.6*args[Definitions.attributeEnum.SKI]},
	"DR" : {
		"between" : " + ",
		formulaAction.getString_array : func(_args) : return ["DEX Scaling`*DEX", "INT Scaling`*INT", "STR Scaling`*STR"],
		formulaAction.getString_partial : func(args) :
			if (args == Definitions.attributeEnum.DEX) :
				return "DEX Scaling`*DEX"
			elif (args == Definitions.attributeEnum.INT) :
				return "INT Scaling`*INT"
			elif (args == Definitions.attributeEnum.STR) :
				return "STR Scaling`*STR"
			else :
				return "",
		formulaAction.getCalculation_array : func(args) : #args[0]-args[N-1] = scalings, args[N]-args[2N-1] = attr
			var retVal : Array[float] = []
			for key in Definitions.attributeDictionary.keys() :
				if (key == Definitions.attributeEnum.STR || key == Definitions.attributeEnum.DEX || key == Definitions.attributeEnum.INT) :
					retVal.append(args[key]*args[Definitions.attributeDictionary.keys().size()+key])
			return retVal,
		formulaAction.getCalculation_partial : func(args) : return args[0]*args[1]}, #args[0] = scaling, args[1] = attr
	"MAXHP" : {
		"between" : "",
		formulaAction.getString_array : func(_args) : return ["10`*DUR"],
		formulaAction.getString_partial : func(_args) : return "10`*DUR",
		formulaAction.getCalculation_array : func(args) : return [10*args],
		formulaAction.getCalculation_partial : func(args) : return 10*args},
	"PHYSDEF" : {
		"between" : " + ",
		formulaAction.getString_array : func(_args) : return ["0.15`*DEX", "0.6`*DUR", "0.3`*STR", "0.6`*SKI"],
		formulaAction.getString_partial : func(args) :
			if (args == Definitions.attributeEnum.DEX) :
				return "0.15`*DEX"
			elif (args == Definitions.attributeEnum.DUR) :
				return "0.6`*DUR"
			elif (args == Definitions.attributeEnum.STR) :
				return "0.3`*STR"
			elif (args == Definitions.attributeEnum.SKI) :
				return "0.6`*SKI"
			else :
				return "",
		formulaAction.getCalculation_array : func(args) : return [0.15*args[Definitions.attributeEnum.DEX], 0.6*args[Definitions.attributeEnum.DUR], 0.3*args[Definitions.attributeEnum.STR], 0.6*args[Definitions.attributeEnum.SKI]],
		formulaAction.getCalculation_partial : func(args) : #[type : Definitions.attributeEnum, val : float]
			if (args[0] == Definitions.attributeEnum.DEX) :
				return 0.15*args[1]
			elif (args[0] == Definitions.attributeEnum.DUR) :
				return 0.6*args[1]
			elif (args[0] == Definitions.attributeEnum.STR) :
				return 0.3*args[1]
			elif (args[0] == Definitions.attributeEnum.SKI) :
				return 0.6*args[1]
			else :
				return 0},
	"MAGDEF" : {
		"between" : " + ",
		formulaAction.getString_array : func(_args) : return ["0.15`*DEX", "0.6`*DUR", "0.3`*INT", "0.6`*SKI"],
		formulaAction.getString_partial : func(args) :
			if (args == Definitions.attributeEnum.DEX) :
				return "0.15`*DEX"
			elif (args == Definitions.attributeEnum.DUR) :
				return "0.6`*DUR"
			elif (args == Definitions.attributeEnum.INT) :
				return "0.3`*INT"
			elif (args == Definitions.attributeEnum.SKI) :
				return "0.6`*SKI"
			else :
				return "",
		formulaAction.getCalculation_array : func(args) : return [0.15*args[Definitions.attributeEnum.DEX], 0.6*args[Definitions.attributeEnum.DUR], 0.3*args[Definitions.attributeEnum.INT], 0.6*args[Definitions.attributeEnum.SKI]],
		formulaAction.getCalculation_partial : func(args) : #[type : Definitions.attributeEnum, val : float]
			if (args[0] == Definitions.attributeEnum.DEX) :
				return 0.15*args[1]
			elif (args[0] == Definitions.attributeEnum.DUR) :
				return 0.6*args[1]
			elif (args[0] == Definitions.attributeEnum.INT) :
				return 0.3*args[1]
			elif (args[0] == Definitions.attributeEnum.SKI) :
				return 0.6*args[1]
			else :
				return 0},
	"Damage" : {
		"between" : "",
		formulaAction.getString_array : func(_args) : return ["Action Power * attacker's DR * (attacker's AR/defender's PHYSDEF or MAGDEF)"],
		formulaAction.getString_partial : func(_args) : return "Action Power * attacker's DR * (attacker's AR/defender's PHYSDEF or MAGDEF)",
		formulaAction.getCalculation_array : func(args) : #args[0] = power, args[1] = DR, args[2] = AR, args[3] = DEF
			return [args[0]*args[1]*args[2]/args[3]],
		formulaAction.getCalculation_partial : func (args) :
			return args[0]*args[1]*args[2]/args[3]}
}
func getFormula(formulaKey : String, action : formulaAction, args) :
	if (action == formulaAction.getString_full) :
		var retVal : String = ""
		var arr = formulas[formulaKey][formulaAction.getString_array].call(args)
		for item in arr :
			retVal += item
			if (item != arr.back()) :
				retVal += formulas[formulaKey]["between"]
		return retVal
	elif (action == formulaAction.getCalculation_full) :
		var retVal : float = 0
		var arr = formulas[formulaKey][formulaAction.getCalculation_array].call(args)
		for item in arr :
			retVal += item
		return retVal
	else :
		return formulas[formulaKey][action].call(args)
## Because I've chosen to use enum keys 
	
	
const problemDictionary = {
	"Damage" : ["Damage Rating", "Magic Damage Taken", "Physical Damage Taken", "Magic Damage Dealt", "Physical Damage Dealt"],
	"Multiplier" : ["Standard Multiplier"],
	"Routine Growth" : ["Routine Growth Ratio"],
	"Routine" : ["Routine Growth", "Cumulative Routine Levels", "Routine Growth Ratio", "Routine Effect", "Routine Speed", "Routine Multiplicity"]
}
const keywords : Array[String] = [
	"Routine Multiplicity",
	"Skill",
	"Routine Speed",
	"Routine Effect",
	"Wait",
	"Inventory Behaviour",
	"Action Power",
	"Always Discard",
	"Always Take",
	"Attack Rating",
	"Attribute",
	"Base",
	"Class",
	"Combat Reward Behaviour",
	"Combat Stat",
	"Currency",
	"Damage Rating",
	"Damage",
	"Dexterity",
	"Durability",
	"Final",
	"Intelligence",
	"Max HP",
	"Magic Defense",
	"Physical Defense",
	"Routine",
	"Scaling",
	"Strength",
	"Weapon",
	"Standard Multiplier",
	"Typical",
	"Bonus",
	"Multiplier",
	"Armor",
	"Accessory",
	"Routine Growth Ratio",
	"Routine Growth",
	"Cumulative Routine Level",
	"Player Panel",
	
	"Physical Damage Dealt",
	"Magic Damage Dealt",
	"Physical Conversion",
	"Magic Conversion",
	"Physical Damage Taken",
	"Magic Damage Taken",
	
	"Tag",
	"Equipment tag",
	"Enemy tag",
	
	"Signature",
	"Boss",
	"Veteran",
	
	"Perennial",
	"Natural",
	"Crude",
	"Advanced",
	"Superior",
	"Dragon's Hoard",
	
	"Melee",
	"Ranged",
	
	"Anti-Magic",
	"Balanced",
	"Anti-Physical",
	
	"Magical",
	"Resistant",
	"Hardened",
	"Ironclad",
	
	"Unequipped",
	"Poorly Equipped",
	"Well Equipped",
	"Elite",
	
	"Vanguard",
	"Backline",
	
	"Element",
	"Fire",
	"Earth",
	"Water",
	"Ice",
	
	"Faction",
	"Demonic Civilian",
	"Demonic Military",
	"Naturekin",
	"Flamekin",
	"Greenskin",
	"Frostkin",
	"Merfolk",
	"Wanderkin",
	"Undead",
	"Nautikin",
	
	"Biome",
	
	"Learning Curve",
	"Magic Find"
	#"Softcap"
	]
const keyword_alternates : Dictionary = {
	"Dexterity" : "DEX",
	"Durability" : "DUR",
	"Intelligence" : "INT",
	"Skill" : "SKI",
	"Strength" : "STR",
	"Max HP" : "MAXHP",
	"Attack Rating" : "AR",
	"Damage Rating" : "DR",
	"Physical Defense" : "PHYSDEF",
	"Magic Defense" : "MAGDEF",
	"Physical Damage Dealt" : "PDD",
	"Physical Damage Taken" : "PDT",
	"Magic Damage Dealt" : "MDD",
	"Magic Damage Taken" : "MDT",
	"Miku Miku Dance" : "MMD",
	"Routine Growth Ratio" : "RGR",
	"Cumulative Routine Level" : "CRL",
	"Routine Effect" : "RE"
}

var descriptions : Dictionary = {
	"Routine Multiplicity" : "Your Routine Multiplicity is the number of Cumulative Routine Levels you gain each time the training bar is filled. The non-integer portion of Routine Multiplicity is the chance of gaining +1 CRL.\n\nFor example, if you have a Routine Multiplicity of 4.3, you will have a 30% chance of gaining 5 CRL and a 70% chance of gaining 4 CRL.\n\t-Base Routine Multiplicity = 1.0.",
	
	"Learning Curve" : "This stuff is easy! As a bright young adventurer, you're still learning quickly. 3.0x Routine Speed for all attributes, linearly decaying to 1.0x as the attribute's Bonus from Class + Cumulative Routine Levels approaches 106.",
	
	#"Softcap" : "Every time the Bonus provided to an attribute by Cumulative Routine Levels reaches a new power of 10 (starting at 100), your Routine Speed for that attribute halves permanently.",
	
	"Class" : "Your Class determines your starting Attributes, Attribute multipliers, and unarmed Weapon Scaling. Fighter is the tankiest, Mage is the highest damage, and Rogue is balanced. You can respec about halfway through the game at the cost of half of your Cumulative Routine Levels",
	
	"Currency" : "Currency items do not take up inventory space and only exist to be exchanged at shops.\n\nThe amount of currency acquired from a victory depends only on how deep in the dungeon it is (except in the case of a particular wandering boss ;3)", 
		
	"Attribute" : "Attributes are a measure of your basic ability, and are shown in the Player Panel.\n\nAttributes are used to calculate your Combat Stats, which are vital for combat. The 5 Attributes are Dexterity, Durability, Intelligence, Skill, and Strength.\n\t-Base Attribute = Class Bonus + Routine Effect * Cumulative Routine Level",
	
	"Dexterity" : "Dexterity is an Attribute measuring your agility, finnesse, and wit.\n\t-When a suitable weapon is equipped, Dexterity provides a Bonus to Base DR of <DR CONTRIBUTION>.\n\t-Dexterity provides a Bonus to Base Physical Defense and Magic Defense of <PHYSDEF CONTRIBUTION> to reflect your skill at dodging.",
	
	"Durability" : "Durability is an Attribute measuring your health and resistance to damage.\n\t-Durability provides a Bonus to Base HP of <MAXHP FORMULA>.\n\t-Durability provides a Bonus to Base Physical Defense and Magic Defense of <PHYSDEF CONTRIBUTION> to reflect your tenacity and grit.",
	
	"Intelligence" : "Intelligence is an Attribute measuring your quick thinking, learning capacity, and accumulated knowledge, and psionic power.\n\t-When a suitable weapon is equipped, Intelligence provides a Bonus to Base DR of <DR CONTRIBUTION>.\n\t-Intelligence provides a Bonus to Base Magic Defense of <MAGDEF CONTRIBUTION> to reflect the fortitude of your psyche.",
	
	"Skill" : "Skill is an Attribute measuring your practice and innate ability in the art of combat.\n\t-Skill provides a Bonus to Base AR of <AR CONTRIBUTION>.\n\t-Skill provides a Bonus to Base Physical Defense and Magic Defense of <PHYSDEF CONTRIBUTION> to reflect your ability to anticipate your opponents' attacks and execute defensive maneuvers",
	
	"Strength" : "Strength is an Attribute measuring your raw physical power.\n\t-When a suitable weapon is equipped, Strength provides a Bonus to Base DR of <DR CONTRIBUTION>.\n\t-Strength provides a Bonus to Base Physical Defense of <PHYSDEF CONTRIBUTION> to reflect your physical health and the hardness of their body.",
	
	"Combat Stat" : "Combat Stats measure a combatant's prowess in certain elements of combat, and yours are displayed in the Player Panel.\n\nWhile enemies typically have preset Combat Stats (scaling with dungeon depth), your stats can be improved in a variety of ways. The five Combat Stats are Max HP, Attack Rating, Damage Rating, Physical Defense, and Magic Defense.",
	
	"Max HP" : "A combatant's Max HP is a Combat Stat measuring their optimal proximity to death. A combatant always starts with HP = Max HP. When their HP hits 0 due to Damage, they die.\n\t-Base Max HP = <MAXHP FORMULA>",
	
	"Physical Defense" : "A combatant's Physical Defense is a Combat Stat that reduces the Damage of incoming damaging physical attacks.\n\t-Base PHYSDEF = <PHYSDEF FORMULA>.",
	
	"Magic Defense" : "A combatant's Magic Defense is a Combat Stat that reduces the Damage of incoming damaging magic attacks.\n\t-Base MAGDEF = <MAGDEF FORMULA>.",
	
	"Weapon" : "Your Weapon determines your DR Attribute Scaling, what your basic attack action is, and provides a Bonus to Base Damage Rating.\n\nYou gain a DR Multiplier of 1.25x for each Element shared by your Weapon and Accessory.",
	
	"Base" : "A Base value is a value before any multipliers are applied. This is an important distinction, because if you've got 10 Base DR and x1000 DR Multiplier, getting another +10 to Base DR will double your damage!\n\nBase values have no direct effect on gameplay (see Final values) though the Final value can be equal to the Base if the product of all multipliers is 1.",
	
	"Bonus" : "Bonus is a term indicating an additive change. It has 2 use cases:\n\t\"Bonus to Base X of Y\" or \n\t\"X Base Bonus +Y\"\n\t-indicates a flat increase Y to the Base of X.\n\t\"Bonus to X Standard Multiplier of Y\" or\n\t\"X Standard Multiplier Bonus +Y\"\n\t-indicates an additive multiplier to X of value Y(see Final)",
	
	"Final" : "A final value is the value after all multipliers have been applied. In practice, the Final value of X is always used in any formula containing X, such as DR in the Damage formula.\n\t-Final = Base * product of Multipliers.\n\t-Or, with the Standard Multiplier factored out,\n\t-Final = Base * (product of non-standard Multipliers) * (1+sum of bonuses to Standard Multiplier)",
	
	"Standard Multiplier" : "The Standard Multiplier for a Final value is a multiplicative multiplier consisting of the sum of all additive multipliers.\n\t-Standard Multiplier = (1+sum of Bonuses to Standard Multiplier).\n\nThe Standard Multiplier is always included in the Final value formula, though in the absence of any Bonuses its value is 1.0x.\n\nAn additive multiplier in math theory is a multiplier that does not multiply the effects of other multipliers (including itself), so 2 additive multipliers of 3x give a total of 6x, not 9x.",
	
	"Multiplier" : "A Multiplier multiplies a Base value to obtain a Final value. All Multipliers stack multiplicatively; additive multipliers are contained in the Standard Multiplier and are referred to as Bonuses.",
	
	"Attack Rating" : "A combatant's Attack Rating is a Combat Stat measuring their accuracy and ability to circumvent their opponent's defenses.\nThe Damage of an attack is proportional to the attacker's Attack Rating.\n\t-Base AR = <AR FORMULA>.",
	
	"Damage Rating" : "A combatant's Damage Rating is a Combat Stat measuring the power of their weapon and how well they can use it. The Damage of an attack is proportional to the attacker's Damage Rating.\n\t-Base DR = <DR FORMULA>.",
	
	"Damage" : "A Damage is damage dealt to a combatant by an attack.\n\t-Base Damage = <DAMAGE VALUE FORMULA>.",
	
	"Action Power" : "The fundamental strength of a combat action, independent of the power of the weapon or its user. Can be seen in the Beastiary (F1) or the Equipment tab for enemies or your weapon, respectively.",

	"Routine" : "Routines passively increase your Cumulative Routine Levels over time, which in turn increases your Attributes. Each Routine improves Cumulative Routine Levels at different rates, and more Routines can be acquired from shops. To view and use Routines, see the Training tab.",
	
	"Cumulative Routine Level" : "There are five Cumulative Routine Levels, one for each Attribute. They can be found under \"Routine Levels\" in the Training tab. Cumulative Routine Levels increase by your Routine Multiplicity (1 for most of the game) every time the corresponding progress bar fills up. CRL fill rates are determined by your Routine Growth rates.\n\t-Cumulative Routine Levels provide a Bonus to the Base of your corresponding Attributes equal to Routine Effect*respective level.",
	
	"Routine Growth": "Routine Growth is the amount of times per second your training bars are filled. For much of the game, this is also the number of Cumulative Routine Levels you gain per second. There are five Routine Growth Rates, one for each Attribute. They can be found at the bottom of the \"Routines\" panel in the Training Tab.\n\t-For a given Attribute, the corresponding Base Routine Growth Rate = RGR*Routine Speed.",
	
	"Routine Speed" : "Listed under \"Other Stats\" in the Player Panel, Routine Speed provides a Multiplier to all Routine Growth of x<Routine Speed>.\n\t-Base Routine Speed = 1.0\n\nRoutine Speed is capped at 100.0.",
	
	"Routine Effect" : "Listed under \"Other Stats\" in the Player Panel. Routine Effect multiplies the Bonus to Base Attributes given by Cumulative Routine Levels.\n\t-Base Routine Effect = 1.0",
	
	"Routine Growth Ratio" : "The Routine Growth Ratio (RGR) is the Numbers displayed underneath each Routine under \"Routines\" in the Training tab. A Routine's RGR provides a Bonus to Base Routine Growth Rate of RGR*Routine Speed. Base RGR varies by Routine, and Bonuses to their Standard Multipliers can be bought in shops.",
	
	"Combat Reward Behaviour" : "Wait\n\nAlways Take\n\nAlways Discard",
	
	"Wait" : "This item must be manually taken or discarded from combat rewards.\nThis setting can be changed at any time per-item in the Encyclopedia (F1).",
	
	"Always Take" : "Always take this item from combat rewards.\nThis setting can be changed at any time per-item in the Encyclopedia (F1).\n\nBehaviour when inventory is full depends on \"Inventory Behaviour\" in the in-game options (F2)",
	
	"Always Discard" : "Always discard this item from combat rewards.\nThis setting can be changed at any time per-item in the Encyclopedia (F1).",
	
	"Inventory Behaviour" : "Determines whether \"Always Take\" will wait or discard when the inventory is full. \"Always Take\" can be set per-item in the Encyclopedia (F1).",
	
	"Scaling" : "Weapons provide a Bonus to Base DR based on your Attributes and your Weapon's Scaling for those respective Attributes. Scaling ranges between E and S+, with S+ being the highest. Hover over the letter in game to see the decimal scaling value.",
	
	"Typical" : "Typical indicates a value is listed without progression scaling. If the value is random, this is the average.",
	
	"Armor" : "A type of equipment. Armor provides a Bonus to Base PHYSDEF and MAGDEF.\n\nYou gain a PHYSDEF and MAGDEF Multiplier of 1.25x for each Element shared by your Armor and Accessory.",
	
	"Accessory" : "A type of equipment. Accessories tend to give Bonuses to Base Attributes and Multipliers to Combat Stats.\n\nYou gain a DR Multiplier of 1.25x for each Element shared by your Weapon and Accessory.\n\nYou gain a PHYSDEF and MAGDEF Multiplier of 1.25x for each Element shared by your Armor and Accessory.",
	
	"Player Panel" : "Located on the left side of the game screen, the Player Panel lists important stats for your character. Hover over the stats to see all of the Bonuses to the Base and Standard Multiplier as well as all non-Standard multipliers.",
	
	"Magic Find" : "Magic Find increases the quantity and quality of item drops from enemies. Currency is unaffected.\n\nWith a Magic Find of 1.0, the drop rarity chances are 60/25/10/3.5/1.5 (common/uncommon/rare/epic/legendary), although bosses have a 15% chance of replacing their first dropped item in a given victory with their signature.\n\nWith a Magic Find of 1.5, 50% more items would drop, and the rarity distribution would instead be 40/37.5/15/5.25/2.25, with a 22.5% signature drop rate from bosses.\n\nMagic Find is capped at 2.297, at which point common items drop 1% of the time.",
	################################################################
	## Tags
	"Tag" : "Tags determine what equipment an enemy can drop. Both equipment and enemies have Tags, and either can disqualify an item from dropping. See Equipment tags and Enemy tags.",
	"Equipment tag" : "The 5 technology Equipment tags are Perennial, Natural, Crude, Advanced, and Superior.\n\nThe 2 offensive (Weapon) Equipment tags are Melee and Ranged.\n\nThe 3 defensive (Armor) Equipment tags are Anti-Magic, Balanced, and Anti-Physical.\n\nItems may also have the standalone Signature Tag.",
	"Enemy tag" : "The 4 technology Enemy tags are Unequipped, Poorly Equipped, Well Equipped, and Elite.\n\nThe 2 offensive (Weapon) Enemy tags are Vanguard and Backline.\n\nThe 4 defensive (Armor) Enemy tags are Magical, Resistant, Hardened, and Ironclad.\n\nEnemies may also have the standalone Boss or Veteran Tags.",

	"Perennial" : "This equipment can be dropped by enemies with any technology Tag (Unequipped, Poorly Equipped, Well Equipped, or Elite)",
	"Natural" : "This equipment can only be dropped by enemies with the Unequipped or Poorly Equipped technology Tags.",
	"Crude" : "This equipment can only be dropped by enemies with the Poorly Equipped technology Tag.",
	"Advanced" : "This equipment can only be dropped by enemies with the Well Equipped technology Tag.",
	"Superior" : "This equipment can only be dropped by enemies with the Well Equipped or Elite technology Tags.",
	
	"Unequipped" : "This enemy can only drop equipment with the Perennial or Natural technology Tags.",
	"Poorly Equipped" : "This enemy can only drop equipment with the Perennial, Natural, or Crude technology Tags.",
	"Well Equipped" : "This enemy can only drop equipment with the Perennial, Advanced, or Superior technology Tags.",
	"Elite" : "This enemy can only drop equipment with the Perennial or Superior technology Tags.",
	"Dragon's Hoard" : "Dragons can drop anything! Ignore all equipment Tags! The Biome still applies though (let's be reasonable here)",
	
	"Melee" : "This Weapon cannot be dropped by enemies with the Backline offense Tag.",
	"Ranged" : "This Weapon cannot be dropped by enemies with the Vanguard offense Tag.",
	
	"Vanguard" : "This enemy cannot drop Weapons with the Ranged offense Tag.",
	"Backline" : "This enemy cannot drop Weapons with the Melee offense Tag.",
	
	"Anti-Magic" : "This Armor provides high Magic Defense and can only drop from enemies with the Magical or Resistant defense Tags.",
	"Balanced" : "This Armor provides balanced Magic Defense and Physical Defense and can only drop from enemies with the Resistant or Hardened defense Tags.",
	"Anti-Physical" : "This Armor provides high Physical Defense and can only drop from enemies with the Hardened or Ironclad defense Tags.",
	
	"Magical" : "This enemy has high Magic Defense and can only drop Armor with the Anti-Magic defense Tag.",
	"Resistant" : "This enemy has fairly high Magic Defense and can only drop Armor with the Anti-Magic or Balanced defense Tags.",
	"Hardened" : "This enemy has fairly high Physical Defense and can only drop Armor with the Anti-Physical or Balanced defense Tags.",
	"Ironclad" : "This enemy has high Physical Defense and can only drop Armor with the Anti-Physical defense Tag.",
	
	"Element" : "Some pieces of equipment have a particular Element. Elemental equipment is typically only dropped in certain Biomes (displayed on the top right corner of the Combat Map), but can provide powerful synergies:\n\nYou gain a DR Multiplier of 1.25x for each Element shared by your Weapon and Accessory.\n\nYou gain a PHYSDEF and MAGDEF Multiplier of 1.25x for each Element shared by your Armor and Accessory.",
	"Fire" : "Fire is the Element of destruction.",
	"Ice" : "Ice is the Element of unchange.",
	"Water" : "Water is the Element of change.",
	"Earth" : "Earth is the Element of creation.",
	
	"Signature" : "This equipment is a Boss' signature drop!",
	"Boss" : "This enemy is the boss of its floor. It drops 225% more items on average and has a powerful signature drop!",
	"Veteran" : "This enemy is quite strong, and has the audacity to not drop any extra items!",
	
	"Faction" : "Each enemy belongs to a single Faction. A given Biome will only contain enemies of permitted factions, displayed on the top right corner of the Combat Map. The 10 Factions are: Flamekin, Frostkin, Naturekin, Nautikin, Demonic Civilian, Demonic Military, Greenskin, Merfolk, Undead, and Wanderkin.",
	"Demonic Civilian" : "The Demonic Civilian Faction includes Demons too weak, dumb, or unruly to be drafted into the Demonic Military.",
	"Demonic Military" : "The Demonic Military Faction consists of cannon fodder, elite shock troops, commanders, war machines, and the Demon King.",
	"Naturekin" : "The Naturekin Faction includes magical beings aligned with earth.",
	"Flamekin" : "The Flamekin Faction includes magical beings aligned with fire.",
	"Frostkin" : "The Frostkin Faction includes magical beings aligned with ice.",
	"Nautikin" : "The Nautikin Faction includes magical beings aligned with water.",
	"Greenskin" : "The Greenskin Faction includes monstrous surface dwelling humanoids, such as orcs and goblins.",
	"Wanderkin" : "The Wanderkin Faction includes hermits, travelers, and malfunctioning automotons that go where they please and have allegiance to no one but themselves. Wanderkin can appear in any biome, with increased rarity",
	"Undead" : "The Undead Faction includes those that practice necromancy and their creations.",
	"Merfolk" : "The Merfolk Faction are a proud and powerful race, unbothered by the affairs of surface or underworld dwellers.",
	
	"Biome" : "Each floor beyond the tutorial is generated with a specific Biome. The Biome determines what Elements are eligible to drop and what Factions are eligible to appear.\n\nThe Biome and its effects can be seen in the top right of the Combat Map.\n\nIf no elements are listed for a biome, equipment from any element can drop, but is 4x as rare (75% chance to be rerolled).",
	##################################################################
	"Physical Damage Dealt" : "Physical Damage Dealt/PDD, applies after Physical Conversion.\n\t-Base PDD = 1.0",
	"Magic Damage Dealt" : "Magic Damage Dealt/MDD, applies after Magic Conversion.\n\t-Base MDD = 1.0",
	"Physical Damage Taken" : "Physical Damage Taken, aka PDT.\n\t-Base PDT = 1.0",
	"Magic Damage Taken" : "Magic Damage Taken, aka MDT.\n\t-Base MDT = 1.0",
	"Physical Conversion" : "Applies a portion of physical damage to the enemy's MAGDEF instead of their PHYSDEF.\n\t-Base Physical Conversion = 0.0",
	"Magic Conversion" : "Applies a portion of magic damage to the enemy's PHYSDEF instead of their MAGDEF.\n\t-Base Magic Conversion = 0.0",
	"Miku Miku Dance" : "henlo"
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
		var DRContribution = getFormula("DR", formulaAction.getString_partial, key)
		descriptions[Definitions.attributeDictionary[key]] = descriptions[Definitions.attributeDictionary[key]].replace("<DR CONTRIBUTION>", DRContribution)
		var PHYSDEFContribution = getFormula("PHYSDEF", formulaAction.getString_partial, key)
		descriptions[Definitions.attributeDictionary[key]] = descriptions[Definitions.attributeDictionary[key]].replace("<PHYSDEF CONTRIBUTION>", PHYSDEFContribution)
		var MAGDEFContribution = getFormula("MAGDEF", formulaAction.getString_partial, key)
		descriptions[Definitions.attributeDictionary[key]] = descriptions[Definitions.attributeDictionary[key]].replace("<MAGDEF CONTRIBUTION>", MAGDEFContribution)
	for key in descriptions.keys() :
		descriptions[key] = descriptions[key].replace("<MAXHP FORMULA>", getFormula("MAXHP", formulaAction.getString_full, null))
		descriptions[key] = descriptions[key].replace("<AR FORMULA>", getFormula("AR", formulaAction.getString_full, null))
		descriptions[key] = descriptions[key].replace("<DR FORMULA>", getFormula("DR", formulaAction.getString_full, null))
		descriptions[key] = descriptions[key].replace("<DAMAGE VALUE FORMULA>", getFormula("Damage", formulaAction.getString_full,null))
		descriptions[key] = descriptions[key].replace("<PHYSDEF FORMULA>", getFormula("PHYSDEF", formulaAction.getString_full, null))
		descriptions[key] = descriptions[key].replace("<MAGDEF FORMULA>", getFormula("MAGDEF", formulaAction.getString_full, null))
