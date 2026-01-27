extends Node

const Environment_FilesDictionary = {
	"chaos" : preload("res://Resources/Environment/Files/chaos.tres"),
	"demonic_city" : preload("res://Resources/Environment/Files/demonic_city.tres"),
	"desert" : preload("res://Resources/Environment/Files/desert.tres"),
	"element_earth" : preload("res://Resources/Environment/Files/element_earth.tres"),
	"element_fire" : preload("res://Resources/Environment/Files/element_fire.tres"),
	"element_fire_earth" : preload("res://Resources/Environment/Files/element_fire_earth.tres"),
	"element_fire_ice" : preload("res://Resources/Environment/Files/element_fire_ice.tres"),
	"element_ice" : preload("res://Resources/Environment/Files/element_ice.tres"),
	"element_ice_earth" : preload("res://Resources/Environment/Files/element_ice_earth.tres"),
	"element_water" : preload("res://Resources/Environment/Files/element_water.tres"),
	"fort_demon" : preload("res://Resources/Environment/Files/fort_demon.tres"),
	"fort_greenskin" : preload("res://Resources/Environment/Files/fort_greenskin.tres"),
	"fort_merfolk" : preload("res://Resources/Environment/Files/fort_merfolk.tres"),
	"fort_undead" : preload("res://Resources/Environment/Files/fort_undead.tres"),
	"hell" : preload("res://Resources/Environment/Files/hell.tres"),
	"hellforge" : preload("res://Resources/Environment/Files/hellforge.tres"),
	"pit_of_depravity" : preload("res://Resources/Environment/Files/pit_of_depravity.tres"),
	"reef_community" : preload("res://Resources/Environment/Files/reef_community.tres"),
	"unholy_alliance" : preload("res://Resources/Environment/Files/unholy_alliance.tres"),
	"unlikely_alliance" : preload("res://Resources/Environment/Files/unlikely_alliance.tres")}


func getEnvironment_Files(resourceName : String) :
	return Environment_FilesDictionary.get(resourceName)

func getEnvironment(resourceName : String) :
	if (Environment_FilesDictionary.has(resourceName)) :
		return Environment_FilesDictionary[resourceName]
	return null

func getEnvironmentDictionary(type : String) :
	if type == "Environment_Files" : 
		return Environment_FilesDictionary

func getAllEnvironment() -> Array :
	var retVal : Array = []
	retVal.append_array(Environment_FilesDictionary.values())
	return retVal

################################################################################
const NewAction_AttacksDictionary = {
	"axe_1h" : preload("res://Resources/NewAction/Attacks/axe_1h.tres"),
	"axe_2h" : preload("res://Resources/NewAction/Attacks/axe_2h.tres"),
	"blunderbuss" : preload("res://Resources/NewAction/Attacks/blunderbuss.tres"),
	"blur" : preload("res://Resources/NewAction/Attacks/blur.tres"),
	"bow" : preload("res://Resources/NewAction/Attacks/bow.tres"),
	"club_1h" : preload("res://Resources/NewAction/Attacks/club_1h.tres"),
	"crack" : preload("res://Resources/NewAction/Attacks/crack.tres"),
	"crossbow" : preload("res://Resources/NewAction/Attacks/crossbow.tres"),
	"curse" : preload("res://Resources/NewAction/Attacks/curse.tres"),
	"dagger" : preload("res://Resources/NewAction/Attacks/dagger.tres"),
	"disintigrate" : preload("res://Resources/NewAction/Attacks/disintigrate.tres"),
	"flame_whip" : preload("res://Resources/NewAction/Attacks/flame_whip.tres"),
	"flurry" : preload("res://Resources/NewAction/Attacks/flurry.tres"),
	"freezeflame_double_slice" : preload("res://Resources/NewAction/Attacks/freezeflame_double_slice.tres"),
	"frost_slash" : preload("res://Resources/NewAction/Attacks/frost_slash.tres"),
	"generic_brutal" : preload("res://Resources/NewAction/Attacks/generic_brutal.tres"),
	"icicle_spike" : preload("res://Resources/NewAction/Attacks/icicle_spike.tres"),
	"katana_attack" : preload("res://Resources/NewAction/Attacks/katana_attack.tres"),
	"kinetic_blast" : preload("res://Resources/NewAction/Attacks/kinetic_blast.tres"),
	"lance" : preload("res://Resources/NewAction/Attacks/lance.tres"),
	"laser" : preload("res://Resources/NewAction/Attacks/laser.tres"),
	"lightning" : preload("res://Resources/NewAction/Attacks/lightning.tres"),
	"lucky_star" : preload("res://Resources/NewAction/Attacks/lucky_star.tres"),
	"mace_1h" : preload("res://Resources/NewAction/Attacks/mace_1h.tres"),
	"mace_2h" : preload("res://Resources/NewAction/Attacks/mace_2h.tres"),
	"magic_lance" : preload("res://Resources/NewAction/Attacks/magic_lance.tres"),
	"magic_repeater" : preload("res://Resources/NewAction/Attacks/magic_repeater.tres"),
	"magic_slashing_sword_2h" : preload("res://Resources/NewAction/Attacks/magic_slashing_sword_2h.tres"),
	"magic_split_man" : preload("res://Resources/NewAction/Attacks/magic_split_man.tres"),
	"scythe" : preload("res://Resources/NewAction/Attacks/scythe.tres"),
	"shortbow" : preload("res://Resources/NewAction/Attacks/shortbow.tres"),
	"slashing_sword_1h" : preload("res://Resources/NewAction/Attacks/slashing_sword_1h.tres"),
	"slashing_sword_2h" : preload("res://Resources/NewAction/Attacks/slashing_sword_2h.tres"),
	"sling" : preload("res://Resources/NewAction/Attacks/sling.tres"),
	"spear_1h" : preload("res://Resources/NewAction/Attacks/spear_1h.tres"),
	"spear_2h" : preload("res://Resources/NewAction/Attacks/spear_2h.tres"),
	"spear_darkness" : preload("res://Resources/NewAction/Attacks/spear_darkness.tres"),
	"split_man" : preload("res://Resources/NewAction/Attacks/split_man.tres"),
	"thresh" : preload("res://Resources/NewAction/Attacks/thresh.tres"),
	"throw_axe" : preload("res://Resources/NewAction/Attacks/throw_axe.tres"),
	"throw_boulder" : preload("res://Resources/NewAction/Attacks/throw_boulder.tres"),
	"throw_kunai" : preload("res://Resources/NewAction/Attacks/throw_kunai.tres"),
	"thrust_sword_1h" : preload("res://Resources/NewAction/Attacks/thrust_sword_1h.tres"),
	"thrust_sword_2h" : preload("res://Resources/NewAction/Attacks/thrust_sword_2h.tres"),
	"water_jet" : preload("res://Resources/NewAction/Attacks/water_jet.tres"),
	"water_jet_2" : preload("res://Resources/NewAction/Attacks/water_jet_2.tres"),
	"water_jet_3" : preload("res://Resources/NewAction/Attacks/water_jet_3.tres")}


func getNewAction_Attacks(resourceName : String) :
	return NewAction_AttacksDictionary.get(resourceName)

const NewAction_EnemyExclusiveAttacksDictionary = {
	"arcane_bolt" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/arcane_bolt.tres"),
	"bite_acid_2" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/bite_acid_2.tres"),
	"bite_big" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/bite_big.tres"),
	"bite_fire" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/bite_fire.tres"),
	"bite_generic" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/bite_generic.tres"),
	"bite_ice" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/bite_ice.tres"),
	"bite_ice_2" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/bite_ice_2.tres"),
	"bite_venomous" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/bite_venomous.tres"),
	"breath_acid" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/breath_acid.tres"),
	"breath_death" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/breath_death.tres"),
	"breath_fire" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/breath_fire.tres"),
	"breath_frost" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/breath_frost.tres"),
	"breath_iron" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/breath_iron.tres"),
	"breath_midas" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/breath_midas.tres"),
	"breath_shadow" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/breath_shadow.tres"),
	"breath_water" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/breath_water.tres"),
	"claw" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/claw.tres"),
	"claw_fire" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/claw_fire.tres"),
	"fire_blast" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/fire_blast.tres"),
	"fire_blast_2" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/fire_blast_2.tres"),
	"fire_blast_3" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/fire_blast_3.tres"),
	"gore" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/gore.tres"),
	"hellfire_cleave" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/hellfire_cleave.tres"),
	"ice_blast" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/ice_blast.tres"),
	"javelin" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/javelin.tres"),
	"jellyfish" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/jellyfish.tres"),
	"lava" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/lava.tres"),
	"poison_spore" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/poison_spore.tres"),
	"psychic_scream" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/psychic_scream.tres"),
	"stomp_fire" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/stomp_fire.tres"),
	"stomp_generic" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/stomp_generic.tres"),
	"stomp_ice" : preload("res://Resources/NewAction/EnemyExclusiveAttacks/stomp_ice.tres")}


func getNewAction_EnemyExclusiveAttacks(resourceName : String) :
	return NewAction_EnemyExclusiveAttacksDictionary.get(resourceName)

func getNewAction(resourceName : String) :
	if (NewAction_AttacksDictionary.has(resourceName)) :
		return NewAction_AttacksDictionary[resourceName]
	if (NewAction_EnemyExclusiveAttacksDictionary.has(resourceName)) :
		return NewAction_EnemyExclusiveAttacksDictionary[resourceName]
	return null

func getNewActionDictionary(type : String) :
	if type == "NewAction_Attacks" : 
		return NewAction_AttacksDictionary
	if type == "NewAction_EnemyExclusiveAttacks" : 
		return NewAction_EnemyExclusiveAttacksDictionary

func getAllNewAction() -> Array :
	var retVal : Array = []
	retVal.append_array(NewAction_AttacksDictionary.values())
	retVal.append_array(NewAction_EnemyExclusiveAttacksDictionary.values())
	return retVal

################################################################################
const NewActor_demonicDictionary = {
	"abomination" : preload("res://Resources/NewActor/demonic/abomination.tres"),
	"demon_elephant" : preload("res://Resources/NewActor/demonic/demon_elephant.tres"),
	"hairy_devil" : preload("res://Resources/NewActor/demonic/hairy_devil.tres"),
	"hell_beast" : preload("res://Resources/NewActor/demonic/hell_beast.tres"),
	"hell_hog" : preload("res://Resources/NewActor/demonic/hell_hog.tres"),
	"hell_hound" : preload("res://Resources/NewActor/demonic/hell_hound.tres"),
	"hellion" : preload("res://Resources/NewActor/demonic/hellion.tres"),
	"succubus" : preload("res://Resources/NewActor/demonic/succubus.tres"),
	"tormentor" : preload("res://Resources/NewActor/demonic/tormentor.tres")}


func getNewActor_demonic(resourceName : String) :
	return NewActor_demonicDictionary.get(resourceName)

const NewActor_demonic_militaryDictionary = {
	"apophis" : preload("res://Resources/NewActor/demonic_military/apophis.tres"),
	"arch_fiend" : preload("res://Resources/NewActor/demonic_military/arch_fiend.tres"),
	"balrog" : preload("res://Resources/NewActor/demonic_military/balrog.tres"),
	"death_knight" : preload("res://Resources/NewActor/demonic_military/death_knight.tres"),
	"hell_knight" : preload("res://Resources/NewActor/demonic_military/hell_knight.tres"),
	"hell_sentinel" : preload("res://Resources/NewActor/demonic_military/hell_sentinel.tres"),
	"hell_wizard" : preload("res://Resources/NewActor/demonic_military/hell_wizard.tres"),
	"imp" : preload("res://Resources/NewActor/demonic_military/imp.tres"),
	"salamander_melee" : preload("res://Resources/NewActor/demonic_military/salamander_melee.tres"),
	"salamander_ranged" : preload("res://Resources/NewActor/demonic_military/salamander_ranged.tres"),
	"shadow_imp" : preload("res://Resources/NewActor/demonic_military/shadow_imp.tres")}


func getNewActor_demonic_military(resourceName : String) :
	return NewActor_demonic_militaryDictionary.get(resourceName)

const NewActor_earthDictionary = {
	"deathcap" : preload("res://Resources/NewActor/earth/deathcap.tres"),
	"dryad" : preload("res://Resources/NewActor/earth/dryad.tres"),
	"faun" : preload("res://Resources/NewActor/earth/faun.tres"),
	"forest_drake" : preload("res://Resources/NewActor/earth/forest_drake.tres"),
	"hill_giant" : preload("res://Resources/NewActor/earth/hill_giant.tres"),
	"lindwurm" : preload("res://Resources/NewActor/earth/lindwurm.tres"),
	"rock_troll" : preload("res://Resources/NewActor/earth/rock_troll.tres"),
	"satyr" : preload("res://Resources/NewActor/earth/satyr.tres"),
	"swamp_dragon" : preload("res://Resources/NewActor/earth/swamp_dragon.tres"),
	"swamp_worm" : preload("res://Resources/NewActor/earth/swamp_worm.tres"),
	"thorned_ambusher" : preload("res://Resources/NewActor/earth/thorned_ambusher.tres"),
	"treant" : preload("res://Resources/NewActor/earth/treant.tres")}


func getNewActor_earth(resourceName : String) :
	return NewActor_earthDictionary.get(resourceName)

const NewActor_fireDictionary = {
	"fire_bat" : preload("res://Resources/NewActor/fire/fire_bat.tres"),
	"fire_crab" : preload("res://Resources/NewActor/fire/fire_crab.tres"),
	"fire_dragon" : preload("res://Resources/NewActor/fire/fire_dragon.tres"),
	"fire_elemental" : preload("res://Resources/NewActor/fire/fire_elemental.tres"),
	"fire_giant" : preload("res://Resources/NewActor/fire/fire_giant.tres"),
	"lava_fish" : preload("res://Resources/NewActor/fire/lava_fish.tres"),
	"lava_snake" : preload("res://Resources/NewActor/fire/lava_snake.tres"),
	"lava_worm" : preload("res://Resources/NewActor/fire/lava_worm.tres"),
	"lesser_fire_elemental" : preload("res://Resources/NewActor/fire/lesser_fire_elemental.tres")}


func getNewActor_fire(resourceName : String) :
	return NewActor_fireDictionary.get(resourceName)

const NewActor_greenskinDictionary = {
	"gnoll" : preload("res://Resources/NewActor/greenskin/gnoll.tres"),
	"goblin_ranger" : preload("res://Resources/NewActor/greenskin/goblin_ranger.tres"),
	"goblin_skirmisher" : preload("res://Resources/NewActor/greenskin/goblin_skirmisher.tres"),
	"juggernaut" : preload("res://Resources/NewActor/greenskin/juggernaut.tres"),
	"orc_knight" : preload("res://Resources/NewActor/greenskin/orc_knight.tres"),
	"orc_sorcerer" : preload("res://Resources/NewActor/greenskin/orc_sorcerer.tres"),
	"orc_warlord" : preload("res://Resources/NewActor/greenskin/orc_warlord.tres"),
	"polyphemus" : preload("res://Resources/NewActor/greenskin/polyphemus.tres")}


func getNewActor_greenskin(resourceName : String) :
	return NewActor_greenskinDictionary.get(resourceName)

const NewActor_iceDictionary = {
	"frost wraith" : preload("res://Resources/NewActor/ice/frost wraith.tres"),
	"ice_beast" : preload("res://Resources/NewActor/ice/ice_beast.tres"),
	"ice_devil" : preload("res://Resources/NewActor/ice/ice_devil.tres"),
	"ice_dragon" : preload("res://Resources/NewActor/ice/ice_dragon.tres"),
	"ice_elemental" : preload("res://Resources/NewActor/ice/ice_elemental.tres"),
	"ice_giant" : preload("res://Resources/NewActor/ice/ice_giant.tres"),
	"ice_hydra" : preload("res://Resources/NewActor/ice/ice_hydra.tres"),
	"ice_slime" : preload("res://Resources/NewActor/ice/ice_slime.tres"),
	"ice_troll" : preload("res://Resources/NewActor/ice/ice_troll.tres"),
	"polar_bear" : preload("res://Resources/NewActor/ice/polar_bear.tres")}


func getNewActor_ice(resourceName : String) :
	return NewActor_iceDictionary.get(resourceName)

const NewActor_merfolkDictionary = {
	"champion_of_poseidon" : preload("res://Resources/NewActor/merfolk/champion_of_poseidon.tres"),
	"ilsuiw" : preload("res://Resources/NewActor/merfolk/ilsuiw.tres"),
	"merfolk_T1_mage" : preload("res://Resources/NewActor/merfolk/merfolk_T1_mage.tres"),
	"merfolk_T1_spear" : preload("res://Resources/NewActor/merfolk/merfolk_T1_spear.tres"),
	"merfolk_T2_mage" : preload("res://Resources/NewActor/merfolk/merfolk_T2_mage.tres"),
	"merfolk_T2_spear_melee" : preload("res://Resources/NewActor/merfolk/merfolk_T2_spear_melee.tres"),
	"merfolk_T2_spear_ranged" : preload("res://Resources/NewActor/merfolk/merfolk_T2_spear_ranged.tres"),
	"siren" : preload("res://Resources/NewActor/merfolk/siren.tres")}


func getNewActor_merfolk(resourceName : String) :
	return NewActor_merfolkDictionary.get(resourceName)

const NewActor_miscDictionary = {
	"athena" : preload("res://Resources/NewActor/misc/athena.tres"),
	"caustic_shrike" : preload("res://Resources/NewActor/misc/caustic_shrike.tres"),
	"gold_dragon" : preload("res://Resources/NewActor/misc/gold_dragon.tres"),
	"iron_dragon" : preload("res://Resources/NewActor/misc/iron_dragon.tres"),
	"iron_golem" : preload("res://Resources/NewActor/misc/iron_golem.tres"),
	"iron_troll" : preload("res://Resources/NewActor/misc/iron_troll.tres"),
	"ironbrand" : preload("res://Resources/NewActor/misc/ironbrand.tres"),
	"shadow_dragon" : preload("res://Resources/NewActor/misc/shadow_dragon.tres"),
	"titan" : preload("res://Resources/NewActor/misc/titan.tres"),
	"troll" : preload("res://Resources/NewActor/misc/troll.tres")}


func getNewActor_misc(resourceName : String) :
	return NewActor_miscDictionary.get(resourceName)

const NewActor_undeadDictionary = {
	"amelia" : preload("res://Resources/NewActor/undead/amelia.tres"),
	"bone_dragon" : preload("res://Resources/NewActor/undead/bone_dragon.tres"),
	"death" : preload("res://Resources/NewActor/undead/death.tres"),
	"jiangshi_assassin" : preload("res://Resources/NewActor/undead/jiangshi_assassin.tres"),
	"lich" : preload("res://Resources/NewActor/undead/lich.tres"),
	"mummy_fighter" : preload("res://Resources/NewActor/undead/mummy_fighter.tres"),
	"mummy_ranger" : preload("res://Resources/NewActor/undead/mummy_ranger.tres"),
	"necromancer" : preload("res://Resources/NewActor/undead/necromancer.tres"),
	"rotten_amaglamation" : preload("res://Resources/NewActor/undead/rotten_amaglamation.tres"),
	"skeleton" : preload("res://Resources/NewActor/undead/skeleton.tres"),
	"vampire" : preload("res://Resources/NewActor/undead/vampire.tres")}


func getNewActor_undead(resourceName : String) :
	return NewActor_undeadDictionary.get(resourceName)

const NewActor_waterDictionary = {
	"black_mamba" : preload("res://Resources/NewActor/water/black_mamba.tres"),
	"electric_eel" : preload("res://Resources/NewActor/water/electric_eel.tres"),
	"jellyfish" : preload("res://Resources/NewActor/water/jellyfish.tres"),
	"kraken" : preload("res://Resources/NewActor/water/kraken.tres"),
	"nymph" : preload("res://Resources/NewActor/water/nymph.tres"),
	"shark" : preload("res://Resources/NewActor/water/shark.tres"),
	"water_dragon" : preload("res://Resources/NewActor/water/water_dragon.tres"),
	"water_elemental" : preload("res://Resources/NewActor/water/water_elemental.tres"),
	"water_troll" : preload("res://Resources/NewActor/water/water_troll.tres")}


func getNewActor_water(resourceName : String) :
	return NewActor_waterDictionary.get(resourceName)

func getNewActor(resourceName : String) :
	if (NewActor_demonicDictionary.has(resourceName)) :
		return NewActor_demonicDictionary[resourceName]
	if (NewActor_demonic_militaryDictionary.has(resourceName)) :
		return NewActor_demonic_militaryDictionary[resourceName]
	if (NewActor_earthDictionary.has(resourceName)) :
		return NewActor_earthDictionary[resourceName]
	if (NewActor_fireDictionary.has(resourceName)) :
		return NewActor_fireDictionary[resourceName]
	if (NewActor_greenskinDictionary.has(resourceName)) :
		return NewActor_greenskinDictionary[resourceName]
	if (NewActor_iceDictionary.has(resourceName)) :
		return NewActor_iceDictionary[resourceName]
	if (NewActor_merfolkDictionary.has(resourceName)) :
		return NewActor_merfolkDictionary[resourceName]
	if (NewActor_miscDictionary.has(resourceName)) :
		return NewActor_miscDictionary[resourceName]
	if (NewActor_undeadDictionary.has(resourceName)) :
		return NewActor_undeadDictionary[resourceName]
	if (NewActor_waterDictionary.has(resourceName)) :
		return NewActor_waterDictionary[resourceName]
	return null

func getNewActorDictionary(type : String) :
	if type == "NewActor_demonic" : 
		return NewActor_demonicDictionary
	if type == "NewActor_demonic_military" : 
		return NewActor_demonic_militaryDictionary
	if type == "NewActor_earth" : 
		return NewActor_earthDictionary
	if type == "NewActor_fire" : 
		return NewActor_fireDictionary
	if type == "NewActor_greenskin" : 
		return NewActor_greenskinDictionary
	if type == "NewActor_ice" : 
		return NewActor_iceDictionary
	if type == "NewActor_merfolk" : 
		return NewActor_merfolkDictionary
	if type == "NewActor_misc" : 
		return NewActor_miscDictionary
	if type == "NewActor_undead" : 
		return NewActor_undeadDictionary
	if type == "NewActor_water" : 
		return NewActor_waterDictionary

func getAllNewActor() -> Array :
	var retVal : Array = []
	retVal.append_array(NewActor_demonicDictionary.values())
	retVal.append_array(NewActor_demonic_militaryDictionary.values())
	retVal.append_array(NewActor_earthDictionary.values())
	retVal.append_array(NewActor_fireDictionary.values())
	retVal.append_array(NewActor_greenskinDictionary.values())
	retVal.append_array(NewActor_iceDictionary.values())
	retVal.append_array(NewActor_merfolkDictionary.values())
	retVal.append_array(NewActor_miscDictionary.values())
	retVal.append_array(NewActor_undeadDictionary.values())
	retVal.append_array(NewActor_waterDictionary.values())
	return retVal

################################################################################
const NewEquipment_AccessoriesDictionary = {
	"apple" : preload("res://Resources/NewEquipment/Accessories/apple.tres"),
	"bread" : preload("res://Resources/NewEquipment/Accessories/bread.tres"),
	"coating_divine" : preload("res://Resources/NewEquipment/Accessories/coating_divine.tres"),
	"coating_earth_1" : preload("res://Resources/NewEquipment/Accessories/coating_earth_1.tres"),
	"coating_earth_2" : preload("res://Resources/NewEquipment/Accessories/coating_earth_2.tres"),
	"coating_fire_1" : preload("res://Resources/NewEquipment/Accessories/coating_fire_1.tres"),
	"coating_fire_2" : preload("res://Resources/NewEquipment/Accessories/coating_fire_2.tres"),
	"coating_ice_1" : preload("res://Resources/NewEquipment/Accessories/coating_ice_1.tres"),
	"coating_ice_2" : preload("res://Resources/NewEquipment/Accessories/coating_ice_2.tres"),
	"coating_water_1" : preload("res://Resources/NewEquipment/Accessories/coating_water_1.tres"),
	"coating_water_2" : preload("res://Resources/NewEquipment/Accessories/coating_water_2.tres"),
	"feathered_hat" : preload("res://Resources/NewEquipment/Accessories/feathered_hat.tres"),
	"guantlets_cyclops_strength" : preload("res://Resources/NewEquipment/Accessories/guantlets_cyclops_strength.tres"),
	"lich_crown" : preload("res://Resources/NewEquipment/Accessories/lich_crown.tres"),
	"lightning_arrows_1" : preload("res://Resources/NewEquipment/Accessories/lightning_arrows_1.tres"),
	"lightning_arrows_2" : preload("res://Resources/NewEquipment/Accessories/lightning_arrows_2.tres"),
	"mage_hat" : preload("res://Resources/NewEquipment/Accessories/mage_hat.tres"),
	"mandrake_root_new" : preload("res://Resources/NewEquipment/Accessories/mandrake_root_new.tres"),
	"pizza" : preload("res://Resources/NewEquipment/Accessories/pizza.tres"),
	"rampart_agony" : preload("res://Resources/NewEquipment/Accessories/rampart_agony.tres"),
	"ring_authority" : preload("res://Resources/NewEquipment/Accessories/ring_authority.tres"),
	"ring_malice" : preload("res://Resources/NewEquipment/Accessories/ring_malice.tres"),
	"rune_advanced_automation" : preload("res://Resources/NewEquipment/Accessories/rune_advanced_automation.tres"),
	"shield_1" : preload("res://Resources/NewEquipment/Accessories/shield_1.tres"),
	"shield_2" : preload("res://Resources/NewEquipment/Accessories/shield_2.tres"),
	"shield_3" : preload("res://Resources/NewEquipment/Accessories/shield_3.tres"),
	"shield_4" : preload("res://Resources/NewEquipment/Accessories/shield_4.tres"),
	"shield_5" : preload("res://Resources/NewEquipment/Accessories/shield_5.tres"),
	"tome_death_1" : preload("res://Resources/NewEquipment/Accessories/tome_death_1.tres"),
	"tome_death_2" : preload("res://Resources/NewEquipment/Accessories/tome_death_2.tres"),
	"tome_earth_1" : preload("res://Resources/NewEquipment/Accessories/tome_earth_1.tres"),
	"tome_earth_2" : preload("res://Resources/NewEquipment/Accessories/tome_earth_2.tres"),
	"tome_fire_1" : preload("res://Resources/NewEquipment/Accessories/tome_fire_1.tres"),
	"tome_fire_2" : preload("res://Resources/NewEquipment/Accessories/tome_fire_2.tres"),
	"tome_ice_1" : preload("res://Resources/NewEquipment/Accessories/tome_ice_1.tres"),
	"tome_ice_2" : preload("res://Resources/NewEquipment/Accessories/tome_ice_2.tres"),
	"tome_water_1" : preload("res://Resources/NewEquipment/Accessories/tome_water_1.tres"),
	"tome_water_2" : preload("res://Resources/NewEquipment/Accessories/tome_water_2.tres"),
	"warrior_helm" : preload("res://Resources/NewEquipment/Accessories/warrior_helm.tres")}


func getNewEquipment_Accessories(resourceName : String) :
	return NewEquipment_AccessoriesDictionary.get(resourceName)

const NewEquipment_ArmorDictionary = {
	"armor_blades" : preload("res://Resources/NewEquipment/Armor/armor_blades.tres"),
	"armor_cephalopod" : preload("res://Resources/NewEquipment/Armor/armor_cephalopod.tres"),
	"armor_crab" : preload("res://Resources/NewEquipment/Armor/armor_crab.tres"),
	"armor_dragonscale" : preload("res://Resources/NewEquipment/Armor/armor_dragonscale.tres"),
	"armor_earth" : preload("res://Resources/NewEquipment/Armor/armor_earth.tres"),
	"armor_fire" : preload("res://Resources/NewEquipment/Armor/armor_fire.tres"),
	"armor_ice" : preload("res://Resources/NewEquipment/Armor/armor_ice.tres"),
	"armor_leather" : preload("res://Resources/NewEquipment/Armor/armor_leather.tres"),
	"armor_molten" : preload("res://Resources/NewEquipment/Armor/armor_molten.tres"),
	"armor_scale" : preload("res://Resources/NewEquipment/Armor/armor_scale.tres"),
	"armor_shadow" : preload("res://Resources/NewEquipment/Armor/armor_shadow.tres"),
	"armor_water" : preload("res://Resources/NewEquipment/Armor/armor_water.tres"),
	"chainmail" : preload("res://Resources/NewEquipment/Armor/chainmail.tres"),
	"dragonbone_plate" : preload("res://Resources/NewEquipment/Armor/dragonbone_plate.tres"),
	"enchanted_robes_new" : preload("res://Resources/NewEquipment/Armor/enchanted_robes_new.tres"),
	"fur_cloak" : preload("res://Resources/NewEquipment/Armor/fur_cloak.tres"),
	"hellforged_plate" : preload("res://Resources/NewEquipment/Armor/hellforged_plate.tres"),
	"mithril_plate" : preload("res://Resources/NewEquipment/Armor/mithril_plate.tres"),
	"moonstone_plate" : preload("res://Resources/NewEquipment/Armor/moonstone_plate.tres"),
	"scale_mail" : preload("res://Resources/NewEquipment/Armor/scale_mail.tres"),
	"silk_robes" : preload("res://Resources/NewEquipment/Armor/silk_robes.tres"),
	"steel_plate" : preload("res://Resources/NewEquipment/Armor/steel_plate.tres"),
	"studded_leather" : preload("res://Resources/NewEquipment/Armor/studded_leather.tres")}


func getNewEquipment_Armor(resourceName : String) :
	return NewEquipment_ArmorDictionary.get(resourceName)

const NewEquipment_CurrencyDictionary = {
	"gold_coin" : preload("res://Resources/NewEquipment/Currency/gold_coin.tres"),
	"ore" : preload("res://Resources/NewEquipment/Currency/ore.tres"),
	"soul" : preload("res://Resources/NewEquipment/Currency/soul.tres")}


func getNewEquipment_Currency(resourceName : String) :
	return NewEquipment_CurrencyDictionary.get(resourceName)

const NewEquipment_WeaponsDictionary = {
	"adamantium_greataxe" : preload("res://Resources/NewEquipment/Weapons/adamantium_greataxe.tres"),
	"amelia_wand" : preload("res://Resources/NewEquipment/Weapons/amelia_wand.tres"),
	"arming_sword" : preload("res://Resources/NewEquipment/Weapons/arming_sword.tres"),
	"asmodeus_staff" : preload("res://Resources/NewEquipment/Weapons/asmodeus_staff.tres"),
	"bardiche" : preload("res://Resources/NewEquipment/Weapons/bardiche.tres"),
	"blade_of_darkness" : preload("res://Resources/NewEquipment/Weapons/blade_of_darkness.tres"),
	"blade_of_sin" : preload("res://Resources/NewEquipment/Weapons/blade_of_sin.tres"),
	"blade_of_the_lich_king" : preload("res://Resources/NewEquipment/Weapons/blade_of_the_lich_king.tres"),
	"blunderbuss" : preload("res://Resources/NewEquipment/Weapons/blunderbuss.tres"),
	"club" : preload("res://Resources/NewEquipment/Weapons/club.tres"),
	"crackling_greataxe" : preload("res://Resources/NewEquipment/Weapons/crackling_greataxe.tres"),
	"crossbow" : preload("res://Resources/NewEquipment/Weapons/crossbow.tres"),
	"crystal_sword" : preload("res://Resources/NewEquipment/Weapons/crystal_sword.tres"),
	"cutlass" : preload("res://Resources/NewEquipment/Weapons/cutlass.tres"),
	"dagger" : preload("res://Resources/NewEquipment/Weapons/dagger.tres"),
	"deaths_scythe" : preload("res://Resources/NewEquipment/Weapons/deaths_scythe.tres"),
	"diamond_staff" : preload("res://Resources/NewEquipment/Weapons/diamond_staff.tres"),
	"divine_greatmace" : preload("res://Resources/NewEquipment/Weapons/divine_greatmace.tres"),
	"earth_hammer" : preload("res://Resources/NewEquipment/Weapons/earth_hammer.tres"),
	"earth_staff" : preload("res://Resources/NewEquipment/Weapons/earth_staff.tres"),
	"emerald_lance" : preload("res://Resources/NewEquipment/Weapons/emerald_lance.tres"),
	"emerald_staff" : preload("res://Resources/NewEquipment/Weapons/emerald_staff.tres"),
	"fire_spitter" : preload("res://Resources/NewEquipment/Weapons/fire_spitter.tres"),
	"fire_staff" : preload("res://Resources/NewEquipment/Weapons/fire_staff.tres"),
	"flail" : preload("res://Resources/NewEquipment/Weapons/flail.tres"),
	"flamberge" : preload("res://Resources/NewEquipment/Weapons/flamberge.tres"),
	"force_wand" : preload("res://Resources/NewEquipment/Weapons/force_wand.tres"),
	"frostfire_twindaggers" : preload("res://Resources/NewEquipment/Weapons/frostfire_twindaggers.tres"),
	"great_flail" : preload("res://Resources/NewEquipment/Weapons/great_flail.tres"),
	"greataxe" : preload("res://Resources/NewEquipment/Weapons/greataxe.tres"),
	"greatsword" : preload("res://Resources/NewEquipment/Weapons/greatsword.tres"),
	"halberd" : preload("res://Resources/NewEquipment/Weapons/halberd.tres"),
	"hammer" : preload("res://Resources/NewEquipment/Weapons/hammer.tres"),
	"handaxe" : preload("res://Resources/NewEquipment/Weapons/handaxe.tres"),
	"hungering_blade" : preload("res://Resources/NewEquipment/Weapons/hungering_blade.tres"),
	"ice_lance" : preload("res://Resources/NewEquipment/Weapons/ice_lance.tres"),
	"ice_staff" : preload("res://Resources/NewEquipment/Weapons/ice_staff.tres"),
	"ice_sword" : preload("res://Resources/NewEquipment/Weapons/ice_sword.tres"),
	"katana" : preload("res://Resources/NewEquipment/Weapons/katana.tres"),
	"khopesh" : preload("res://Resources/NewEquipment/Weapons/khopesh.tres"),
	"kunai" : preload("res://Resources/NewEquipment/Weapons/kunai.tres"),
	"lance" : preload("res://Resources/NewEquipment/Weapons/lance.tres"),
	"lightning_rod" : preload("res://Resources/NewEquipment/Weapons/lightning_rod.tres"),
	"long_axe" : preload("res://Resources/NewEquipment/Weapons/long_axe.tres"),
	"longbow" : preload("res://Resources/NewEquipment/Weapons/longbow.tres"),
	"longsword" : preload("res://Resources/NewEquipment/Weapons/longsword.tres"),
	"mace" : preload("res://Resources/NewEquipment/Weapons/mace.tres"),
	"magma_greatsword" : preload("res://Resources/NewEquipment/Weapons/magma_greatsword.tres"),
	"masamune" : preload("res://Resources/NewEquipment/Weapons/masamune.tres"),
	"morning_star" : preload("res://Resources/NewEquipment/Weapons/morning_star.tres"),
	"muramasa" : preload("res://Resources/NewEquipment/Weapons/muramasa.tres"),
	"occult_wand" : preload("res://Resources/NewEquipment/Weapons/occult_wand.tres"),
	"phantasm" : preload("res://Resources/NewEquipment/Weapons/phantasm.tres"),
	"pike" : preload("res://Resources/NewEquipment/Weapons/pike.tres"),
	"rapier" : preload("res://Resources/NewEquipment/Weapons/rapier.tres"),
	"recurve_bow" : preload("res://Resources/NewEquipment/Weapons/recurve_bow.tres"),
	"rhomphaia" : preload("res://Resources/NewEquipment/Weapons/rhomphaia.tres"),
	"rondel_dagger" : preload("res://Resources/NewEquipment/Weapons/rondel_dagger.tres"),
	"ruby_staff_new" : preload("res://Resources/NewEquipment/Weapons/ruby_staff_new.tres"),
	"sapphire_staff" : preload("res://Resources/NewEquipment/Weapons/sapphire_staff.tres"),
	"scimitar" : preload("res://Resources/NewEquipment/Weapons/scimitar.tres"),
	"scythe" : preload("res://Resources/NewEquipment/Weapons/scythe.tres"),
	"shamshir" : preload("res://Resources/NewEquipment/Weapons/shamshir.tres"),
	"shortbow" : preload("res://Resources/NewEquipment/Weapons/shortbow.tres"),
	"shortsword" : preload("res://Resources/NewEquipment/Weapons/shortsword.tres"),
	"shotel" : preload("res://Resources/NewEquipment/Weapons/shotel.tres"),
	"sling_new" : preload("res://Resources/NewEquipment/Weapons/sling_new.tres"),
	"spear" : preload("res://Resources/NewEquipment/Weapons/spear.tres"),
	"staff_of_pain" : preload("res://Resources/NewEquipment/Weapons/staff_of_pain.tres"),
	"star_rod" : preload("res://Resources/NewEquipment/Weapons/star_rod.tres"),
	"throwing_axe" : preload("res://Resources/NewEquipment/Weapons/throwing_axe.tres"),
	"topaz_staff" : preload("res://Resources/NewEquipment/Weapons/topaz_staff.tres"),
	"trident" : preload("res://Resources/NewEquipment/Weapons/trident.tres"),
	"twisted_sword" : preload("res://Resources/NewEquipment/Weapons/twisted_sword.tres"),
	"umbral_dagger" : preload("res://Resources/NewEquipment/Weapons/umbral_dagger.tres"),
	"void_blade" : preload("res://Resources/NewEquipment/Weapons/void_blade.tres"),
	"wand_of_disintigration" : preload("res://Resources/NewEquipment/Weapons/wand_of_disintigration.tres"),
	"warbrand" : preload("res://Resources/NewEquipment/Weapons/warbrand.tres"),
	"water_staff" : preload("res://Resources/NewEquipment/Weapons/water_staff.tres"),
	"whip" : preload("res://Resources/NewEquipment/Weapons/whip.tres")}


func getNewEquipment_Weapons(resourceName : String) :
	return NewEquipment_WeaponsDictionary.get(resourceName)

func getNewEquipment(resourceName : String) :
	if (NewEquipment_AccessoriesDictionary.has(resourceName)) :
		return NewEquipment_AccessoriesDictionary[resourceName]
	if (NewEquipment_ArmorDictionary.has(resourceName)) :
		return NewEquipment_ArmorDictionary[resourceName]
	if (NewEquipment_CurrencyDictionary.has(resourceName)) :
		return NewEquipment_CurrencyDictionary[resourceName]
	if (NewEquipment_WeaponsDictionary.has(resourceName)) :
		return NewEquipment_WeaponsDictionary[resourceName]
	return null

func getNewEquipmentDictionary(type : String) :
	if type == "NewEquipment_Accessories" : 
		return NewEquipment_AccessoriesDictionary
	if type == "NewEquipment_Armor" : 
		return NewEquipment_ArmorDictionary
	if type == "NewEquipment_Currency" : 
		return NewEquipment_CurrencyDictionary
	if type == "NewEquipment_Weapons" : 
		return NewEquipment_WeaponsDictionary

func getAllNewEquipment() -> Array :
	var retVal : Array = []
	retVal.append_array(NewEquipment_AccessoriesDictionary.values())
	retVal.append_array(NewEquipment_ArmorDictionary.values())
	retVal.append_array(NewEquipment_CurrencyDictionary.values())
	retVal.append_array(NewEquipment_WeaponsDictionary.values())
	return retVal

################################################################################
const OldAction_AttacksDictionary = {
	"bite" : preload("res://Resources/OldAction/Attacks/Enemies/bite.tres"),
	"leech_bolt_1_enemy" : preload("res://Resources/OldAction/Attacks/Enemies/leech_bolt_1_enemy.tres"),
	"mandragora" : preload("res://Resources/OldAction/Attacks/Enemies/mandragora.tres"),
	"multi_peck" : preload("res://Resources/OldAction/Attacks/Enemies/multi_peck.tres"),
	"peck" : preload("res://Resources/OldAction/Attacks/Enemies/peck.tres"),
	"ram" : preload("res://Resources/OldAction/Attacks/Enemies/ram.tres"),
	"vampiric_bite" : preload("res://Resources/OldAction/Attacks/Enemies/vampiric_bite.tres"),
	"bludgeon" : preload("res://Resources/OldAction/Attacks/Tutorial/bludgeon.tres"),
	"claw_zombie" : preload("res://Resources/OldAction/Attacks/Tutorial/claw_zombie.tres"),
	"knifehand" : preload("res://Resources/OldAction/Attacks/Tutorial/knifehand.tres"),
	"knifehand_2" : preload("res://Resources/OldAction/Attacks/Tutorial/knifehand_2.tres"),
	"nibble" : preload("res://Resources/OldAction/Attacks/Tutorial/nibble.tres"),
	"psy_blast" : preload("res://Resources/OldAction/Attacks/Tutorial/psy_blast.tres"),
	"psy_blast_2" : preload("res://Resources/OldAction/Attacks/Tutorial/psy_blast_2.tres"),
	"punch" : preload("res://Resources/OldAction/Attacks/Tutorial/punch.tres"),
	"punch_2" : preload("res://Resources/OldAction/Attacks/Tutorial/punch_2.tres"),
	"weak_stab" : preload("res://Resources/OldAction/Attacks/Tutorial/weak_stab.tres"),
	"claw_basic" : preload("res://Resources/OldAction/Attacks/claw_basic.tres"),
	"dagger_basic" : preload("res://Resources/OldAction/Attacks/dagger_basic.tres"),
	"dart_basic" : preload("res://Resources/OldAction/Attacks/dart_basic.tres"),
	"executioners_sword_basic" : preload("res://Resources/OldAction/Attacks/executioners_sword_basic.tres"),
	"godpunch" : preload("res://Resources/OldAction/Attacks/godpunch.tres"),
	"kinetic_blast" : preload("res://Resources/OldAction/Attacks/kinetic_blast.tres"),
	"leech_bolt_1" : preload("res://Resources/OldAction/Attacks/leech_bolt_1.tres"),
	"light_laser_1" : preload("res://Resources/OldAction/Attacks/light_laser_1.tres"),
	"sling_basic" : preload("res://Resources/OldAction/Attacks/sling_basic.tres"),
	"spear_basic" : preload("res://Resources/OldAction/Attacks/spear_basic.tres"),
	"triple_peck" : preload("res://Resources/OldAction/Attacks/triple_peck.tres")}


func getOldAction_Attacks(resourceName : String) :
	return OldAction_AttacksDictionary.get(resourceName)

func getOldAction(resourceName : String) :
	if (OldAction_AttacksDictionary.has(resourceName)) :
		return OldAction_AttacksDictionary[resourceName]
	return null

func getOldActionDictionary(type : String) :
	if type == "OldAction_Attacks" : 
		return OldAction_AttacksDictionary

func getAllOldAction() -> Array :
	var retVal : Array = []
	retVal.append_array(OldAction_AttacksDictionary.values())
	return retVal

################################################################################
const OldActor_Floor0Dictionary = {
	"goblin" : preload("res://Resources/OldActor/Floor0/goblin.tres"),
	"hobgoblin" : preload("res://Resources/OldActor/Floor0/hobgoblin.tres"),
	"orc" : preload("res://Resources/OldActor/Floor0/orc.tres"),
	"rat" : preload("res://Resources/OldActor/Floor0/rat.tres"),
	"zombie" : preload("res://Resources/OldActor/Floor0/zombie.tres")}


func getOldActor_Floor0(resourceName : String) :
	return OldActor_Floor0Dictionary.get(resourceName)

const OldActor_Floor1Dictionary = {
	"cave_mole" : preload("res://Resources/OldActor/Floor1/cave_mole.tres"),
	"dire_rat" : preload("res://Resources/OldActor/Floor1/dire_rat.tres"),
	"giant_magpie" : preload("res://Resources/OldActor/Floor1/giant_magpie.tres"),
	"goose_hydra" : preload("res://Resources/OldActor/Floor1/goose_hydra.tres"),
	"kiwi" : preload("res://Resources/OldActor/Floor1/kiwi.tres"),
	"mandragora" : preload("res://Resources/OldActor/Floor1/mandragora.tres"),
	"manticore" : preload("res://Resources/OldActor/Floor1/manticore.tres"),
	"mini_roc" : preload("res://Resources/OldActor/Floor1/mini_roc.tres"),
	"mountain_goat" : preload("res://Resources/OldActor/Floor1/mountain_goat.tres"),
	"mountain_lion" : preload("res://Resources/OldActor/Floor1/mountain_lion.tres"),
	"rock_dove" : preload("res://Resources/OldActor/Floor1/rock_dove.tres"),
	"vampire_bat" : preload("res://Resources/OldActor/Floor1/vampire_bat.tres")}


func getOldActor_Floor1(resourceName : String) :
	return OldActor_Floor1Dictionary.get(resourceName)

const OldActor_MiscDictionary = {
	"human" : preload("res://Resources/OldActor/Misc/human.tres")}


func getOldActor_Misc(resourceName : String) :
	return OldActor_MiscDictionary.get(resourceName)

func getOldActor(resourceName : String) :
	if (OldActor_Floor0Dictionary.has(resourceName)) :
		return OldActor_Floor0Dictionary[resourceName]
	if (OldActor_Floor1Dictionary.has(resourceName)) :
		return OldActor_Floor1Dictionary[resourceName]
	if (OldActor_MiscDictionary.has(resourceName)) :
		return OldActor_MiscDictionary[resourceName]
	return null

func getOldActorDictionary(type : String) :
	if type == "OldActor_Floor0" : 
		return OldActor_Floor0Dictionary
	if type == "OldActor_Floor1" : 
		return OldActor_Floor1Dictionary
	if type == "OldActor_Misc" : 
		return OldActor_MiscDictionary

func getAllOldActor() -> Array :
	var retVal : Array = []
	retVal.append_array(OldActor_Floor0Dictionary.values())
	retVal.append_array(OldActor_Floor1Dictionary.values())
	retVal.append_array(OldActor_MiscDictionary.values())
	return retVal

################################################################################
const OldEquipment_AccessoriesDictionary = {
	"clown_horn" : preload("res://Resources/OldEquipment/Accessories/clown_horn.tres"),
	"kiwi" : preload("res://Resources/OldEquipment/Accessories/kiwi.tres"),
	"magpie_eye" : preload("res://Resources/OldEquipment/Accessories/magpie_eye.tres"),
	"mandragora_potion" : preload("res://Resources/OldEquipment/Accessories/mandragora_potion.tres"),
	"manticore_tooth" : preload("res://Resources/OldEquipment/Accessories/manticore_tooth.tres"),
	"manticore_venom" : preload("res://Resources/OldEquipment/Accessories/manticore_venom.tres"),
	"ram_horn" : preload("res://Resources/OldEquipment/Accessories/ram_horn.tres")}


func getOldEquipment_Accessories(resourceName : String) :
	return OldEquipment_AccessoriesDictionary.get(resourceName)

const OldEquipment_ArmorDictionary = {
	"casual" : preload("res://Resources/OldEquipment/Armor/casual.tres"),
	"enchanted_robes" : preload("res://Resources/OldEquipment/Armor/enchanted_robes.tres"),
	"feathered_cloak" : preload("res://Resources/OldEquipment/Armor/feathered_cloak.tres"),
	"lion_armor" : preload("res://Resources/OldEquipment/Armor/lion_armor.tres"),
	"manticore_armor" : preload("res://Resources/OldEquipment/Armor/manticore_armor.tres"),
	"mole_armor" : preload("res://Resources/OldEquipment/Armor/mole_armor.tres"),
	"scraps" : preload("res://Resources/OldEquipment/Armor/scraps.tres")}


func getOldEquipment_Armor(resourceName : String) :
	return OldEquipment_ArmorDictionary.get(resourceName)

const OldEquipment_CurrencyDictionary = {
	"pebble" : preload("res://Resources/OldEquipment/Currency/pebble.tres")}


func getOldEquipment_Currency(resourceName : String) :
	return OldEquipment_CurrencyDictionary.get(resourceName)

const OldEquipment_WeaponsDictionary = {
	"bat_bat" : preload("res://Resources/OldEquipment/Weapons/bat_bat.tres"),
	"bat_staff" : preload("res://Resources/OldEquipment/Weapons/bat_staff.tres"),
	"cat_claws" : preload("res://Resources/OldEquipment/Weapons/cat_claws.tres"),
	"goose_flail" : preload("res://Resources/OldEquipment/Weapons/goose_flail.tres"),
	"kiwi_dagger" : preload("res://Resources/OldEquipment/Weapons/kiwi_dagger.tres"),
	"magic_stick_int" : preload("res://Resources/OldEquipment/Weapons/magic_stick_int.tres"),
	"magic_stick_str" : preload("res://Resources/OldEquipment/Weapons/magic_stick_str.tres"),
	"manticore_dart" : preload("res://Resources/OldEquipment/Weapons/manticore_dart.tres"),
	"manticore_wand" : preload("res://Resources/OldEquipment/Weapons/manticore_wand.tres"),
	"mole_pickaxe" : preload("res://Resources/OldEquipment/Weapons/mole_pickaxe.tres"),
	"ram_spear" : preload("res://Resources/OldEquipment/Weapons/ram_spear.tres"),
	"ruby_staff" : preload("res://Resources/OldEquipment/Weapons/ruby_staff.tres"),
	"shiv" : preload("res://Resources/OldEquipment/Weapons/shiv.tres"),
	"sling" : preload("res://Resources/OldEquipment/Weapons/sling.tres"),
	"unarmed_Fighter" : preload("res://Resources/OldEquipment/Weapons/unarmed_Fighter.tres"),
	"unarmed_Mage" : preload("res://Resources/OldEquipment/Weapons/unarmed_Mage.tres"),
	"unarmed_Rogue" : preload("res://Resources/OldEquipment/Weapons/unarmed_Rogue.tres")}


func getOldEquipment_Weapons(resourceName : String) :
	return OldEquipment_WeaponsDictionary.get(resourceName)

func getOldEquipment(resourceName : String) :
	if (OldEquipment_AccessoriesDictionary.has(resourceName)) :
		return OldEquipment_AccessoriesDictionary[resourceName]
	if (OldEquipment_ArmorDictionary.has(resourceName)) :
		return OldEquipment_ArmorDictionary[resourceName]
	if (OldEquipment_CurrencyDictionary.has(resourceName)) :
		return OldEquipment_CurrencyDictionary[resourceName]
	if (OldEquipment_WeaponsDictionary.has(resourceName)) :
		return OldEquipment_WeaponsDictionary[resourceName]
	return null

func getOldEquipmentDictionary(type : String) :
	if type == "OldEquipment_Accessories" : 
		return OldEquipment_AccessoriesDictionary
	if type == "OldEquipment_Armor" : 
		return OldEquipment_ArmorDictionary
	if type == "OldEquipment_Currency" : 
		return OldEquipment_CurrencyDictionary
	if type == "OldEquipment_Weapons" : 
		return OldEquipment_WeaponsDictionary

func getAllOldEquipment() -> Array :
	var retVal : Array = []
	retVal.append_array(OldEquipment_AccessoriesDictionary.values())
	retVal.append_array(OldEquipment_ArmorDictionary.values())
	retVal.append_array(OldEquipment_CurrencyDictionary.values())
	retVal.append_array(OldEquipment_WeaponsDictionary.values())
	return retVal

################################################################################
const Routine_FilesDictionary = {
	"barefoot" : preload("res://Resources/Routine/Files/barefoot.tres"),
	"earth" : preload("res://Resources/Routine/Files/earth.tres"),
	"fence_swordfish" : preload("res://Resources/Routine/Files/fence_swordfish.tres"),
	"fire" : preload("res://Resources/Routine/Files/fire.tres"),
	"hug_cacti" : preload("res://Resources/Routine/Files/hug_cacti.tres"),
	"hunt" : preload("res://Resources/Routine/Files/hunt.tres"),
	"ice" : preload("res://Resources/Routine/Files/ice.tres"),
	"lift_weights" : preload("res://Resources/Routine/Files/lift_weights.tres"),
	"offhand" : preload("res://Resources/Routine/Files/offhand.tres"),
	"parrying" : preload("res://Resources/Routine/Files/parrying.tres"),
	"pickpocket_goblins" : preload("res://Resources/Routine/Files/pickpocket_goblins.tres"),
	"punch_walls" : preload("res://Resources/Routine/Files/punch_walls.tres"),
	"read_novels" : preload("res://Resources/Routine/Files/read_novels.tres"),
	"spar" : preload("res://Resources/Routine/Files/spar.tres"),
	"spar_herophile" : preload("res://Resources/Routine/Files/spar_herophile.tres"),
	"tactics" : preload("res://Resources/Routine/Files/tactics.tres")}


func getRoutine_Files(resourceName : String) :
	return Routine_FilesDictionary.get(resourceName)

func getRoutine(resourceName : String) :
	if (Routine_FilesDictionary.has(resourceName)) :
		return Routine_FilesDictionary[resourceName]
	return null

func getRoutineDictionary(type : String) :
	if type == "Routine_Files" : 
		return Routine_FilesDictionary

func getAllRoutine() -> Array :
	var retVal : Array = []
	retVal.append_array(Routine_FilesDictionary.values())
	return retVal

################################################################################
const PlayerTexture_bardingDictionary = {
	"black_knight" : preload("res://Resources/Textures/PlayerTexture/barding/black_knight.png"),
	"centaur_barding_blue" : preload("res://Resources/Textures/PlayerTexture/barding/centaur_barding_blue.png"),
	"centaur_barding_magenta" : preload("res://Resources/Textures/PlayerTexture/barding/centaur_barding_magenta.png"),
	"centaur_barding_metal" : preload("res://Resources/Textures/PlayerTexture/barding/centaur_barding_metal.png"),
	"centaur_barding_red" : preload("res://Resources/Textures/PlayerTexture/barding/centaur_barding_red.png"),
	"lightning_scales" : preload("res://Resources/Textures/PlayerTexture/barding/lightning_scales.png"),
	"naga_barding_blue" : preload("res://Resources/Textures/PlayerTexture/barding/naga_barding_blue.png"),
	"naga_barding_magenta" : preload("res://Resources/Textures/PlayerTexture/barding/naga_barding_magenta.png"),
	"naga_barding_metal" : preload("res://Resources/Textures/PlayerTexture/barding/naga_barding_metal.png"),
	"naga_barding_red" : preload("res://Resources/Textures/PlayerTexture/barding/naga_barding_red.png")}


func getPlayerTexture_barding(resourceName : String) :
	return PlayerTexture_bardingDictionary.get(resourceName)

const PlayerTexture_baseDictionary = {
	"ahuman_female" : preload("res://Resources/Textures/PlayerTexture/base/ahuman_female.png"),
	"ahuman_male" : preload("res://Resources/Textures/PlayerTexture/base/ahuman_male.png"),
	"centaur_brown_female" : preload("res://Resources/Textures/PlayerTexture/base/centaur_brown_female.png"),
	"centaur_brown_male" : preload("res://Resources/Textures/PlayerTexture/base/centaur_brown_male.png"),
	"centaur_darkbrown_female" : preload("res://Resources/Textures/PlayerTexture/base/centaur_darkbrown_female.png"),
	"centaur_darkbrown_male" : preload("res://Resources/Textures/PlayerTexture/base/centaur_darkbrown_male.png"),
	"centaur_darkgrey_female" : preload("res://Resources/Textures/PlayerTexture/base/centaur_darkgrey_female.png"),
	"centaur_darkgrey_male" : preload("res://Resources/Textures/PlayerTexture/base/centaur_darkgrey_male.png"),
	"centaur_lightbrown_female" : preload("res://Resources/Textures/PlayerTexture/base/centaur_lightbrown_female.png"),
	"centaur_lightbrown_male" : preload("res://Resources/Textures/PlayerTexture/base/centaur_lightbrown_male.png"),
	"centaur_lightgrey_female" : preload("res://Resources/Textures/PlayerTexture/base/centaur_lightgrey_female.png"),
	"centaur_lightgrey_male" : preload("res://Resources/Textures/PlayerTexture/base/centaur_lightgrey_male.png"),
	"demigod_male" : preload("res://Resources/Textures/PlayerTexture/base/demigod_male.png"),
	"demonspawn_black_female" : preload("res://Resources/Textures/PlayerTexture/base/demonspawn_black_female.png"),
	"demonspawn_black_male" : preload("res://Resources/Textures/PlayerTexture/base/demonspawn_black_male.png"),
	"demonspawn_pink" : preload("res://Resources/Textures/PlayerTexture/base/demonspawn_pink.png"),
	"demonspawn_red_female" : preload("res://Resources/Textures/PlayerTexture/base/demonspawn_red_female.png"),
	"demonspawn_red_male" : preload("res://Resources/Textures/PlayerTexture/base/demonspawn_red_male.png"),
	"draconian_black_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_black_female.png"),
	"draconian_black_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_black_male.png"),
	"draconian_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_female.png"),
	"draconian_gold_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_gold_female.png"),
	"draconian_gold_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_gold_male.png"),
	"draconian_gray_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_gray_female.png"),
	"draconian_gray_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_gray_male.png"),
	"draconian_green_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_green_female.png"),
	"draconian_green_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_green_male.png"),
	"draconian_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_male.png"),
	"draconian_mottled_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_mottled_female.png"),
	"draconian_mottled_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_mottled_male.png"),
	"draconian_pale_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_pale_female.png"),
	"draconian_pale_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_pale_male.png"),
	"draconian_purple_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_purple_female.png"),
	"draconian_purple_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_purple_male.png"),
	"draconian_red_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_red_female.png"),
	"draconian_red_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_red_male.png"),
	"draconian_white_female" : preload("res://Resources/Textures/PlayerTexture/base/draconian_white_female.png"),
	"draconian_white_male" : preload("res://Resources/Textures/PlayerTexture/base/draconian_white_male.png"),
	"dwarf_female" : preload("res://Resources/Textures/PlayerTexture/base/dwarf_female.png"),
	"dwarf_male" : preload("res://Resources/Textures/PlayerTexture/base/dwarf_male.png"),
	"elf_female" : preload("res://Resources/Textures/PlayerTexture/base/elf_female.png"),
	"elf_male" : preload("res://Resources/Textures/PlayerTexture/base/elf_male.png"),
	"formicid" : preload("res://Resources/Textures/PlayerTexture/base/formicid.png"),
	"gargoyle_female" : preload("res://Resources/Textures/PlayerTexture/base/gargoyle_female.png"),
	"gargoyle_male" : preload("res://Resources/Textures/PlayerTexture/base/gargoyle_male.png"),
	"ghoul" : preload("res://Resources/Textures/PlayerTexture/base/ghoul.png"),
	"ghoul_2_female" : preload("res://Resources/Textures/PlayerTexture/base/ghoul_2_female.png"),
	"ghoul_2_male" : preload("res://Resources/Textures/PlayerTexture/base/ghoul_2_male.png"),
	"gnome_female" : preload("res://Resources/Textures/PlayerTexture/base/gnome_female.png"),
	"gnome_male" : preload("res://Resources/Textures/PlayerTexture/base/gnome_male.png"),
	"halfling_female" : preload("res://Resources/Textures/PlayerTexture/base/halfling_female.png"),
	"halfling_male" : preload("res://Resources/Textures/PlayerTexture/base/halfling_male.png"),
	"kenku_winged_female" : preload("res://Resources/Textures/PlayerTexture/base/kenku_winged_female.png"),
	"kenku_winged_male" : preload("res://Resources/Textures/PlayerTexture/base/kenku_winged_male.png"),
	"kenku_wingless_female" : preload("res://Resources/Textures/PlayerTexture/base/kenku_wingless_female.png"),
	"kenku_wingless_male" : preload("res://Resources/Textures/PlayerTexture/base/kenku_wingless_male.png"),
	"kobold_female_new" : preload("res://Resources/Textures/PlayerTexture/base/kobold_female_new.png"),
	"kobold_female_old" : preload("res://Resources/Textures/PlayerTexture/base/kobold_female_old.png"),
	"kobold_male_new" : preload("res://Resources/Textures/PlayerTexture/base/kobold_male_new.png"),
	"kobold_male_old" : preload("res://Resources/Textures/PlayerTexture/base/kobold_male_old.png"),
	"lorc_female_0" : preload("res://Resources/Textures/PlayerTexture/base/lorc_female_0.png"),
	"lorc_female_1" : preload("res://Resources/Textures/PlayerTexture/base/lorc_female_1.png"),
	"lorc_female_2" : preload("res://Resources/Textures/PlayerTexture/base/lorc_female_2.png"),
	"lorc_female_3" : preload("res://Resources/Textures/PlayerTexture/base/lorc_female_3.png"),
	"lorc_female_4" : preload("res://Resources/Textures/PlayerTexture/base/lorc_female_4.png"),
	"lorc_female_5" : preload("res://Resources/Textures/PlayerTexture/base/lorc_female_5.png"),
	"lorc_female_6" : preload("res://Resources/Textures/PlayerTexture/base/lorc_female_6.png"),
	"lorc_male_0" : preload("res://Resources/Textures/PlayerTexture/base/lorc_male_0.png"),
	"lorc_male_1" : preload("res://Resources/Textures/PlayerTexture/base/lorc_male_1.png"),
	"lorc_male_2" : preload("res://Resources/Textures/PlayerTexture/base/lorc_male_2.png"),
	"lorc_male_3" : preload("res://Resources/Textures/PlayerTexture/base/lorc_male_3.png"),
	"lorc_male_4" : preload("res://Resources/Textures/PlayerTexture/base/lorc_male_4.png"),
	"lorc_male_5" : preload("res://Resources/Textures/PlayerTexture/base/lorc_male_5.png"),
	"lorc_male_6" : preload("res://Resources/Textures/PlayerTexture/base/lorc_male_6.png"),
	"merfolk_female" : preload("res://Resources/Textures/PlayerTexture/base/merfolk_female.png"),
	"merfolk_male" : preload("res://Resources/Textures/PlayerTexture/base/merfolk_male.png"),
	"merfolk_water_female" : preload("res://Resources/Textures/PlayerTexture/base/merfolk_water_female.png"),
	"merfolk_water_male" : preload("res://Resources/Textures/PlayerTexture/base/merfolk_water_male.png"),
	"minotaur_brown_1_male" : preload("res://Resources/Textures/PlayerTexture/base/minotaur_brown_1_male.png"),
	"minotaur_brown_2_male" : preload("res://Resources/Textures/PlayerTexture/base/minotaur_brown_2_male.png"),
	"minotaur_female" : preload("res://Resources/Textures/PlayerTexture/base/minotaur_female.png"),
	"minotaur_male" : preload("res://Resources/Textures/PlayerTexture/base/minotaur_male.png"),
	"mummy_female" : preload("res://Resources/Textures/PlayerTexture/base/mummy_female.png"),
	"mummy_male" : preload("res://Resources/Textures/PlayerTexture/base/mummy_male.png"),
	"naga_blue_female" : preload("res://Resources/Textures/PlayerTexture/base/naga_blue_female.png"),
	"naga_blue_male" : preload("res://Resources/Textures/PlayerTexture/base/naga_blue_male.png"),
	"naga_darkgreen_female" : preload("res://Resources/Textures/PlayerTexture/base/naga_darkgreen_female.png"),
	"naga_darkgreen_male" : preload("res://Resources/Textures/PlayerTexture/base/naga_darkgreen_male.png"),
	"naga_female" : preload("res://Resources/Textures/PlayerTexture/base/naga_female.png"),
	"naga_lightgreen_female" : preload("res://Resources/Textures/PlayerTexture/base/naga_lightgreen_female.png"),
	"naga_lightgreen_male" : preload("res://Resources/Textures/PlayerTexture/base/naga_lightgreen_male.png"),
	"naga_male" : preload("res://Resources/Textures/PlayerTexture/base/naga_male.png"),
	"naga_red_female" : preload("res://Resources/Textures/PlayerTexture/base/naga_red_female.png"),
	"naga_red_male" : preload("res://Resources/Textures/PlayerTexture/base/naga_red_male.png"),
	"octopode_1" : preload("res://Resources/Textures/PlayerTexture/base/octopode_1.png"),
	"octopode_2" : preload("res://Resources/Textures/PlayerTexture/base/octopode_2.png"),
	"octopode_3" : preload("res://Resources/Textures/PlayerTexture/base/octopode_3.png"),
	"octopode_4" : preload("res://Resources/Textures/PlayerTexture/base/octopode_4.png"),
	"octopode_5" : preload("res://Resources/Textures/PlayerTexture/base/octopode_5.png"),
	"ogre_female" : preload("res://Resources/Textures/PlayerTexture/base/ogre_female.png"),
	"ogre_male" : preload("res://Resources/Textures/PlayerTexture/base/ogre_male.png"),
	"orc_female" : preload("res://Resources/Textures/PlayerTexture/base/orc_female.png"),
	"orc_male" : preload("res://Resources/Textures/PlayerTexture/base/orc_male.png"),
	"shadow" : preload("res://Resources/Textures/PlayerTexture/base/shadow.png"),
	"spriggan_female" : preload("res://Resources/Textures/PlayerTexture/base/spriggan_female.png"),
	"spriggan_male" : preload("res://Resources/Textures/PlayerTexture/base/spriggan_male.png"),
	"tengu_wingless_brown_female" : preload("res://Resources/Textures/PlayerTexture/base/tengu_wingless_brown_female.png"),
	"tengu_wingless_brown_male" : preload("res://Resources/Textures/PlayerTexture/base/tengu_wingless_brown_male.png"),
	"troll_female" : preload("res://Resources/Textures/PlayerTexture/base/troll_female.png"),
	"troll_male" : preload("res://Resources/Textures/PlayerTexture/base/troll_male.png"),
	"vampire_female" : preload("res://Resources/Textures/PlayerTexture/base/vampire_female.png"),
	"vampire_male" : preload("res://Resources/Textures/PlayerTexture/base/vampire_male.png"),
	"zdeep_dwarf_female" : preload("res://Resources/Textures/PlayerTexture/base/zdeep_dwarf_female.png"),
	"zdeep_dwarf_male" : preload("res://Resources/Textures/PlayerTexture/base/zdeep_dwarf_male.png"),
	"zdeep_elf_female" : preload("res://Resources/Textures/PlayerTexture/base/zdeep_elf_female.png"),
	"zdeep_elf_male" : preload("res://Resources/Textures/PlayerTexture/base/zdeep_elf_male.png")}


func getPlayerTexture_base(resourceName : String) :
	return PlayerTexture_baseDictionary.get(resourceName)

const PlayerTexture_beardDictionary = {
	"long_black" : preload("res://Resources/Textures/PlayerTexture/beard/long_black.png"),
	"long_green" : preload("res://Resources/Textures/PlayerTexture/beard/long_green.png"),
	"long_red" : preload("res://Resources/Textures/PlayerTexture/beard/long_red.png"),
	"long_white" : preload("res://Resources/Textures/PlayerTexture/beard/long_white.png"),
	"long_yellow" : preload("res://Resources/Textures/PlayerTexture/beard/long_yellow.png"),
	"pj" : preload("res://Resources/Textures/PlayerTexture/beard/pj.png"),
	"short_black" : preload("res://Resources/Textures/PlayerTexture/beard/short_black.png"),
	"short_green" : preload("res://Resources/Textures/PlayerTexture/beard/short_green.png"),
	"short_red" : preload("res://Resources/Textures/PlayerTexture/beard/short_red.png"),
	"short_white" : preload("res://Resources/Textures/PlayerTexture/beard/short_white.png"),
	"short_yellow" : preload("res://Resources/Textures/PlayerTexture/beard/short_yellow.png")}


func getPlayerTexture_beard(resourceName : String) :
	return PlayerTexture_beardDictionary.get(resourceName)

const PlayerTexture_bodyDictionary = {
	"animal_skin" : preload("res://Resources/Textures/PlayerTexture/body/animal_skin.png"),
	"aragorn" : preload("res://Resources/Textures/PlayerTexture/body/aragorn.png"),
	"aragorn_2" : preload("res://Resources/Textures/PlayerTexture/body/aragorn_2.png"),
	"armor_blue_gold" : preload("res://Resources/Textures/PlayerTexture/body/armor_blue_gold.png"),
	"armor_mummy" : preload("res://Resources/Textures/PlayerTexture/body/armor_mummy.png"),
	"arwen" : preload("res://Resources/Textures/PlayerTexture/body/arwen.png"),
	"banded" : preload("res://Resources/Textures/PlayerTexture/body/banded.png"),
	"banded_2" : preload("res://Resources/Textures/PlayerTexture/body/banded_2.png"),
	"belt_1" : preload("res://Resources/Textures/PlayerTexture/body/belt_1.png"),
	"belt_2" : preload("res://Resources/Textures/PlayerTexture/body/belt_2.png"),
	"bikini_red" : preload("res://Resources/Textures/PlayerTexture/body/bikini_red.png"),
	"bloody" : preload("res://Resources/Textures/PlayerTexture/body/bloody.png"),
	"boromir" : preload("res://Resources/Textures/PlayerTexture/body/boromir.png"),
	"bplate_green" : preload("res://Resources/Textures/PlayerTexture/body/bplate_green.png"),
	"bplate_metal_1" : preload("res://Resources/Textures/PlayerTexture/body/bplate_metal_1.png"),
	"breast_black" : preload("res://Resources/Textures/PlayerTexture/body/breast_black.png"),
	"chainmail" : preload("res://Resources/Textures/PlayerTexture/body/chainmail.png"),
	"chainmail_3" : preload("res://Resources/Textures/PlayerTexture/body/chainmail_3.png"),
	"china_red" : preload("res://Resources/Textures/PlayerTexture/body/china_red.png"),
	"china_red_2" : preload("res://Resources/Textures/PlayerTexture/body/china_red_2.png"),
	"chunli" : preload("res://Resources/Textures/PlayerTexture/body/chunli.png"),
	"coat_black" : preload("res://Resources/Textures/PlayerTexture/body/coat_black.png"),
	"coat_red" : preload("res://Resources/Textures/PlayerTexture/body/coat_red.png"),
	"crystal_plate" : preload("res://Resources/Textures/PlayerTexture/body/crystal_plate.png"),
	"dragon_armor_blue_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_blue_new.png"),
	"dragon_armor_blue_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_blue_old.png"),
	"dragon_armor_brown_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_brown_new.png"),
	"dragon_armor_brown_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_brown_old.png"),
	"dragon_armor_cyan_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_cyan_new.png"),
	"dragon_armor_cyan_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_cyan_old.png"),
	"dragon_armor_gold_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_gold_new.png"),
	"dragon_armor_gold_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_gold_old.png"),
	"dragon_armor_green" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_green.png"),
	"dragon_armor_magenta_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_magenta_new.png"),
	"dragon_armor_magenta_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_magenta_old.png"),
	"dragon_armor_pearl" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_pearl.png"),
	"dragon_armor_quicksilver" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_quicksilver.png"),
	"dragon_armor_shadow" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_shadow.png"),
	"dragon_armor_white_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_white_new.png"),
	"dragon_armor_white_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_armor_white_old.png"),
	"dragon_scale_blue_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_blue_new.png"),
	"dragon_scale_blue_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_blue_old.png"),
	"dragon_scale_brown_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_brown_new.png"),
	"dragon_scale_brown_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_brown_old.png"),
	"dragon_scale_cyan_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_cyan_new.png"),
	"dragon_scale_cyan_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_cyan_old.png"),
	"dragon_scale_gold_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_gold_new.png"),
	"dragon_scale_gold_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_gold_old.png"),
	"dragon_scale_green" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_green.png"),
	"dragon_scale_magenta_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_magenta_new.png"),
	"dragon_scale_magenta_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_magenta_old.png"),
	"dragon_scale_pearl" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_pearl.png"),
	"dragon_scale_quicksilver" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_quicksilver.png"),
	"dragon_scale_shadow" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_shadow.png"),
	"dragon_scale_white_new" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_white_new.png"),
	"dragon_scale_white_old" : preload("res://Resources/Textures/PlayerTexture/body/dragon_scale_white_old.png"),
	"dress_green" : preload("res://Resources/Textures/PlayerTexture/body/dress_green.png"),
	"dress_white" : preload("res://Resources/Textures/PlayerTexture/body/dress_white.png"),
	"faerie_dragon_armor" : preload("res://Resources/Textures/PlayerTexture/body/faerie_dragon_armor.png"),
	"frodo" : preload("res://Resources/Textures/PlayerTexture/body/frodo.png"),
	"gandalf_g" : preload("res://Resources/Textures/PlayerTexture/body/gandalf_g.png"),
	"gil-galad" : preload("res://Resources/Textures/PlayerTexture/body/gil-galad.png"),
	"gimli" : preload("res://Resources/Textures/PlayerTexture/body/gimli.png"),
	"green_chain" : preload("res://Resources/Textures/PlayerTexture/body/green_chain.png"),
	"green_susp" : preload("res://Resources/Textures/PlayerTexture/body/green_susp.png"),
	"half_plate" : preload("res://Resources/Textures/PlayerTexture/body/half_plate.png"),
	"half_plate_2" : preload("res://Resources/Textures/PlayerTexture/body/half_plate_2.png"),
	"half_plate_3" : preload("res://Resources/Textures/PlayerTexture/body/half_plate_3.png"),
	"isildur" : preload("res://Resources/Textures/PlayerTexture/body/isildur.png"),
	"jacket_2" : preload("res://Resources/Textures/PlayerTexture/body/jacket_2.png"),
	"jacket_3" : preload("res://Resources/Textures/PlayerTexture/body/jacket_3.png"),
	"jacket_stud" : preload("res://Resources/Textures/PlayerTexture/body/jacket_stud.png"),
	"jessica" : preload("res://Resources/Textures/PlayerTexture/body/jessica.png"),
	"karate" : preload("res://Resources/Textures/PlayerTexture/body/karate.png"),
	"karate_2" : preload("res://Resources/Textures/PlayerTexture/body/karate_2.png"),
	"lears_chain_mail" : preload("res://Resources/Textures/PlayerTexture/body/lears_chain_mail.png"),
	"leather_2" : preload("res://Resources/Textures/PlayerTexture/body/leather_2.png"),
	"leather_armor" : preload("res://Resources/Textures/PlayerTexture/body/leather_armor.png"),
	"leather_armor_2" : preload("res://Resources/Textures/PlayerTexture/body/leather_armor_2.png"),
	"leather_armor_3" : preload("res://Resources/Textures/PlayerTexture/body/leather_armor_3.png"),
	"leather_green" : preload("res://Resources/Textures/PlayerTexture/body/leather_green.png"),
	"leather_heavy" : preload("res://Resources/Textures/PlayerTexture/body/leather_heavy.png"),
	"leather_jacket" : preload("res://Resources/Textures/PlayerTexture/body/leather_jacket.png"),
	"leather_metal" : preload("res://Resources/Textures/PlayerTexture/body/leather_metal.png"),
	"leather_red" : preload("res://Resources/Textures/PlayerTexture/body/leather_red.png"),
	"leather_short" : preload("res://Resources/Textures/PlayerTexture/body/leather_short.png"),
	"leather_stud" : preload("res://Resources/Textures/PlayerTexture/body/leather_stud.png"),
	"legolas" : preload("res://Resources/Textures/PlayerTexture/body/legolas.png"),
	"maxwell_new" : preload("res://Resources/Textures/PlayerTexture/body/maxwell_new.png"),
	"maxwell_old" : preload("res://Resources/Textures/PlayerTexture/body/maxwell_old.png"),
	"merry" : preload("res://Resources/Textures/PlayerTexture/body/merry.png"),
	"mesh_black" : preload("res://Resources/Textures/PlayerTexture/body/mesh_black.png"),
	"mesh_red" : preload("res://Resources/Textures/PlayerTexture/body/mesh_red.png"),
	"metal_blue" : preload("res://Resources/Textures/PlayerTexture/body/metal_blue.png"),
	"monk_black" : preload("res://Resources/Textures/PlayerTexture/body/monk_black.png"),
	"monk_blue" : preload("res://Resources/Textures/PlayerTexture/body/monk_blue.png"),
	"neck" : preload("res://Resources/Textures/PlayerTexture/body/neck.png"),
	"orange_crystal" : preload("res://Resources/Textures/PlayerTexture/body/orange_crystal.png"),
	"pipin" : preload("res://Resources/Textures/PlayerTexture/body/pipin.png"),
	"pj" : preload("res://Resources/Textures/PlayerTexture/body/pj.png"),
	"plate" : preload("res://Resources/Textures/PlayerTexture/body/plate.png"),
	"plate_2" : preload("res://Resources/Textures/PlayerTexture/body/plate_2.png"),
	"plate_and_cloth" : preload("res://Resources/Textures/PlayerTexture/body/plate_and_cloth.png"),
	"plate_and_cloth_2" : preload("res://Resources/Textures/PlayerTexture/body/plate_and_cloth_2.png"),
	"plate_black" : preload("res://Resources/Textures/PlayerTexture/body/plate_black.png"),
	"ringmail" : preload("res://Resources/Textures/PlayerTexture/body/ringmail.png"),
	"robe_black" : preload("res://Resources/Textures/PlayerTexture/body/robe_black.png"),
	"robe_black_gold" : preload("res://Resources/Textures/PlayerTexture/body/robe_black_gold.png"),
	"robe_black_hood" : preload("res://Resources/Textures/PlayerTexture/body/robe_black_hood.png"),
	"robe_black_red" : preload("res://Resources/Textures/PlayerTexture/body/robe_black_red.png"),
	"robe_blue" : preload("res://Resources/Textures/PlayerTexture/body/robe_blue.png"),
	"robe_blue_green" : preload("res://Resources/Textures/PlayerTexture/body/robe_blue_green.png"),
	"robe_blue_white" : preload("res://Resources/Textures/PlayerTexture/body/robe_blue_white.png"),
	"robe_brown" : preload("res://Resources/Textures/PlayerTexture/body/robe_brown.png"),
	"robe_brown_2" : preload("res://Resources/Textures/PlayerTexture/body/robe_brown_2.png"),
	"robe_brown_3" : preload("res://Resources/Textures/PlayerTexture/body/robe_brown_3.png"),
	"robe_clouds" : preload("res://Resources/Textures/PlayerTexture/body/robe_clouds.png"),
	"robe_cyan" : preload("res://Resources/Textures/PlayerTexture/body/robe_cyan.png"),
	"robe_gray_2" : preload("res://Resources/Textures/PlayerTexture/body/robe_gray_2.png"),
	"robe_green" : preload("res://Resources/Textures/PlayerTexture/body/robe_green.png"),
	"robe_green_gold" : preload("res://Resources/Textures/PlayerTexture/body/robe_green_gold.png"),
	"robe_jester" : preload("res://Resources/Textures/PlayerTexture/body/robe_jester.png"),
	"robe_misfortune" : preload("res://Resources/Textures/PlayerTexture/body/robe_misfortune.png"),
	"robe_of_night" : preload("res://Resources/Textures/PlayerTexture/body/robe_of_night.png"),
	"robe_purple" : preload("res://Resources/Textures/PlayerTexture/body/robe_purple.png"),
	"robe_rainbow" : preload("res://Resources/Textures/PlayerTexture/body/robe_rainbow.png"),
	"robe_red" : preload("res://Resources/Textures/PlayerTexture/body/robe_red.png"),
	"robe_red_2" : preload("res://Resources/Textures/PlayerTexture/body/robe_red_2.png"),
	"robe_red_3" : preload("res://Resources/Textures/PlayerTexture/body/robe_red_3.png"),
	"robe_red_gold" : preload("res://Resources/Textures/PlayerTexture/body/robe_red_gold.png"),
	"robe_white" : preload("res://Resources/Textures/PlayerTexture/body/robe_white.png"),
	"robe_white_2" : preload("res://Resources/Textures/PlayerTexture/body/robe_white_2.png"),
	"robe_white_blue" : preload("res://Resources/Textures/PlayerTexture/body/robe_white_blue.png"),
	"robe_white_green" : preload("res://Resources/Textures/PlayerTexture/body/robe_white_green.png"),
	"robe_white_red" : preload("res://Resources/Textures/PlayerTexture/body/robe_white_red.png"),
	"robe_yellow" : preload("res://Resources/Textures/PlayerTexture/body/robe_yellow.png"),
	"sam" : preload("res://Resources/Textures/PlayerTexture/body/sam.png"),
	"saruman" : preload("res://Resources/Textures/PlayerTexture/body/saruman.png"),
	"scalemail" : preload("res://Resources/Textures/PlayerTexture/body/scalemail.png"),
	"scalemail_2" : preload("res://Resources/Textures/PlayerTexture/body/scalemail_2.png"),
	"shirt_black" : preload("res://Resources/Textures/PlayerTexture/body/shirt_black.png"),
	"shirt_black_3" : preload("res://Resources/Textures/PlayerTexture/body/shirt_black_3.png"),
	"shirt_black_and_cloth" : preload("res://Resources/Textures/PlayerTexture/body/shirt_black_and_cloth.png"),
	"shirt_blue" : preload("res://Resources/Textures/PlayerTexture/body/shirt_blue.png"),
	"shirt_check" : preload("res://Resources/Textures/PlayerTexture/body/shirt_check.png"),
	"shirt_hawaii" : preload("res://Resources/Textures/PlayerTexture/body/shirt_hawaii.png"),
	"shirt_vest" : preload("res://Resources/Textures/PlayerTexture/body/shirt_vest.png"),
	"shirt_white_1" : preload("res://Resources/Textures/PlayerTexture/body/shirt_white_1.png"),
	"shirt_white_2" : preload("res://Resources/Textures/PlayerTexture/body/shirt_white_2.png"),
	"shirt_white_3" : preload("res://Resources/Textures/PlayerTexture/body/shirt_white_3.png"),
	"shirt_white_yellow" : preload("res://Resources/Textures/PlayerTexture/body/shirt_white_yellow.png"),
	"shoulder_pad" : preload("res://Resources/Textures/PlayerTexture/body/shoulder_pad.png"),
	"skirt_onep_grey" : preload("res://Resources/Textures/PlayerTexture/body/skirt_onep_grey.png"),
	"slit_black" : preload("res://Resources/Textures/PlayerTexture/body/slit_black.png"),
	"susp_black" : preload("res://Resources/Textures/PlayerTexture/body/susp_black.png"),
	"troll_hide" : preload("res://Resources/Textures/PlayerTexture/body/troll_hide.png"),
	"vanhel_1" : preload("res://Resources/Textures/PlayerTexture/body/vanhel_1.png"),
	"vest_red" : preload("res://Resources/Textures/PlayerTexture/body/vest_red.png"),
	"vest_red_2" : preload("res://Resources/Textures/PlayerTexture/body/vest_red_2.png"),
	"zhor" : preload("res://Resources/Textures/PlayerTexture/body/zhor.png")}


func getPlayerTexture_body(resourceName : String) :
	return PlayerTexture_bodyDictionary.get(resourceName)

const PlayerTexture_bootsDictionary = {
	"blue_gold" : preload("res://Resources/Textures/PlayerTexture/boots/blue_gold.png"),
	"hooves" : preload("res://Resources/Textures/PlayerTexture/boots/hooves.png"),
	"long_red" : preload("res://Resources/Textures/PlayerTexture/boots/long_red.png"),
	"long_white" : preload("res://Resources/Textures/PlayerTexture/boots/long_white.png"),
	"mesh_black" : preload("res://Resources/Textures/PlayerTexture/boots/mesh_black.png"),
	"mesh_blue" : preload("res://Resources/Textures/PlayerTexture/boots/mesh_blue.png"),
	"mesh_red" : preload("res://Resources/Textures/PlayerTexture/boots/mesh_red.png"),
	"mesh_white" : preload("res://Resources/Textures/PlayerTexture/boots/mesh_white.png"),
	"middle_brown" : preload("res://Resources/Textures/PlayerTexture/boots/middle_brown.png"),
	"middle_brown_2" : preload("res://Resources/Textures/PlayerTexture/boots/middle_brown_2.png"),
	"middle_brown_3" : preload("res://Resources/Textures/PlayerTexture/boots/middle_brown_3.png"),
	"middle_gold" : preload("res://Resources/Textures/PlayerTexture/boots/middle_gold.png"),
	"middle_gray" : preload("res://Resources/Textures/PlayerTexture/boots/middle_gray.png"),
	"middle_green" : preload("res://Resources/Textures/PlayerTexture/boots/middle_green.png"),
	"middle_purple" : preload("res://Resources/Textures/PlayerTexture/boots/middle_purple.png"),
	"middle_ybrown" : preload("res://Resources/Textures/PlayerTexture/boots/middle_ybrown.png"),
	"pj" : preload("res://Resources/Textures/PlayerTexture/boots/pj.png"),
	"short_brown" : preload("res://Resources/Textures/PlayerTexture/boots/short_brown.png"),
	"short_brown_2" : preload("res://Resources/Textures/PlayerTexture/boots/short_brown_2.png"),
	"short_purple" : preload("res://Resources/Textures/PlayerTexture/boots/short_purple.png"),
	"short_red" : preload("res://Resources/Textures/PlayerTexture/boots/short_red.png"),
	"spider" : preload("res://Resources/Textures/PlayerTexture/boots/spider.png")}


func getPlayerTexture_boots(resourceName : String) :
	return PlayerTexture_bootsDictionary.get(resourceName)

const PlayerTexture_cloakDictionary = {
	"black" : preload("res://Resources/Textures/PlayerTexture/cloak/black.png"),
	"blue" : preload("res://Resources/Textures/PlayerTexture/cloak/blue.png"),
	"brown" : preload("res://Resources/Textures/PlayerTexture/cloak/brown.png"),
	"cyan" : preload("res://Resources/Textures/PlayerTexture/cloak/cyan.png"),
	"dragonskin" : preload("res://Resources/Textures/PlayerTexture/cloak/dragonskin.png"),
	"gray" : preload("res://Resources/Textures/PlayerTexture/cloak/gray.png"),
	"green" : preload("res://Resources/Textures/PlayerTexture/cloak/green.png"),
	"magenta" : preload("res://Resources/Textures/PlayerTexture/cloak/magenta.png"),
	"ratskin" : preload("res://Resources/Textures/PlayerTexture/cloak/ratskin.png"),
	"red" : preload("res://Resources/Textures/PlayerTexture/cloak/red.png"),
	"white" : preload("res://Resources/Textures/PlayerTexture/cloak/white.png"),
	"yellow" : preload("res://Resources/Textures/PlayerTexture/cloak/yellow.png")}


func getPlayerTexture_cloak(resourceName : String) :
	return PlayerTexture_cloakDictionary.get(resourceName)

const PlayerTexture_draconic_headDictionary = {
	"draconic_head_black" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_black.png"),
	"draconic_head_brown" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_brown.png"),
	"draconic_head_green" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_green.png"),
	"draconic_head_grey" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_grey.png"),
	"draconic_head_mottled" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_mottled.png"),
	"draconic_head_pale" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_pale.png"),
	"draconic_head_purple" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_purple.png"),
	"draconic_head_red" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_red.png"),
	"draconic_head_white" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_white.png"),
	"draconic_head_yellow" : preload("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_yellow.png")}


func getPlayerTexture_draconic_head(resourceName : String) :
	return PlayerTexture_draconic_headDictionary.get(resourceName)

const PlayerTexture_draconic_wingDictionary = {
	"draconic_wing_black" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_black.png"),
	"draconic_wing_brown" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_brown.png"),
	"draconic_wing_green" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_green.png"),
	"draconic_wing_grey" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_grey.png"),
	"draconic_wing_mottled" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_mottled.png"),
	"draconic_wing_pale" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_pale.png"),
	"draconic_wing_purple" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_purple.png"),
	"draconic_wing_red" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_red.png"),
	"draconic_wing_white" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_white.png"),
	"draconic_wing_yellow" : preload("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_yellow.png")}


func getPlayerTexture_draconic_wing(resourceName : String) :
	return PlayerTexture_draconic_wingDictionary.get(resourceName)

const PlayerTexture_enchantmentDictionary = {
	"sticky_flame" : preload("res://Resources/Textures/PlayerTexture/enchantment/sticky_flame.png")}


func getPlayerTexture_enchantment(resourceName : String) :
	return PlayerTexture_enchantmentDictionary.get(resourceName)

const PlayerTexture_felidsDictionary = {
	"cat_1" : preload("res://Resources/Textures/PlayerTexture/felids/cat_1.png"),
	"cat_2" : preload("res://Resources/Textures/PlayerTexture/felids/cat_2.png"),
	"cat_3" : preload("res://Resources/Textures/PlayerTexture/felids/cat_3.png"),
	"cat_4" : preload("res://Resources/Textures/PlayerTexture/felids/cat_4.png"),
	"cat_5" : preload("res://Resources/Textures/PlayerTexture/felids/cat_5.png")}


func getPlayerTexture_felids(resourceName : String) :
	return PlayerTexture_felidsDictionary.get(resourceName)

const PlayerTexture_glovesDictionary = {
	"claws" : preload("res://Resources/Textures/PlayerTexture/gloves/claws.png"),
	"gauntlet_blue" : preload("res://Resources/Textures/PlayerTexture/gloves/gauntlet_blue.png"),
	"glove_black" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_black.png"),
	"glove_black_2" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_black_2.png"),
	"glove_blue" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_blue.png"),
	"glove_brown" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_brown.png"),
	"glove_chunli" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_chunli.png"),
	"glove_gold" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_gold.png"),
	"glove_gray" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_gray.png"),
	"glove_grayfist" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_grayfist.png"),
	"glove_orange" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_orange.png"),
	"glove_purple" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_purple.png"),
	"glove_red" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_red.png"),
	"glove_short_blue" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_short_blue.png"),
	"glove_short_gray" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_short_gray.png"),
	"glove_short_green" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_short_green.png"),
	"glove_short_red" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_short_red.png"),
	"glove_short_white" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_short_white.png"),
	"glove_short_yellow" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_short_yellow.png"),
	"glove_white" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_white.png"),
	"glove_wrist_purple" : preload("res://Resources/Textures/PlayerTexture/gloves/glove_wrist_purple.png")}


func getPlayerTexture_gloves(resourceName : String) :
	return PlayerTexture_glovesDictionary.get(resourceName)

const PlayerTexture_hairDictionary = {
	"aragorn" : preload("res://Resources/Textures/PlayerTexture/hair/aragorn.png"),
	"arwen" : preload("res://Resources/Textures/PlayerTexture/hair/arwen.png"),
	"boromir" : preload("res://Resources/Textures/PlayerTexture/hair/boromir.png"),
	"brown_1" : preload("res://Resources/Textures/PlayerTexture/hair/brown_1.png"),
	"brown_2" : preload("res://Resources/Textures/PlayerTexture/hair/brown_2.png"),
	"djinn_1" : preload("res://Resources/Textures/PlayerTexture/hair/djinn_1.png"),
	"djinn_2" : preload("res://Resources/Textures/PlayerTexture/hair/djinn_2.png"),
	"elf_black" : preload("res://Resources/Textures/PlayerTexture/hair/elf_black.png"),
	"elf_red" : preload("res://Resources/Textures/PlayerTexture/hair/elf_red.png"),
	"elf_white" : preload("res://Resources/Textures/PlayerTexture/hair/elf_white.png"),
	"elf_yellow" : preload("res://Resources/Textures/PlayerTexture/hair/elf_yellow.png"),
	"fem_black" : preload("res://Resources/Textures/PlayerTexture/hair/fem_black.png"),
	"fem_red" : preload("res://Resources/Textures/PlayerTexture/hair/fem_red.png"),
	"fem_white" : preload("res://Resources/Textures/PlayerTexture/hair/fem_white.png"),
	"fem_yellow" : preload("res://Resources/Textures/PlayerTexture/hair/fem_yellow.png"),
	"frodo" : preload("res://Resources/Textures/PlayerTexture/hair/frodo.png"),
	"green" : preload("res://Resources/Textures/PlayerTexture/hair/green.png"),
	"knot_red" : preload("res://Resources/Textures/PlayerTexture/hair/knot_red.png"),
	"legolas" : preload("res://Resources/Textures/PlayerTexture/hair/legolas.png"),
	"long_black" : preload("res://Resources/Textures/PlayerTexture/hair/long_black.png"),
	"long_red" : preload("res://Resources/Textures/PlayerTexture/hair/long_red.png"),
	"long_white" : preload("res://Resources/Textures/PlayerTexture/hair/long_white.png"),
	"long_yellow" : preload("res://Resources/Textures/PlayerTexture/hair/long_yellow.png"),
	"merry" : preload("res://Resources/Textures/PlayerTexture/hair/merry.png"),
	"pigtail_red" : preload("res://Resources/Textures/PlayerTexture/hair/pigtail_red.png"),
	"pigtails_brown" : preload("res://Resources/Textures/PlayerTexture/hair/pigtails_brown.png"),
	"pigtails_yellow" : preload("res://Resources/Textures/PlayerTexture/hair/pigtails_yellow.png"),
	"pj" : preload("res://Resources/Textures/PlayerTexture/hair/pj.png"),
	"ponytail_yellow" : preload("res://Resources/Textures/PlayerTexture/hair/ponytail_yellow.png"),
	"sam" : preload("res://Resources/Textures/PlayerTexture/hair/sam.png"),
	"short_black" : preload("res://Resources/Textures/PlayerTexture/hair/short_black.png"),
	"short_red" : preload("res://Resources/Textures/PlayerTexture/hair/short_red.png"),
	"short_white" : preload("res://Resources/Textures/PlayerTexture/hair/short_white.png"),
	"short_yellow" : preload("res://Resources/Textures/PlayerTexture/hair/short_yellow.png"),
	"tengu_comb" : preload("res://Resources/Textures/PlayerTexture/hair/tengu_comb.png")}


func getPlayerTexture_hair(resourceName : String) :
	return PlayerTexture_hairDictionary.get(resourceName)

const PlayerTexture_haloDictionary = {
	"halo_player" : preload("res://Resources/Textures/PlayerTexture/halo/halo_player.png")}


func getPlayerTexture_halo(resourceName : String) :
	return PlayerTexture_haloDictionary.get(resourceName)

const PlayerTexture_hand_leftDictionary = {
	"book_black" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_black.png"),
	"book_blue" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_blue.png"),
	"book_blue_dim" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_blue_dim.png"),
	"book_cyan" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_cyan.png"),
	"book_cyan_dim" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_cyan_dim.png"),
	"book_green" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_green.png"),
	"book_green_dim" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_green_dim.png"),
	"book_magenta" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_magenta.png"),
	"book_magenta_dim" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_magenta_dim.png"),
	"book_red" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_red.png"),
	"book_red_dim" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_red_dim.png"),
	"book_sky" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_sky.png"),
	"book_white" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_white.png"),
	"book_yellow" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_yellow.png"),
	"book_yellow_dim" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/book_yellow_dim.png"),
	"dagger_new" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/dagger_new.png"),
	"dagger_old" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/dagger_old.png"),
	"fire_cyan" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_cyan.png"),
	"fire_dark" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_dark.png"),
	"fire_green" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_green.png"),
	"fire_white" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_white.png"),
	"fire_white_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_white_2.png"),
	"flail_great" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/flail_great.png"),
	"flail_great_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/flail_great_2.png"),
	"giant_club" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club.png"),
	"giant_club_plain" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club_plain.png"),
	"giant_club_slant" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club_slant.png"),
	"giant_club_spike" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club_spike.png"),
	"giant_club_spike_slant" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club_spike_slant.png"),
	"great_mace" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/great_mace.png"),
	"great_mace_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/great_mace_2.png"),
	"lantern" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/lantern.png"),
	"light_blue" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/light_blue.png"),
	"light_red" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/light_red.png"),
	"light_yellow" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/light_yellow.png"),
	"pj" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/pj.png"),
	"rapier_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/rapier_2.png"),
	"sabre" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/sabre.png"),
	"short_sword_slant_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/short_sword_slant_2.png"),
	"short_sword_slant_new" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/short_sword_slant_new.png"),
	"short_sword_slant_old" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/short_sword_slant_old.png"),
	"spark" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/spark.png"),
	"torch" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/torch.png"),
	"torch_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/misc/torch_2.png"),
	"boromir" : preload("res://Resources/Textures/PlayerTexture/hand_left/boromir.png"),
	"buckler_green" : preload("res://Resources/Textures/PlayerTexture/hand_left/buckler_green.png"),
	"buckler_rb" : preload("res://Resources/Textures/PlayerTexture/hand_left/buckler_rb.png"),
	"buckler_round_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/buckler_round_2.png"),
	"buckler_round_3" : preload("res://Resources/Textures/PlayerTexture/hand_left/buckler_round_3.png"),
	"buckler_spiral" : preload("res://Resources/Textures/PlayerTexture/hand_left/buckler_spiral.png"),
	"bullseye" : preload("res://Resources/Textures/PlayerTexture/hand_left/bullseye.png"),
	"gil-galad" : preload("res://Resources/Textures/PlayerTexture/hand_left/gil-galad.png"),
	"gong" : preload("res://Resources/Textures/PlayerTexture/hand_left/gong.png"),
	"lshield_gold" : preload("res://Resources/Textures/PlayerTexture/hand_left/lshield_gold.png"),
	"lshield_green" : preload("res://Resources/Textures/PlayerTexture/hand_left/lshield_green.png"),
	"lshield_long_red" : preload("res://Resources/Textures/PlayerTexture/hand_left/lshield_long_red.png"),
	"lshield_louise" : preload("res://Resources/Textures/PlayerTexture/hand_left/lshield_louise.png"),
	"lshield_quartered" : preload("res://Resources/Textures/PlayerTexture/hand_left/lshield_quartered.png"),
	"lshield_spiral" : preload("res://Resources/Textures/PlayerTexture/hand_left/lshield_spiral.png"),
	"lshield_teal" : preload("res://Resources/Textures/PlayerTexture/hand_left/lshield_teal.png"),
	"shield_dd" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_dd.png"),
	"shield_dd_scion" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_dd_scion.png"),
	"shield_diamond_yellow" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_diamond_yellow.png"),
	"shield_donald" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_donald.png"),
	"shield_draconic_knight" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_draconic_knight.png"),
	"shield_goblin" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_goblin.png"),
	"shield_holy" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_holy.png"),
	"shield_kite_1" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_kite_1.png"),
	"shield_kite_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_kite_2.png"),
	"shield_kite_3" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_kite_3.png"),
	"shield_kite_4" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_kite_4.png"),
	"shield_knight_blue" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_knight_blue.png"),
	"shield_knight_gray" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_knight_gray.png"),
	"shield_knight_rw" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_knight_rw.png"),
	"shield_large_dd_dk" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_large_dd_dk.png"),
	"shield_long_cross" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_long_cross.png"),
	"shield_long_red" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_long_red.png"),
	"shield_middle_black" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_black.png"),
	"shield_middle_brown" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_brown.png"),
	"shield_middle_cyan" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_cyan.png"),
	"shield_middle_ethn" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_ethn.png"),
	"shield_middle_gray" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_gray.png"),
	"shield_middle_round" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_round.png"),
	"shield_middle_unicorn" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_unicorn.png"),
	"shield_of_ignorance" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_of_ignorance.png"),
	"shield_of_resistance" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_of_resistance.png"),
	"shield_round_1" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_1.png"),
	"shield_round_2" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_2.png"),
	"shield_round_3" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_3.png"),
	"shield_round_4" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_4.png"),
	"shield_round_5" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_5.png"),
	"shield_round_6" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_6.png"),
	"shield_round_7" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_7.png"),
	"shield_round_small" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_small.png"),
	"shield_round_white" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_round_white.png"),
	"shield_shaman" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_shaman.png"),
	"shield_skull" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_skull.png"),
	"shield_spriggan" : preload("res://Resources/Textures/PlayerTexture/hand_left/shield_spriggan.png")}


func getPlayerTexture_hand_left(resourceName : String) :
	return PlayerTexture_hand_leftDictionary.get(resourceName)

const PlayerTexture_hand_rightDictionary = {
	"arc_blade" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/arc_blade.png"),
	"arga_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/arga_new.png"),
	"arga_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/arga_old.png"),
	"asmodeus_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/asmodeus_new.png"),
	"asmodeus_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/asmodeus_old.png"),
	"axe_of_woe" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/axe_of_woe.png"),
	"axe_trog" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/axe_trog.png"),
	"bloodbane_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/bloodbane_new.png"),
	"bloodbane_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/bloodbane_old.png"),
	"blowgun_assassin" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/blowgun_assassin.png"),
	"botono" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/botono.png"),
	"chilly_death_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/chilly_death_new.png"),
	"chilly_death_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/chilly_death_old.png"),
	"crossbow_fire" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/crossbow_fire.png"),
	"crystal_spear_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/crystal_spear_new.png"),
	"crystal_spear_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/crystal_spear_old.png"),
	"cutlass" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/cutlass.png"),
	"dire_lajatang" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/dire_lajatang.png"),
	"dispater_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/dispater_new.png"),
	"dispater_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/dispater_old.png"),
	"doom_knight_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/doom_knight_new.png"),
	"doom_knight_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/doom_knight_old.png"),
	"elemental_staff" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/elemental_staff.png"),
	"eos" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/eos.png"),
	"finisher" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/finisher.png"),
	"firestarter" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/firestarter.png"),
	"flaming_death_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/flaming_death_new.png"),
	"flaming_death_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/flaming_death_old.png"),
	"glaive_of_prune_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/glaive_of_prune_new.png"),
	"glaive_of_prune_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/glaive_of_prune_old.png"),
	"glaive_of_the_guard_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/glaive_of_the_guard_new.png"),
	"glaive_of_the_guard_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/glaive_of_the_guard_old.png"),
	"gyre" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/gyre.png"),
	"jihad" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/jihad.png"),
	"knife_of_accuracy" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/knife_of_accuracy.png"),
	"krishna" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/krishna.png"),
	"leech" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/leech.png"),
	"mace_of_brilliance" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/mace_of_brilliance.png"),
	"mace_of_variability" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/mace_of_variability.png"),
	"majin" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/majin.png"),
	"morg" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/morg.png"),
	"olgreb" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/olgreb.png"),
	"order" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/order.png"),
	"plutonium_sword_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/plutonium_sword_new.png"),
	"plutonium_sword_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/plutonium_sword_old.png"),
	"punk" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/punk.png"),
	"serpent_scourge" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/serpent_scourge.png"),
	"shillelagh" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/shillelagh.png"),
	"singing_sword" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/singing_sword.png"),
	"sniper" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/sniper.png"),
	"spriggans_knife_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/spriggans_knife_new.png"),
	"spriggans_knife_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/spriggans_knife_old.png"),
	"sword_of_power_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/sword_of_power_new.png"),
	"sword_of_power_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/sword_of_power_old.png"),
	"trident_octopus_king" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/trident_octopus_king.png"),
	"undeadhunter" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/undeadhunter.png"),
	"vampires_tooth" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/vampires_tooth.png"),
	"wucad_mu" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/wucad_mu.png"),
	"wyrmbane" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/wyrmbane.png"),
	"zonguldrok" : preload("res://Resources/Textures/PlayerTexture/hand_right/artefact/zonguldrok.png"),
	"bladehands_fe" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/bladehands_fe.png"),
	"bladehands_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/bladehands_new.png"),
	"bladehands_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/bladehands_old.png"),
	"bladehands_op" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/bladehands_op.png"),
	"bone_lantern" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/bone_lantern.png"),
	"bottle" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/bottle.png"),
	"box" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/box.png"),
	"crystal" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/crystal.png"),
	"deck" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/deck.png"),
	"disc" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/disc.png"),
	"fan" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/fan.png"),
	"fire_blue" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_blue.png"),
	"fire_cyan" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_cyan.png"),
	"fire_dark" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_dark.png"),
	"fire_green" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_green.png"),
	"fire_red" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_red.png"),
	"fire_white" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_white.png"),
	"fire_white_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_white_2.png"),
	"head" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/head.png"),
	"horn" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/horn.png"),
	"lantern" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/lantern.png"),
	"light_blue" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/light_blue.png"),
	"light_red" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/light_red.png"),
	"light_yellow" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/light_yellow.png"),
	"orb" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/orb.png"),
	"skull" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/skull.png"),
	"spark" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/spark.png"),
	"stone" : preload("res://Resources/Textures/PlayerTexture/hand_right/misc/stone.png"),
	"aragorn" : preload("res://Resources/Textures/PlayerTexture/hand_right/aragorn.png"),
	"arwen" : preload("res://Resources/Textures/PlayerTexture/hand_right/arwen.png"),
	"axe" : preload("res://Resources/Textures/PlayerTexture/hand_right/axe.png"),
	"axe_blood" : preload("res://Resources/Textures/PlayerTexture/hand_right/axe_blood.png"),
	"axe_double" : preload("res://Resources/Textures/PlayerTexture/hand_right/axe_double.png"),
	"axe_executioner_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/axe_executioner_2.png"),
	"axe_executioner_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/axe_executioner_new.png"),
	"axe_executioner_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/axe_executioner_old.png"),
	"axe_short" : preload("res://Resources/Textures/PlayerTexture/hand_right/axe_short.png"),
	"axe_small" : preload("res://Resources/Textures/PlayerTexture/hand_right/axe_small.png"),
	"battleaxe" : preload("res://Resources/Textures/PlayerTexture/hand_right/battleaxe.png"),
	"battleaxe_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/battleaxe_2.png"),
	"black_sword" : preload("res://Resources/Textures/PlayerTexture/hand_right/black_sword.png"),
	"black_whip_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/black_whip_new.png"),
	"black_whip_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/black_whip_old.png"),
	"blessed_blade" : preload("res://Resources/Textures/PlayerTexture/hand_right/blessed_blade.png"),
	"blowgun" : preload("res://Resources/Textures/PlayerTexture/hand_right/blowgun.png"),
	"boromir" : preload("res://Resources/Textures/PlayerTexture/hand_right/boromir.png"),
	"bow" : preload("res://Resources/Textures/PlayerTexture/hand_right/bow.png"),
	"bow_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/bow_2.png"),
	"bow_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/bow_3.png"),
	"bow_blue" : preload("res://Resources/Textures/PlayerTexture/hand_right/bow_blue.png"),
	"broad_axe" : preload("res://Resources/Textures/PlayerTexture/hand_right/broad_axe.png"),
	"broadsword" : preload("res://Resources/Textures/PlayerTexture/hand_right/broadsword.png"),
	"club" : preload("res://Resources/Textures/PlayerTexture/hand_right/club.png"),
	"club_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/club_2.png"),
	"club_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/club_3.png"),
	"club_slant" : preload("res://Resources/Textures/PlayerTexture/hand_right/club_slant.png"),
	"crossbow" : preload("res://Resources/Textures/PlayerTexture/hand_right/crossbow.png"),
	"crossbow_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/crossbow_2.png"),
	"crossbow_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/crossbow_3.png"),
	"crossbow_4" : preload("res://Resources/Textures/PlayerTexture/hand_right/crossbow_4.png"),
	"d_glaive" : preload("res://Resources/Textures/PlayerTexture/hand_right/d_glaive.png"),
	"dagger_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/dagger_new.png"),
	"dagger_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/dagger_old.png"),
	"dagger_slant_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/dagger_slant_2.png"),
	"dagger_slant_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/dagger_slant_new.png"),
	"dagger_slant_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/dagger_slant_old.png"),
	"dart" : preload("res://Resources/Textures/PlayerTexture/hand_right/dart.png"),
	"double_sword_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/double_sword_2.png"),
	"double_sword_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/double_sword_3.png"),
	"double_sword_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/double_sword_new.png"),
	"double_sword_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/double_sword_old.png"),
	"enchantress_dagger" : preload("res://Resources/Textures/PlayerTexture/hand_right/enchantress_dagger.png"),
	"eveningstar_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/eveningstar_2.png"),
	"eveningstar_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/eveningstar_new.png"),
	"eveningstar_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/eveningstar_old.png"),
	"falchion_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/falchion_2.png"),
	"falchion_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/falchion_new.png"),
	"falchion_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/falchion_old.png"),
	"flail_ball_2_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_2_new.png"),
	"flail_ball_2_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_2_old.png"),
	"flail_ball_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_3.png"),
	"flail_ball_4" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_4.png"),
	"flail_ball_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_new.png"),
	"flail_ball_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_old.png"),
	"flail_balls" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_balls.png"),
	"flail_great" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_great.png"),
	"flail_great_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_great_2.png"),
	"flail_spike" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_spike.png"),
	"flail_spike_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_spike_2.png"),
	"flail_stick" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_stick.png"),
	"flail_stick_slant" : preload("res://Resources/Textures/PlayerTexture/hand_right/flail_stick_slant.png"),
	"fork_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/fork_2.png"),
	"frodo" : preload("res://Resources/Textures/PlayerTexture/hand_right/frodo.png"),
	"gandalf" : preload("res://Resources/Textures/PlayerTexture/hand_right/gandalf.png"),
	"giant_club" : preload("res://Resources/Textures/PlayerTexture/hand_right/giant_club.png"),
	"giant_club_plain" : preload("res://Resources/Textures/PlayerTexture/hand_right/giant_club_plain.png"),
	"giant_club_slant" : preload("res://Resources/Textures/PlayerTexture/hand_right/giant_club_slant.png"),
	"giant_club_spike" : preload("res://Resources/Textures/PlayerTexture/hand_right/giant_club_spike.png"),
	"giant_club_spike_slant" : preload("res://Resources/Textures/PlayerTexture/hand_right/giant_club_spike_slant.png"),
	"gimli" : preload("res://Resources/Textures/PlayerTexture/hand_right/gimli.png"),
	"glaive_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/glaive_2.png"),
	"glaive_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/glaive_3.png"),
	"glaive_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/glaive_new.png"),
	"glaive_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/glaive_old.png"),
	"glaive_three" : preload("res://Resources/Textures/PlayerTexture/hand_right/glaive_three.png"),
	"glaive_three_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/glaive_three_2.png"),
	"great_axe" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_axe.png"),
	"great_bow" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_bow.png"),
	"great_mace" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_mace.png"),
	"great_mace_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_mace_2.png"),
	"great_staff" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_staff.png"),
	"great_sword" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_sword.png"),
	"great_sword_slant_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_sword_slant_2.png"),
	"great_sword_slant_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_sword_slant_new.png"),
	"great_sword_slant_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/great_sword_slant_old.png"),
	"greatsling" : preload("res://Resources/Textures/PlayerTexture/hand_right/greatsling.png"),
	"halberd_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/halberd_new.png"),
	"halberd_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/halberd_old.png"),
	"hammer_2_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/hammer_2_new.png"),
	"hammer_2_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/hammer_2_old.png"),
	"hammer_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/hammer_3.png"),
	"hammer_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/hammer_new.png"),
	"hammer_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/hammer_old.png"),
	"hand_axe_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/hand_axe_2.png"),
	"hand_axe_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/hand_axe_new.png"),
	"hand_axe_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/hand_axe_old.png"),
	"hand_crossbow" : preload("res://Resources/Textures/PlayerTexture/hand_right/hand_crossbow.png"),
	"heavy_sword" : preload("res://Resources/Textures/PlayerTexture/hand_right/heavy_sword.png"),
	"holy_scourge_1" : preload("res://Resources/Textures/PlayerTexture/hand_right/holy_scourge_1.png"),
	"holy_scourge_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/holy_scourge_2.png"),
	"hook" : preload("res://Resources/Textures/PlayerTexture/hand_right/hook.png"),
	"katana" : preload("res://Resources/Textures/PlayerTexture/hand_right/katana.png"),
	"katana_slant" : preload("res://Resources/Textures/PlayerTexture/hand_right/katana_slant.png"),
	"knife" : preload("res://Resources/Textures/PlayerTexture/hand_right/knife.png"),
	"lance" : preload("res://Resources/Textures/PlayerTexture/hand_right/lance.png"),
	"lance_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/lance_2.png"),
	"large_mace" : preload("res://Resources/Textures/PlayerTexture/hand_right/large_mace.png"),
	"legolas" : preload("res://Resources/Textures/PlayerTexture/hand_right/legolas.png"),
	"long_sword" : preload("res://Resources/Textures/PlayerTexture/hand_right/long_sword.png"),
	"long_sword_slant_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/long_sword_slant_2.png"),
	"long_sword_slant_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/long_sword_slant_new.png"),
	"long_sword_slant_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/long_sword_slant_old.png"),
	"mace_2_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/mace_2_new.png"),
	"mace_2_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/mace_2_old.png"),
	"mace_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/mace_3.png"),
	"mace_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/mace_new.png"),
	"mace_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/mace_old.png"),
	"mace_ruby_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/mace_ruby_new.png"),
	"mace_ruby_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/mace_ruby_old.png"),
	"morningstar_2_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/morningstar_2_new.png"),
	"morningstar_2_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/morningstar_2_old.png"),
	"morningstar_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/morningstar_new.png"),
	"morningstar_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/morningstar_old.png"),
	"nunchaku" : preload("res://Resources/Textures/PlayerTexture/hand_right/nunchaku.png"),
	"pick_axe" : preload("res://Resources/Textures/PlayerTexture/hand_right/pick_axe.png"),
	"pike" : preload("res://Resources/Textures/PlayerTexture/hand_right/pike.png"),
	"pole_forked" : preload("res://Resources/Textures/PlayerTexture/hand_right/pole_forked.png"),
	"quarterstaff" : preload("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff.png"),
	"quarterstaff_1" : preload("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_1.png"),
	"quarterstaff_2_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_2_new.png"),
	"quarterstaff_2_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_2_old.png"),
	"quarterstaff_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_3.png"),
	"quarterstaff_4" : preload("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_4.png"),
	"quarterstaff_jester" : preload("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_jester.png"),
	"rapier" : preload("res://Resources/Textures/PlayerTexture/hand_right/rapier.png"),
	"rapier_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/rapier_2.png"),
	"rod_aries_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_aries_new.png"),
	"rod_aries_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_aries_old.png"),
	"rod_blue_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_blue_new.png"),
	"rod_blue_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_blue_old.png"),
	"rod_brown_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_brown_new.png"),
	"rod_brown_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_brown_old.png"),
	"rod_emerald_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_emerald_new.png"),
	"rod_emerald_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_emerald_old.png"),
	"rod_forked_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_forked_new.png"),
	"rod_forked_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_forked_old.png"),
	"rod_hammer_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_hammer_new.png"),
	"rod_hammer_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_hammer_old.png"),
	"rod_magenta_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_magenta_new.png"),
	"rod_magenta_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_magenta_old.png"),
	"rod_moon_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_moon_new.png"),
	"rod_moon_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_moon_old.png"),
	"rod_ruby_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_ruby_new.png"),
	"rod_ruby_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_ruby_old.png"),
	"rod_thick_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_thick_new.png"),
	"rod_thick_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/rod_thick_old.png"),
	"sabre" : preload("res://Resources/Textures/PlayerTexture/hand_right/sabre.png"),
	"saruman" : preload("res://Resources/Textures/PlayerTexture/hand_right/saruman.png"),
	"scepter" : preload("res://Resources/Textures/PlayerTexture/hand_right/scepter.png"),
	"scimitar_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/scimitar_new.png"),
	"scimitar_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/scimitar_old.png"),
	"scythe_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/scythe_2.png"),
	"scythe_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/scythe_new.png"),
	"scythe_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/scythe_old.png"),
	"scythe_slant" : preload("res://Resources/Textures/PlayerTexture/hand_right/scythe_slant.png"),
	"short_sword" : preload("res://Resources/Textures/PlayerTexture/hand_right/short_sword.png"),
	"short_sword_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/short_sword_2.png"),
	"short_sword_slant_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/short_sword_slant_2.png"),
	"short_sword_slant_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/short_sword_slant_3.png"),
	"short_sword_slant_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/short_sword_slant_new.png"),
	"short_sword_slant_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/short_sword_slant_old.png"),
	"sickle" : preload("res://Resources/Textures/PlayerTexture/hand_right/sickle.png"),
	"sling" : preload("res://Resources/Textures/PlayerTexture/hand_right/sling.png"),
	"spear" : preload("res://Resources/Textures/PlayerTexture/hand_right/spear.png"),
	"spear_1" : preload("res://Resources/Textures/PlayerTexture/hand_right/spear_1.png"),
	"spear_2_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/spear_2_new.png"),
	"spear_2_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/spear_2_old.png"),
	"spear_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/spear_3.png"),
	"spear_4" : preload("res://Resources/Textures/PlayerTexture/hand_right/spear_4.png"),
	"spear_5" : preload("res://Resources/Textures/PlayerTexture/hand_right/spear_5.png"),
	"staff_evil" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_evil.png"),
	"staff_fancy" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_fancy.png"),
	"staff_fork" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_fork.png"),
	"staff_large" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_large.png"),
	"staff_mage" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_mage.png"),
	"staff_mage_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_mage_2.png"),
	"staff_mummy" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_mummy.png"),
	"staff_organic" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_organic.png"),
	"staff_plain" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_plain.png"),
	"staff_ring_blue" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_ring_blue.png"),
	"staff_ruby" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_ruby.png"),
	"staff_scepter" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_scepter.png"),
	"staff_skull" : preload("res://Resources/Textures/PlayerTexture/hand_right/staff_skull.png"),
	"stick" : preload("res://Resources/Textures/PlayerTexture/hand_right/stick.png"),
	"sword_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_2.png"),
	"sword_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_3.png"),
	"sword_black" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_black.png"),
	"sword_breaker" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_breaker.png"),
	"sword_jag" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_jag.png"),
	"sword_seven" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_seven.png"),
	"sword_thief" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_thief.png"),
	"sword_tri" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_tri.png"),
	"sword_twist" : preload("res://Resources/Textures/PlayerTexture/hand_right/sword_twist.png"),
	"trident" : preload("res://Resources/Textures/PlayerTexture/hand_right/trident.png"),
	"trident_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/trident_2.png"),
	"trident_3" : preload("res://Resources/Textures/PlayerTexture/hand_right/trident_3.png"),
	"trident_demon" : preload("res://Resources/Textures/PlayerTexture/hand_right/trident_demon.png"),
	"trident_elec" : preload("res://Resources/Textures/PlayerTexture/hand_right/trident_elec.png"),
	"trident_two" : preload("res://Resources/Textures/PlayerTexture/hand_right/trident_two.png"),
	"trident_two_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/trident_two_2.png"),
	"triple_sword_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/triple_sword_2.png"),
	"triple_sword_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/triple_sword_new.png"),
	"triple_sword_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/triple_sword_old.png"),
	"trishula" : preload("res://Resources/Textures/PlayerTexture/hand_right/trishula.png"),
	"war_axe_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/war_axe_new.png"),
	"war_axe_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/war_axe_old.png"),
	"whip_2" : preload("res://Resources/Textures/PlayerTexture/hand_right/whip_2.png"),
	"whip_new" : preload("res://Resources/Textures/PlayerTexture/hand_right/whip_new.png"),
	"whip_old" : preload("res://Resources/Textures/PlayerTexture/hand_right/whip_old.png")}


func getPlayerTexture_hand_right(resourceName : String) :
	return PlayerTexture_hand_rightDictionary.get(resourceName)

const PlayerTexture_headDictionary = {
	"art_dragonhelm" : preload("res://Resources/Textures/PlayerTexture/head/art_dragonhelm.png"),
	"band_blue" : preload("res://Resources/Textures/PlayerTexture/head/band_blue.png"),
	"band_magenta" : preload("res://Resources/Textures/PlayerTexture/head/band_magenta.png"),
	"band_red" : preload("res://Resources/Textures/PlayerTexture/head/band_red.png"),
	"band_white" : preload("res://Resources/Textures/PlayerTexture/head/band_white.png"),
	"band_yellow" : preload("res://Resources/Textures/PlayerTexture/head/band_yellow.png"),
	"bandana_ybrown" : preload("res://Resources/Textures/PlayerTexture/head/bandana_ybrown.png"),
	"bear" : preload("res://Resources/Textures/PlayerTexture/head/bear.png"),
	"black_horn" : preload("res://Resources/Textures/PlayerTexture/head/black_horn.png"),
	"black_horn_2" : preload("res://Resources/Textures/PlayerTexture/head/black_horn_2.png"),
	"blue_horn_gold" : preload("res://Resources/Textures/PlayerTexture/head/blue_horn_gold.png"),
	"brown_gold" : preload("res://Resources/Textures/PlayerTexture/head/brown_gold.png"),
	"cap_black_1" : preload("res://Resources/Textures/PlayerTexture/head/cap_black_1.png"),
	"cap_blue" : preload("res://Resources/Textures/PlayerTexture/head/cap_blue.png"),
	"chain" : preload("res://Resources/Textures/PlayerTexture/head/chain.png"),
	"cheek_red" : preload("res://Resources/Textures/PlayerTexture/head/cheek_red.png"),
	"clown_1" : preload("res://Resources/Textures/PlayerTexture/head/clown_1.png"),
	"clown_2" : preload("res://Resources/Textures/PlayerTexture/head/clown_2.png"),
	"cone_blue" : preload("res://Resources/Textures/PlayerTexture/head/cone_blue.png"),
	"cone_red" : preload("res://Resources/Textures/PlayerTexture/head/cone_red.png"),
	"crown_gold_1" : preload("res://Resources/Textures/PlayerTexture/head/crown_gold_1.png"),
	"crown_gold_2" : preload("res://Resources/Textures/PlayerTexture/head/crown_gold_2.png"),
	"crown_gold_3" : preload("res://Resources/Textures/PlayerTexture/head/crown_gold_3.png"),
	"dyrovepreva_new" : preload("res://Resources/Textures/PlayerTexture/head/dyrovepreva_new.png"),
	"dyrovepreva_old" : preload("res://Resources/Textures/PlayerTexture/head/dyrovepreva_old.png"),
	"eternal_torment" : preload("res://Resources/Textures/PlayerTexture/head/eternal_torment.png"),
	"etheric_cage" : preload("res://Resources/Textures/PlayerTexture/head/etheric_cage.png"),
	"feather_blue" : preload("res://Resources/Textures/PlayerTexture/head/feather_blue.png"),
	"feather_green" : preload("res://Resources/Textures/PlayerTexture/head/feather_green.png"),
	"feather_red" : preload("res://Resources/Textures/PlayerTexture/head/feather_red.png"),
	"feather_white" : preload("res://Resources/Textures/PlayerTexture/head/feather_white.png"),
	"feather_yellow" : preload("res://Resources/Textures/PlayerTexture/head/feather_yellow.png"),
	"fhelm_gray_3" : preload("res://Resources/Textures/PlayerTexture/head/fhelm_gray_3.png"),
	"fhelm_horn_2" : preload("res://Resources/Textures/PlayerTexture/head/fhelm_horn_2.png"),
	"fhelm_horn_yellow" : preload("res://Resources/Textures/PlayerTexture/head/fhelm_horn_yellow.png"),
	"full_black" : preload("res://Resources/Textures/PlayerTexture/head/full_black.png"),
	"full_gold" : preload("res://Resources/Textures/PlayerTexture/head/full_gold.png"),
	"gandalf" : preload("res://Resources/Textures/PlayerTexture/head/gandalf.png"),
	"hat_black" : preload("res://Resources/Textures/PlayerTexture/head/hat_black.png"),
	"healer" : preload("res://Resources/Textures/PlayerTexture/head/healer.png"),
	"helm_gimli" : preload("res://Resources/Textures/PlayerTexture/head/helm_gimli.png"),
	"helm_green" : preload("res://Resources/Textures/PlayerTexture/head/helm_green.png"),
	"helm_plume" : preload("res://Resources/Textures/PlayerTexture/head/helm_plume.png"),
	"helm_red" : preload("res://Resources/Textures/PlayerTexture/head/helm_red.png"),
	"hood_black_2" : preload("res://Resources/Textures/PlayerTexture/head/hood_black_2.png"),
	"hood_cyan" : preload("res://Resources/Textures/PlayerTexture/head/hood_cyan.png"),
	"hood_gray" : preload("res://Resources/Textures/PlayerTexture/head/hood_gray.png"),
	"hood_green" : preload("res://Resources/Textures/PlayerTexture/head/hood_green.png"),
	"hood_green_2" : preload("res://Resources/Textures/PlayerTexture/head/hood_green_2.png"),
	"hood_orange" : preload("res://Resources/Textures/PlayerTexture/head/hood_orange.png"),
	"hood_red" : preload("res://Resources/Textures/PlayerTexture/head/hood_red.png"),
	"hood_red_2" : preload("res://Resources/Textures/PlayerTexture/head/hood_red_2.png"),
	"hood_white" : preload("res://Resources/Textures/PlayerTexture/head/hood_white.png"),
	"hood_white_2" : preload("res://Resources/Textures/PlayerTexture/head/hood_white_2.png"),
	"hood_ybrown" : preload("res://Resources/Textures/PlayerTexture/head/hood_ybrown.png"),
	"horn_evil" : preload("res://Resources/Textures/PlayerTexture/head/horn_evil.png"),
	"horn_gray" : preload("res://Resources/Textures/PlayerTexture/head/horn_gray.png"),
	"horned" : preload("res://Resources/Textures/PlayerTexture/head/horned.png"),
	"horns_1" : preload("res://Resources/Textures/PlayerTexture/head/horns_1.png"),
	"horns_2" : preload("res://Resources/Textures/PlayerTexture/head/horns_2.png"),
	"horns_3" : preload("res://Resources/Textures/PlayerTexture/head/horns_3.png"),
	"iron_1" : preload("res://Resources/Textures/PlayerTexture/head/iron_1.png"),
	"iron_2" : preload("res://Resources/Textures/PlayerTexture/head/iron_2.png"),
	"iron_3" : preload("res://Resources/Textures/PlayerTexture/head/iron_3.png"),
	"iron_red" : preload("res://Resources/Textures/PlayerTexture/head/iron_red.png"),
	"isildur" : preload("res://Resources/Textures/PlayerTexture/head/isildur.png"),
	"mummy" : preload("res://Resources/Textures/PlayerTexture/head/mummy.png"),
	"ninja_black" : preload("res://Resources/Textures/PlayerTexture/head/ninja_black.png"),
	"straw" : preload("res://Resources/Textures/PlayerTexture/head/straw.png"),
	"taiso_blue" : preload("res://Resources/Textures/PlayerTexture/head/taiso_blue.png"),
	"taiso_magenta" : preload("res://Resources/Textures/PlayerTexture/head/taiso_magenta.png"),
	"taiso_red" : preload("res://Resources/Textures/PlayerTexture/head/taiso_red.png"),
	"taiso_white" : preload("res://Resources/Textures/PlayerTexture/head/taiso_white.png"),
	"taiso_yellow" : preload("res://Resources/Textures/PlayerTexture/head/taiso_yellow.png"),
	"turban_brown" : preload("res://Resources/Textures/PlayerTexture/head/turban_brown.png"),
	"turban_purple" : preload("res://Resources/Textures/PlayerTexture/head/turban_purple.png"),
	"turban_white" : preload("res://Resources/Textures/PlayerTexture/head/turban_white.png"),
	"viking_brown_1" : preload("res://Resources/Textures/PlayerTexture/head/viking_brown_1.png"),
	"viking_brown_2" : preload("res://Resources/Textures/PlayerTexture/head/viking_brown_2.png"),
	"viking_gold" : preload("res://Resources/Textures/PlayerTexture/head/viking_gold.png"),
	"wizard_blackgold" : preload("res://Resources/Textures/PlayerTexture/head/wizard_blackgold.png"),
	"wizard_blackred" : preload("res://Resources/Textures/PlayerTexture/head/wizard_blackred.png"),
	"wizard_blue" : preload("res://Resources/Textures/PlayerTexture/head/wizard_blue.png"),
	"wizard_bluegreen" : preload("res://Resources/Textures/PlayerTexture/head/wizard_bluegreen.png"),
	"wizard_brown" : preload("res://Resources/Textures/PlayerTexture/head/wizard_brown.png"),
	"wizard_darkgreen" : preload("res://Resources/Textures/PlayerTexture/head/wizard_darkgreen.png"),
	"wizard_lightgreen" : preload("res://Resources/Textures/PlayerTexture/head/wizard_lightgreen.png"),
	"wizard_purple" : preload("res://Resources/Textures/PlayerTexture/head/wizard_purple.png"),
	"wizard_red" : preload("res://Resources/Textures/PlayerTexture/head/wizard_red.png"),
	"wizard_white" : preload("res://Resources/Textures/PlayerTexture/head/wizard_white.png"),
	"yellow_wing" : preload("res://Resources/Textures/PlayerTexture/head/yellow_wing.png")}


func getPlayerTexture_head(resourceName : String) :
	return PlayerTexture_headDictionary.get(resourceName)

const PlayerTexture_legsDictionary = {
	"belt_gray" : preload("res://Resources/Textures/PlayerTexture/legs/belt_gray.png"),
	"belt_redbrown" : preload("res://Resources/Textures/PlayerTexture/legs/belt_redbrown.png"),
	"bikini_red" : preload("res://Resources/Textures/PlayerTexture/legs/bikini_red.png"),
	"chunli" : preload("res://Resources/Textures/PlayerTexture/legs/chunli.png"),
	"garter" : preload("res://Resources/Textures/PlayerTexture/legs/garter.png"),
	"leg_armor_0" : preload("res://Resources/Textures/PlayerTexture/legs/leg_armor_0.png"),
	"leg_armor_1" : preload("res://Resources/Textures/PlayerTexture/legs/leg_armor_1.png"),
	"leg_armor_2" : preload("res://Resources/Textures/PlayerTexture/legs/leg_armor_2.png"),
	"leg_armor_3" : preload("res://Resources/Textures/PlayerTexture/legs/leg_armor_3.png"),
	"leg_armor_4" : preload("res://Resources/Textures/PlayerTexture/legs/leg_armor_4.png"),
	"leg_armor_5" : preload("res://Resources/Textures/PlayerTexture/legs/leg_armor_5.png"),
	"loincloth_red" : preload("res://Resources/Textures/PlayerTexture/legs/loincloth_red.png"),
	"long_red" : preload("res://Resources/Textures/PlayerTexture/legs/long_red.png"),
	"metal_gray" : preload("res://Resources/Textures/PlayerTexture/legs/metal_gray.png"),
	"metal_green" : preload("res://Resources/Textures/PlayerTexture/legs/metal_green.png"),
	"pants_16" : preload("res://Resources/Textures/PlayerTexture/legs/pants_16.png"),
	"pants_black" : preload("res://Resources/Textures/PlayerTexture/legs/pants_black.png"),
	"pants_blue" : preload("res://Resources/Textures/PlayerTexture/legs/pants_blue.png"),
	"pants_brown" : preload("res://Resources/Textures/PlayerTexture/legs/pants_brown.png"),
	"pants_darkgreen" : preload("res://Resources/Textures/PlayerTexture/legs/pants_darkgreen.png"),
	"pants_l_white" : preload("res://Resources/Textures/PlayerTexture/legs/pants_l_white.png"),
	"pants_orange" : preload("res://Resources/Textures/PlayerTexture/legs/pants_orange.png"),
	"pants_red" : preload("res://Resources/Textures/PlayerTexture/legs/pants_red.png"),
	"pants_short_brown" : preload("res://Resources/Textures/PlayerTexture/legs/pants_short_brown.png"),
	"pants_short_brown_3" : preload("res://Resources/Textures/PlayerTexture/legs/pants_short_brown_3.png"),
	"pants_short_darkbrown" : preload("res://Resources/Textures/PlayerTexture/legs/pants_short_darkbrown.png"),
	"pants_short_gray" : preload("res://Resources/Textures/PlayerTexture/legs/pants_short_gray.png"),
	"pj" : preload("res://Resources/Textures/PlayerTexture/legs/pj.png"),
	"skirt_blue" : preload("res://Resources/Textures/PlayerTexture/legs/skirt_blue.png"),
	"skirt_green" : preload("res://Resources/Textures/PlayerTexture/legs/skirt_green.png"),
	"skirt_red" : preload("res://Resources/Textures/PlayerTexture/legs/skirt_red.png"),
	"skirt_white" : preload("res://Resources/Textures/PlayerTexture/legs/skirt_white.png"),
	"skirt_white_2" : preload("res://Resources/Textures/PlayerTexture/legs/skirt_white_2.png"),
	"trouser_green" : preload("res://Resources/Textures/PlayerTexture/legs/trouser_green.png")}


func getPlayerTexture_legs(resourceName : String) :
	return PlayerTexture_legsDictionary.get(resourceName)

const PlayerTexture_mutationsDictionary = {
	"cat_10" : preload("res://Resources/Textures/PlayerTexture/mutations/cat_10.png"),
	"cat_6" : preload("res://Resources/Textures/PlayerTexture/mutations/cat_6.png"),
	"cat_7" : preload("res://Resources/Textures/PlayerTexture/mutations/cat_7.png"),
	"cat_8" : preload("res://Resources/Textures/PlayerTexture/mutations/cat_8.png"),
	"cat_9" : preload("res://Resources/Textures/PlayerTexture/mutations/cat_9.png"),
	"octopode_1" : preload("res://Resources/Textures/PlayerTexture/mutations/octopode_1.png")}


func getPlayerTexture_mutations(resourceName : String) :
	return PlayerTexture_mutationsDictionary.get(resourceName)

const PlayerTexture_transformDictionary = {
	"bat_form" : preload("res://Resources/Textures/PlayerTexture/transform/bat_form.png"),
	"dragon_form" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form.png"),
	"dragon_form_black" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_black.png"),
	"dragon_form_green" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_green.png"),
	"dragon_form_grey" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_grey.png"),
	"dragon_form_mottled" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_mottled.png"),
	"dragon_form_pale" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_pale.png"),
	"dragon_form_purple" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_purple.png"),
	"dragon_form_red" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_red.png"),
	"dragon_form_white" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_white.png"),
	"dragon_form_yellow" : preload("res://Resources/Textures/PlayerTexture/transform/dragon_form_yellow.png"),
	"ice_form" : preload("res://Resources/Textures/PlayerTexture/transform/ice_form.png"),
	"lich_form" : preload("res://Resources/Textures/PlayerTexture/transform/lich_form.png"),
	"lich_form_octopode" : preload("res://Resources/Textures/PlayerTexture/transform/lich_form_octopode.png"),
	"mushroom_form" : preload("res://Resources/Textures/PlayerTexture/transform/mushroom_form.png"),
	"pig_form_new" : preload("res://Resources/Textures/PlayerTexture/transform/pig_form_new.png"),
	"pig_form_old" : preload("res://Resources/Textures/PlayerTexture/transform/pig_form_old.png"),
	"shadow_form" : preload("res://Resources/Textures/PlayerTexture/transform/shadow_form.png"),
	"statue_form_centaur" : preload("res://Resources/Textures/PlayerTexture/transform/statue_form_centaur.png"),
	"statue_form_felid" : preload("res://Resources/Textures/PlayerTexture/transform/statue_form_felid.png"),
	"statue_form_humanoid" : preload("res://Resources/Textures/PlayerTexture/transform/statue_form_humanoid.png"),
	"statue_form_naga" : preload("res://Resources/Textures/PlayerTexture/transform/statue_form_naga.png"),
	"tree_form" : preload("res://Resources/Textures/PlayerTexture/transform/tree_form.png")}


func getPlayerTexture_transform(resourceName : String) :
	return PlayerTexture_transformDictionary.get(resourceName)

func getPlayerTexture(resourceName : String) :
	if (PlayerTexture_bardingDictionary.has(resourceName)) :
		return PlayerTexture_bardingDictionary[resourceName]
	if (PlayerTexture_baseDictionary.has(resourceName)) :
		return PlayerTexture_baseDictionary[resourceName]
	if (PlayerTexture_beardDictionary.has(resourceName)) :
		return PlayerTexture_beardDictionary[resourceName]
	if (PlayerTexture_bodyDictionary.has(resourceName)) :
		return PlayerTexture_bodyDictionary[resourceName]
	if (PlayerTexture_bootsDictionary.has(resourceName)) :
		return PlayerTexture_bootsDictionary[resourceName]
	if (PlayerTexture_cloakDictionary.has(resourceName)) :
		return PlayerTexture_cloakDictionary[resourceName]
	if (PlayerTexture_draconic_headDictionary.has(resourceName)) :
		return PlayerTexture_draconic_headDictionary[resourceName]
	if (PlayerTexture_draconic_wingDictionary.has(resourceName)) :
		return PlayerTexture_draconic_wingDictionary[resourceName]
	if (PlayerTexture_enchantmentDictionary.has(resourceName)) :
		return PlayerTexture_enchantmentDictionary[resourceName]
	if (PlayerTexture_felidsDictionary.has(resourceName)) :
		return PlayerTexture_felidsDictionary[resourceName]
	if (PlayerTexture_glovesDictionary.has(resourceName)) :
		return PlayerTexture_glovesDictionary[resourceName]
	if (PlayerTexture_hairDictionary.has(resourceName)) :
		return PlayerTexture_hairDictionary[resourceName]
	if (PlayerTexture_haloDictionary.has(resourceName)) :
		return PlayerTexture_haloDictionary[resourceName]
	if (PlayerTexture_hand_leftDictionary.has(resourceName)) :
		return PlayerTexture_hand_leftDictionary[resourceName]
	if (PlayerTexture_hand_rightDictionary.has(resourceName)) :
		return PlayerTexture_hand_rightDictionary[resourceName]
	if (PlayerTexture_headDictionary.has(resourceName)) :
		return PlayerTexture_headDictionary[resourceName]
	if (PlayerTexture_legsDictionary.has(resourceName)) :
		return PlayerTexture_legsDictionary[resourceName]
	if (PlayerTexture_mutationsDictionary.has(resourceName)) :
		return PlayerTexture_mutationsDictionary[resourceName]
	if (PlayerTexture_transformDictionary.has(resourceName)) :
		return PlayerTexture_transformDictionary[resourceName]
	return null

func getPlayerTextureDictionary(type : String) :
	if type == "PlayerTexture_barding" : 
		return PlayerTexture_bardingDictionary
	if type == "PlayerTexture_base" : 
		return PlayerTexture_baseDictionary
	if type == "PlayerTexture_beard" : 
		return PlayerTexture_beardDictionary
	if type == "PlayerTexture_body" : 
		return PlayerTexture_bodyDictionary
	if type == "PlayerTexture_boots" : 
		return PlayerTexture_bootsDictionary
	if type == "PlayerTexture_cloak" : 
		return PlayerTexture_cloakDictionary
	if type == "PlayerTexture_draconic_head" : 
		return PlayerTexture_draconic_headDictionary
	if type == "PlayerTexture_draconic_wing" : 
		return PlayerTexture_draconic_wingDictionary
	if type == "PlayerTexture_enchantment" : 
		return PlayerTexture_enchantmentDictionary
	if type == "PlayerTexture_felids" : 
		return PlayerTexture_felidsDictionary
	if type == "PlayerTexture_gloves" : 
		return PlayerTexture_glovesDictionary
	if type == "PlayerTexture_hair" : 
		return PlayerTexture_hairDictionary
	if type == "PlayerTexture_halo" : 
		return PlayerTexture_haloDictionary
	if type == "PlayerTexture_hand_left" : 
		return PlayerTexture_hand_leftDictionary
	if type == "PlayerTexture_hand_right" : 
		return PlayerTexture_hand_rightDictionary
	if type == "PlayerTexture_head" : 
		return PlayerTexture_headDictionary
	if type == "PlayerTexture_legs" : 
		return PlayerTexture_legsDictionary
	if type == "PlayerTexture_mutations" : 
		return PlayerTexture_mutationsDictionary
	if type == "PlayerTexture_transform" : 
		return PlayerTexture_transformDictionary

func getAllPlayerTexture() -> Array :
	var retVal : Array = []
	retVal.append_array(PlayerTexture_bardingDictionary.values())
	retVal.append_array(PlayerTexture_baseDictionary.values())
	retVal.append_array(PlayerTexture_beardDictionary.values())
	retVal.append_array(PlayerTexture_bodyDictionary.values())
	retVal.append_array(PlayerTexture_bootsDictionary.values())
	retVal.append_array(PlayerTexture_cloakDictionary.values())
	retVal.append_array(PlayerTexture_draconic_headDictionary.values())
	retVal.append_array(PlayerTexture_draconic_wingDictionary.values())
	retVal.append_array(PlayerTexture_enchantmentDictionary.values())
	retVal.append_array(PlayerTexture_felidsDictionary.values())
	retVal.append_array(PlayerTexture_glovesDictionary.values())
	retVal.append_array(PlayerTexture_hairDictionary.values())
	retVal.append_array(PlayerTexture_haloDictionary.values())
	retVal.append_array(PlayerTexture_hand_leftDictionary.values())
	retVal.append_array(PlayerTexture_hand_rightDictionary.values())
	retVal.append_array(PlayerTexture_headDictionary.values())
	retVal.append_array(PlayerTexture_legsDictionary.values())
	retVal.append_array(PlayerTexture_mutationsDictionary.values())
	retVal.append_array(PlayerTexture_transformDictionary.values())
	return retVal

################################################################################
