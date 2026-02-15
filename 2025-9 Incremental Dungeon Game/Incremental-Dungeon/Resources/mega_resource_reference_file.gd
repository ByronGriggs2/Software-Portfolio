extends Node

var Environment_FilesDictionary = {
	"chaos" : load("res://Resources/Environment/Files/chaos.tres"),
	"demonic_city" : load("res://Resources/Environment/Files/demonic_city.tres"),
	"desert" : load("res://Resources/Environment/Files/desert.tres"),
	"element_earth" : load("res://Resources/Environment/Files/element_earth.tres"),
	"element_fire" : load("res://Resources/Environment/Files/element_fire.tres"),
	"element_fire_earth" : load("res://Resources/Environment/Files/element_fire_earth.tres"),
	"element_fire_ice" : load("res://Resources/Environment/Files/element_fire_ice.tres"),
	"element_ice" : load("res://Resources/Environment/Files/element_ice.tres"),
	"element_ice_earth" : load("res://Resources/Environment/Files/element_ice_earth.tres"),
	"element_water" : load("res://Resources/Environment/Files/element_water.tres"),
	"fort_demon" : load("res://Resources/Environment/Files/fort_demon.tres"),
	"fort_greenskin" : load("res://Resources/Environment/Files/fort_greenskin.tres"),
	"fort_merfolk" : load("res://Resources/Environment/Files/fort_merfolk.tres"),
	"fort_undead" : load("res://Resources/Environment/Files/fort_undead.tres"),
	"hell" : load("res://Resources/Environment/Files/hell.tres"),
	"hellforge" : load("res://Resources/Environment/Files/hellforge.tres"),
	"pit_of_depravity" : load("res://Resources/Environment/Files/pit_of_depravity.tres"),
	"reef_community" : load("res://Resources/Environment/Files/reef_community.tres"),
	"unholy_alliance" : load("res://Resources/Environment/Files/unholy_alliance.tres"),
	"unlikely_alliance" : load("res://Resources/Environment/Files/unlikely_alliance.tres")}


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
var NewAction_AttacksDictionary = {
	"axe_1h" : load("res://Resources/NewAction/Attacks/axe_1h.tres"),
	"axe_2h" : load("res://Resources/NewAction/Attacks/axe_2h.tres"),
	"blunderbuss" : load("res://Resources/NewAction/Attacks/blunderbuss.tres"),
	"blur" : load("res://Resources/NewAction/Attacks/blur.tres"),
	"bow" : load("res://Resources/NewAction/Attacks/bow.tres"),
	"club_1h" : load("res://Resources/NewAction/Attacks/club_1h.tres"),
	"crack" : load("res://Resources/NewAction/Attacks/crack.tres"),
	"crossbow" : load("res://Resources/NewAction/Attacks/crossbow.tres"),
	"curse" : load("res://Resources/NewAction/Attacks/curse.tres"),
	"dagger" : load("res://Resources/NewAction/Attacks/dagger.tres"),
	"disintigrate" : load("res://Resources/NewAction/Attacks/disintigrate.tres"),
	"flame_whip" : load("res://Resources/NewAction/Attacks/flame_whip.tres"),
	"flurry" : load("res://Resources/NewAction/Attacks/flurry.tres"),
	"freezeflame_double_slice" : load("res://Resources/NewAction/Attacks/freezeflame_double_slice.tres"),
	"frost_slash" : load("res://Resources/NewAction/Attacks/frost_slash.tres"),
	"generic_brutal" : load("res://Resources/NewAction/Attacks/generic_brutal.tres"),
	"icicle_spike" : load("res://Resources/NewAction/Attacks/icicle_spike.tres"),
	"katana_attack" : load("res://Resources/NewAction/Attacks/katana_attack.tres"),
	"kinetic_blast" : load("res://Resources/NewAction/Attacks/kinetic_blast.tres"),
	"lance" : load("res://Resources/NewAction/Attacks/lance.tres"),
	"laser" : load("res://Resources/NewAction/Attacks/laser.tres"),
	"lightning" : load("res://Resources/NewAction/Attacks/lightning.tres"),
	"lucky_star" : load("res://Resources/NewAction/Attacks/lucky_star.tres"),
	"mace_1h" : load("res://Resources/NewAction/Attacks/mace_1h.tres"),
	"mace_2h" : load("res://Resources/NewAction/Attacks/mace_2h.tres"),
	"magic_lance" : load("res://Resources/NewAction/Attacks/magic_lance.tres"),
	"magic_repeater" : load("res://Resources/NewAction/Attacks/magic_repeater.tres"),
	"magic_slashing_sword_2h" : load("res://Resources/NewAction/Attacks/magic_slashing_sword_2h.tres"),
	"magic_split_man" : load("res://Resources/NewAction/Attacks/magic_split_man.tres"),
	"scythe" : load("res://Resources/NewAction/Attacks/scythe.tres"),
	"shortbow" : load("res://Resources/NewAction/Attacks/shortbow.tres"),
	"slashing_sword_1h" : load("res://Resources/NewAction/Attacks/slashing_sword_1h.tres"),
	"slashing_sword_2h" : load("res://Resources/NewAction/Attacks/slashing_sword_2h.tres"),
	"sling" : load("res://Resources/NewAction/Attacks/sling.tres"),
	"spear_1h" : load("res://Resources/NewAction/Attacks/spear_1h.tres"),
	"spear_2h" : load("res://Resources/NewAction/Attacks/spear_2h.tres"),
	"spear_darkness" : load("res://Resources/NewAction/Attacks/spear_darkness.tres"),
	"split_man" : load("res://Resources/NewAction/Attacks/split_man.tres"),
	"thresh" : load("res://Resources/NewAction/Attacks/thresh.tres"),
	"throw_axe" : load("res://Resources/NewAction/Attacks/throw_axe.tres"),
	"throw_boulder" : load("res://Resources/NewAction/Attacks/throw_boulder.tres"),
	"throw_kunai" : load("res://Resources/NewAction/Attacks/throw_kunai.tres"),
	"thrust_sword_1h" : load("res://Resources/NewAction/Attacks/thrust_sword_1h.tres"),
	"thrust_sword_2h" : load("res://Resources/NewAction/Attacks/thrust_sword_2h.tres"),
	"water_jet" : load("res://Resources/NewAction/Attacks/water_jet.tres"),
	"water_jet_2" : load("res://Resources/NewAction/Attacks/water_jet_2.tres"),
	"water_jet_3" : load("res://Resources/NewAction/Attacks/water_jet_3.tres")}


func getNewAction_Attacks(resourceName : String) :
	return NewAction_AttacksDictionary.get(resourceName)

var NewAction_EnemyExclusiveAttacksDictionary = {
	"arcane_bolt" : load("res://Resources/NewAction/EnemyExclusiveAttacks/arcane_bolt.tres"),
	"bite_acid_2" : load("res://Resources/NewAction/EnemyExclusiveAttacks/bite_acid_2.tres"),
	"bite_big" : load("res://Resources/NewAction/EnemyExclusiveAttacks/bite_big.tres"),
	"bite_fire" : load("res://Resources/NewAction/EnemyExclusiveAttacks/bite_fire.tres"),
	"bite_generic" : load("res://Resources/NewAction/EnemyExclusiveAttacks/bite_generic.tres"),
	"bite_ice" : load("res://Resources/NewAction/EnemyExclusiveAttacks/bite_ice.tres"),
	"bite_ice_2" : load("res://Resources/NewAction/EnemyExclusiveAttacks/bite_ice_2.tres"),
	"bite_venomous" : load("res://Resources/NewAction/EnemyExclusiveAttacks/bite_venomous.tres"),
	"breath_acid" : load("res://Resources/NewAction/EnemyExclusiveAttacks/breath_acid.tres"),
	"breath_death" : load("res://Resources/NewAction/EnemyExclusiveAttacks/breath_death.tres"),
	"breath_fire" : load("res://Resources/NewAction/EnemyExclusiveAttacks/breath_fire.tres"),
	"breath_frost" : load("res://Resources/NewAction/EnemyExclusiveAttacks/breath_frost.tres"),
	"breath_iron" : load("res://Resources/NewAction/EnemyExclusiveAttacks/breath_iron.tres"),
	"breath_midas" : load("res://Resources/NewAction/EnemyExclusiveAttacks/breath_midas.tres"),
	"breath_shadow" : load("res://Resources/NewAction/EnemyExclusiveAttacks/breath_shadow.tres"),
	"breath_water" : load("res://Resources/NewAction/EnemyExclusiveAttacks/breath_water.tres"),
	"claw" : load("res://Resources/NewAction/EnemyExclusiveAttacks/claw.tres"),
	"claw_fire" : load("res://Resources/NewAction/EnemyExclusiveAttacks/claw_fire.tres"),
	"fire_blast" : load("res://Resources/NewAction/EnemyExclusiveAttacks/fire_blast.tres"),
	"fire_blast_2" : load("res://Resources/NewAction/EnemyExclusiveAttacks/fire_blast_2.tres"),
	"fire_blast_3" : load("res://Resources/NewAction/EnemyExclusiveAttacks/fire_blast_3.tres"),
	"gore" : load("res://Resources/NewAction/EnemyExclusiveAttacks/gore.tres"),
	"hellfire_cleave" : load("res://Resources/NewAction/EnemyExclusiveAttacks/hellfire_cleave.tres"),
	"ice_blast" : load("res://Resources/NewAction/EnemyExclusiveAttacks/ice_blast.tres"),
	"javelin" : load("res://Resources/NewAction/EnemyExclusiveAttacks/javelin.tres"),
	"jellyfish" : load("res://Resources/NewAction/EnemyExclusiveAttacks/jellyfish.tres"),
	"lava" : load("res://Resources/NewAction/EnemyExclusiveAttacks/lava.tres"),
	"poison_spore" : load("res://Resources/NewAction/EnemyExclusiveAttacks/poison_spore.tres"),
	"psychic_scream" : load("res://Resources/NewAction/EnemyExclusiveAttacks/psychic_scream.tres"),
	"stomp_fire" : load("res://Resources/NewAction/EnemyExclusiveAttacks/stomp_fire.tres"),
	"stomp_generic" : load("res://Resources/NewAction/EnemyExclusiveAttacks/stomp_generic.tres"),
	"stomp_ice" : load("res://Resources/NewAction/EnemyExclusiveAttacks/stomp_ice.tres")}


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
var NewActor_demonicDictionary = {
	"abomination" : load("res://Resources/NewActor/demonic/abomination.tres"),
	"demon_elephant" : load("res://Resources/NewActor/demonic/demon_elephant.tres"),
	"hairy_devil" : load("res://Resources/NewActor/demonic/hairy_devil.tres"),
	"hell_beast" : load("res://Resources/NewActor/demonic/hell_beast.tres"),
	"hell_hog" : load("res://Resources/NewActor/demonic/hell_hog.tres"),
	"hell_hound" : load("res://Resources/NewActor/demonic/hell_hound.tres"),
	"hellion" : load("res://Resources/NewActor/demonic/hellion.tres"),
	"succubus" : load("res://Resources/NewActor/demonic/succubus.tres"),
	"tormentor" : load("res://Resources/NewActor/demonic/tormentor.tres")}


func getNewActor_demonic(resourceName : String) :
	return NewActor_demonicDictionary.get(resourceName)

var NewActor_demonic_militaryDictionary = {
	"apophis" : load("res://Resources/NewActor/demonic_military/apophis.tres"),
	"arch_fiend" : load("res://Resources/NewActor/demonic_military/arch_fiend.tres"),
	"balrog" : load("res://Resources/NewActor/demonic_military/balrog.tres"),
	"death_knight" : load("res://Resources/NewActor/demonic_military/death_knight.tres"),
	"hell_knight" : load("res://Resources/NewActor/demonic_military/hell_knight.tres"),
	"hell_sentinel" : load("res://Resources/NewActor/demonic_military/hell_sentinel.tres"),
	"hell_wizard" : load("res://Resources/NewActor/demonic_military/hell_wizard.tres"),
	"imp" : load("res://Resources/NewActor/demonic_military/imp.tres"),
	"salamander_melee" : load("res://Resources/NewActor/demonic_military/salamander_melee.tres"),
	"salamander_ranged" : load("res://Resources/NewActor/demonic_military/salamander_ranged.tres"),
	"shadow_imp" : load("res://Resources/NewActor/demonic_military/shadow_imp.tres")}


func getNewActor_demonic_military(resourceName : String) :
	return NewActor_demonic_militaryDictionary.get(resourceName)

var NewActor_earthDictionary = {
	"deathcap" : load("res://Resources/NewActor/earth/deathcap.tres"),
	"dryad" : load("res://Resources/NewActor/earth/dryad.tres"),
	"faun" : load("res://Resources/NewActor/earth/faun.tres"),
	"forest_drake" : load("res://Resources/NewActor/earth/forest_drake.tres"),
	"hill_giant" : load("res://Resources/NewActor/earth/hill_giant.tres"),
	"lindwurm" : load("res://Resources/NewActor/earth/lindwurm.tres"),
	"rock_troll" : load("res://Resources/NewActor/earth/rock_troll.tres"),
	"satyr" : load("res://Resources/NewActor/earth/satyr.tres"),
	"swamp_dragon" : load("res://Resources/NewActor/earth/swamp_dragon.tres"),
	"swamp_worm" : load("res://Resources/NewActor/earth/swamp_worm.tres"),
	"thorned_ambusher" : load("res://Resources/NewActor/earth/thorned_ambusher.tres"),
	"treant" : load("res://Resources/NewActor/earth/treant.tres")}


func getNewActor_earth(resourceName : String) :
	return NewActor_earthDictionary.get(resourceName)

var NewActor_fireDictionary = {
	"fire_bat" : load("res://Resources/NewActor/fire/fire_bat.tres"),
	"fire_crab" : load("res://Resources/NewActor/fire/fire_crab.tres"),
	"fire_dragon" : load("res://Resources/NewActor/fire/fire_dragon.tres"),
	"fire_elemental" : load("res://Resources/NewActor/fire/fire_elemental.tres"),
	"fire_giant" : load("res://Resources/NewActor/fire/fire_giant.tres"),
	"lava_fish" : load("res://Resources/NewActor/fire/lava_fish.tres"),
	"lava_snake" : load("res://Resources/NewActor/fire/lava_snake.tres"),
	"lava_worm" : load("res://Resources/NewActor/fire/lava_worm.tres"),
	"lesser_fire_elemental" : load("res://Resources/NewActor/fire/lesser_fire_elemental.tres")}


func getNewActor_fire(resourceName : String) :
	return NewActor_fireDictionary.get(resourceName)

var NewActor_greenskinDictionary = {
	"gnoll" : load("res://Resources/NewActor/greenskin/gnoll.tres"),
	"goblin_ranger" : load("res://Resources/NewActor/greenskin/goblin_ranger.tres"),
	"goblin_skirmisher" : load("res://Resources/NewActor/greenskin/goblin_skirmisher.tres"),
	"juggernaut" : load("res://Resources/NewActor/greenskin/juggernaut.tres"),
	"orc_knight" : load("res://Resources/NewActor/greenskin/orc_knight.tres"),
	"orc_sorcerer" : load("res://Resources/NewActor/greenskin/orc_sorcerer.tres"),
	"orc_warlord" : load("res://Resources/NewActor/greenskin/orc_warlord.tres"),
	"polyphemus" : load("res://Resources/NewActor/greenskin/polyphemus.tres")}


func getNewActor_greenskin(resourceName : String) :
	return NewActor_greenskinDictionary.get(resourceName)

var NewActor_iceDictionary = {
	"frost wraith" : load("res://Resources/NewActor/ice/frost wraith.tres"),
	"ice_beast" : load("res://Resources/NewActor/ice/ice_beast.tres"),
	"ice_devil" : load("res://Resources/NewActor/ice/ice_devil.tres"),
	"ice_dragon" : load("res://Resources/NewActor/ice/ice_dragon.tres"),
	"ice_elemental" : load("res://Resources/NewActor/ice/ice_elemental.tres"),
	"ice_giant" : load("res://Resources/NewActor/ice/ice_giant.tres"),
	"ice_hydra" : load("res://Resources/NewActor/ice/ice_hydra.tres"),
	"ice_slime" : load("res://Resources/NewActor/ice/ice_slime.tres"),
	"ice_troll" : load("res://Resources/NewActor/ice/ice_troll.tres"),
	"polar_bear" : load("res://Resources/NewActor/ice/polar_bear.tres")}


func getNewActor_ice(resourceName : String) :
	return NewActor_iceDictionary.get(resourceName)

var NewActor_merfolkDictionary = {
	"champion_of_poseidon" : load("res://Resources/NewActor/merfolk/champion_of_poseidon.tres"),
	"ilsuiw" : load("res://Resources/NewActor/merfolk/ilsuiw.tres"),
	"merfolk_T1_mage" : load("res://Resources/NewActor/merfolk/merfolk_T1_mage.tres"),
	"merfolk_T1_spear" : load("res://Resources/NewActor/merfolk/merfolk_T1_spear.tres"),
	"merfolk_T2_mage" : load("res://Resources/NewActor/merfolk/merfolk_T2_mage.tres"),
	"merfolk_T2_spear_melee" : load("res://Resources/NewActor/merfolk/merfolk_T2_spear_melee.tres"),
	"merfolk_T2_spear_ranged" : load("res://Resources/NewActor/merfolk/merfolk_T2_spear_ranged.tres"),
	"siren" : load("res://Resources/NewActor/merfolk/siren.tres")}


func getNewActor_merfolk(resourceName : String) :
	return NewActor_merfolkDictionary.get(resourceName)

var NewActor_miscDictionary = {
	"athena" : load("res://Resources/NewActor/misc/athena.tres"),
	"caustic_shrike" : load("res://Resources/NewActor/misc/caustic_shrike.tres"),
	"gold_dragon" : load("res://Resources/NewActor/misc/gold_dragon.tres"),
	"iron_dragon" : load("res://Resources/NewActor/misc/iron_dragon.tres"),
	"iron_golem" : load("res://Resources/NewActor/misc/iron_golem.tres"),
	"iron_troll" : load("res://Resources/NewActor/misc/iron_troll.tres"),
	"ironbrand" : load("res://Resources/NewActor/misc/ironbrand.tres"),
	"shadow_dragon" : load("res://Resources/NewActor/misc/shadow_dragon.tres"),
	"titan" : load("res://Resources/NewActor/misc/titan.tres"),
	"troll" : load("res://Resources/NewActor/misc/troll.tres")}


func getNewActor_misc(resourceName : String) :
	return NewActor_miscDictionary.get(resourceName)

var NewActor_undeadDictionary = {
	"amelia" : load("res://Resources/NewActor/undead/amelia.tres"),
	"bone_dragon" : load("res://Resources/NewActor/undead/bone_dragon.tres"),
	"death" : load("res://Resources/NewActor/undead/death.tres"),
	"jiangshi_assassin" : load("res://Resources/NewActor/undead/jiangshi_assassin.tres"),
	"lich" : load("res://Resources/NewActor/undead/lich.tres"),
	"mummy_fighter" : load("res://Resources/NewActor/undead/mummy_fighter.tres"),
	"mummy_ranger" : load("res://Resources/NewActor/undead/mummy_ranger.tres"),
	"necromancer" : load("res://Resources/NewActor/undead/necromancer.tres"),
	"rotten_amaglamation" : load("res://Resources/NewActor/undead/rotten_amaglamation.tres"),
	"skeleton" : load("res://Resources/NewActor/undead/skeleton.tres"),
	"vampire" : load("res://Resources/NewActor/undead/vampire.tres")}


func getNewActor_undead(resourceName : String) :
	return NewActor_undeadDictionary.get(resourceName)

var NewActor_waterDictionary = {
	"black_mamba" : load("res://Resources/NewActor/water/black_mamba.tres"),
	"electric_eel" : load("res://Resources/NewActor/water/electric_eel.tres"),
	"jellyfish" : load("res://Resources/NewActor/water/jellyfish.tres"),
	"kraken" : load("res://Resources/NewActor/water/kraken.tres"),
	"nymph" : load("res://Resources/NewActor/water/nymph.tres"),
	"shark" : load("res://Resources/NewActor/water/shark.tres"),
	"water_dragon" : load("res://Resources/NewActor/water/water_dragon.tres"),
	"water_elemental" : load("res://Resources/NewActor/water/water_elemental.tres"),
	"water_troll" : load("res://Resources/NewActor/water/water_troll.tres")}


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
var NewEquipment_AccessoriesDictionary = {
	"apple" : load("res://Resources/NewEquipment/Accessories/apple.tres"),
	"bread" : load("res://Resources/NewEquipment/Accessories/bread.tres"),
	"coating_divine" : load("res://Resources/NewEquipment/Accessories/coating_divine.tres"),
	"coating_earth_1" : load("res://Resources/NewEquipment/Accessories/coating_earth_1.tres"),
	"coating_earth_2" : load("res://Resources/NewEquipment/Accessories/coating_earth_2.tres"),
	"coating_fire_1" : load("res://Resources/NewEquipment/Accessories/coating_fire_1.tres"),
	"coating_fire_2" : load("res://Resources/NewEquipment/Accessories/coating_fire_2.tres"),
	"coating_ice_1" : load("res://Resources/NewEquipment/Accessories/coating_ice_1.tres"),
	"coating_ice_2" : load("res://Resources/NewEquipment/Accessories/coating_ice_2.tres"),
	"coating_water_1" : load("res://Resources/NewEquipment/Accessories/coating_water_1.tres"),
	"coating_water_2" : load("res://Resources/NewEquipment/Accessories/coating_water_2.tres"),
	"feathered_hat" : load("res://Resources/NewEquipment/Accessories/feathered_hat.tres"),
	"guantlets_cyclops_strength" : load("res://Resources/NewEquipment/Accessories/guantlets_cyclops_strength.tres"),
	"lich_crown" : load("res://Resources/NewEquipment/Accessories/lich_crown.tres"),
	"lightning_arrows_1" : load("res://Resources/NewEquipment/Accessories/lightning_arrows_1.tres"),
	"lightning_arrows_2" : load("res://Resources/NewEquipment/Accessories/lightning_arrows_2.tres"),
	"mage_hat" : load("res://Resources/NewEquipment/Accessories/mage_hat.tres"),
	"mandrake_root_new" : load("res://Resources/NewEquipment/Accessories/mandrake_root_new.tres"),
	"pizza" : load("res://Resources/NewEquipment/Accessories/pizza.tres"),
	"rampart_agony" : load("res://Resources/NewEquipment/Accessories/rampart_agony.tres"),
	"ring_authority" : load("res://Resources/NewEquipment/Accessories/ring_authority.tres"),
	"ring_malice" : load("res://Resources/NewEquipment/Accessories/ring_malice.tres"),
	"rune_advanced_automation" : load("res://Resources/NewEquipment/Accessories/rune_advanced_automation.tres"),
	"shield_1" : load("res://Resources/NewEquipment/Accessories/shield_1.tres"),
	"shield_2" : load("res://Resources/NewEquipment/Accessories/shield_2.tres"),
	"shield_3" : load("res://Resources/NewEquipment/Accessories/shield_3.tres"),
	"shield_4" : load("res://Resources/NewEquipment/Accessories/shield_4.tres"),
	"shield_5" : load("res://Resources/NewEquipment/Accessories/shield_5.tres"),
	"tome_death_1" : load("res://Resources/NewEquipment/Accessories/tome_death_1.tres"),
	"tome_death_2" : load("res://Resources/NewEquipment/Accessories/tome_death_2.tres"),
	"tome_earth_1" : load("res://Resources/NewEquipment/Accessories/tome_earth_1.tres"),
	"tome_earth_2" : load("res://Resources/NewEquipment/Accessories/tome_earth_2.tres"),
	"tome_fire_1" : load("res://Resources/NewEquipment/Accessories/tome_fire_1.tres"),
	"tome_fire_2" : load("res://Resources/NewEquipment/Accessories/tome_fire_2.tres"),
	"tome_ice_1" : load("res://Resources/NewEquipment/Accessories/tome_ice_1.tres"),
	"tome_ice_2" : load("res://Resources/NewEquipment/Accessories/tome_ice_2.tres"),
	"tome_water_1" : load("res://Resources/NewEquipment/Accessories/tome_water_1.tres"),
	"tome_water_2" : load("res://Resources/NewEquipment/Accessories/tome_water_2.tres"),
	"warrior_helm" : load("res://Resources/NewEquipment/Accessories/warrior_helm.tres")}


func getNewEquipment_Accessories(resourceName : String) :
	return NewEquipment_AccessoriesDictionary.get(resourceName)

var NewEquipment_ArmorDictionary = {
	"armor_blades" : load("res://Resources/NewEquipment/Armor/armor_blades.tres"),
	"armor_cephalopod" : load("res://Resources/NewEquipment/Armor/armor_cephalopod.tres"),
	"armor_crab" : load("res://Resources/NewEquipment/Armor/armor_crab.tres"),
	"armor_dragonscale" : load("res://Resources/NewEquipment/Armor/armor_dragonscale.tres"),
	"armor_earth" : load("res://Resources/NewEquipment/Armor/armor_earth.tres"),
	"armor_fire" : load("res://Resources/NewEquipment/Armor/armor_fire.tres"),
	"armor_ice" : load("res://Resources/NewEquipment/Armor/armor_ice.tres"),
	"armor_leather" : load("res://Resources/NewEquipment/Armor/armor_leather.tres"),
	"armor_molten" : load("res://Resources/NewEquipment/Armor/armor_molten.tres"),
	"armor_scale" : load("res://Resources/NewEquipment/Armor/armor_scale.tres"),
	"armor_shadow" : load("res://Resources/NewEquipment/Armor/armor_shadow.tres"),
	"armor_water" : load("res://Resources/NewEquipment/Armor/armor_water.tres"),
	"chainmail" : load("res://Resources/NewEquipment/Armor/chainmail.tres"),
	"dragonbone_plate" : load("res://Resources/NewEquipment/Armor/dragonbone_plate.tres"),
	"enchanted_robes_new" : load("res://Resources/NewEquipment/Armor/enchanted_robes_new.tres"),
	"fur_cloak" : load("res://Resources/NewEquipment/Armor/fur_cloak.tres"),
	"hellforged_plate" : load("res://Resources/NewEquipment/Armor/hellforged_plate.tres"),
	"mithril_plate" : load("res://Resources/NewEquipment/Armor/mithril_plate.tres"),
	"moonstone_plate" : load("res://Resources/NewEquipment/Armor/moonstone_plate.tres"),
	"scale_mail" : load("res://Resources/NewEquipment/Armor/scale_mail.tres"),
	"silk_robes" : load("res://Resources/NewEquipment/Armor/silk_robes.tres"),
	"steel_plate" : load("res://Resources/NewEquipment/Armor/steel_plate.tres"),
	"studded_leather" : load("res://Resources/NewEquipment/Armor/studded_leather.tres")}


func getNewEquipment_Armor(resourceName : String) :
	return NewEquipment_ArmorDictionary.get(resourceName)

var NewEquipment_CurrencyDictionary = {
	"gold_coin" : load("res://Resources/NewEquipment/Currency/gold_coin.tres"),
	"ore" : load("res://Resources/NewEquipment/Currency/ore.tres"),
	"soul" : load("res://Resources/NewEquipment/Currency/soul.tres")}


func getNewEquipment_Currency(resourceName : String) :
	return NewEquipment_CurrencyDictionary.get(resourceName)

var NewEquipment_WeaponsDictionary = {
	"adamantium_greataxe" : load("res://Resources/NewEquipment/Weapons/adamantium_greataxe.tres"),
	"amelia_wand" : load("res://Resources/NewEquipment/Weapons/amelia_wand.tres"),
	"arming_sword" : load("res://Resources/NewEquipment/Weapons/arming_sword.tres"),
	"asmodeus_staff" : load("res://Resources/NewEquipment/Weapons/asmodeus_staff.tres"),
	"bardiche" : load("res://Resources/NewEquipment/Weapons/bardiche.tres"),
	"blade_of_darkness" : load("res://Resources/NewEquipment/Weapons/blade_of_darkness.tres"),
	"blade_of_sin" : load("res://Resources/NewEquipment/Weapons/blade_of_sin.tres"),
	"blade_of_the_lich_king" : load("res://Resources/NewEquipment/Weapons/blade_of_the_lich_king.tres"),
	"blunderbuss" : load("res://Resources/NewEquipment/Weapons/blunderbuss.tres"),
	"club" : load("res://Resources/NewEquipment/Weapons/club.tres"),
	"crackling_greataxe" : load("res://Resources/NewEquipment/Weapons/crackling_greataxe.tres"),
	"crossbow" : load("res://Resources/NewEquipment/Weapons/crossbow.tres"),
	"crystal_sword" : load("res://Resources/NewEquipment/Weapons/crystal_sword.tres"),
	"cutlass" : load("res://Resources/NewEquipment/Weapons/cutlass.tres"),
	"dagger" : load("res://Resources/NewEquipment/Weapons/dagger.tres"),
	"deaths_scythe" : load("res://Resources/NewEquipment/Weapons/deaths_scythe.tres"),
	"diamond_staff" : load("res://Resources/NewEquipment/Weapons/diamond_staff.tres"),
	"divine_greatmace" : load("res://Resources/NewEquipment/Weapons/divine_greatmace.tres"),
	"earth_hammer" : load("res://Resources/NewEquipment/Weapons/earth_hammer.tres"),
	"earth_staff" : load("res://Resources/NewEquipment/Weapons/earth_staff.tres"),
	"emerald_lance" : load("res://Resources/NewEquipment/Weapons/emerald_lance.tres"),
	"emerald_staff" : load("res://Resources/NewEquipment/Weapons/emerald_staff.tres"),
	"fire_spitter" : load("res://Resources/NewEquipment/Weapons/fire_spitter.tres"),
	"fire_staff" : load("res://Resources/NewEquipment/Weapons/fire_staff.tres"),
	"flail" : load("res://Resources/NewEquipment/Weapons/flail.tres"),
	"flamberge" : load("res://Resources/NewEquipment/Weapons/flamberge.tres"),
	"force_wand" : load("res://Resources/NewEquipment/Weapons/force_wand.tres"),
	"frostfire_twindaggers" : load("res://Resources/NewEquipment/Weapons/frostfire_twindaggers.tres"),
	"great_flail" : load("res://Resources/NewEquipment/Weapons/great_flail.tres"),
	"greataxe" : load("res://Resources/NewEquipment/Weapons/greataxe.tres"),
	"greatsword" : load("res://Resources/NewEquipment/Weapons/greatsword.tres"),
	"halberd" : load("res://Resources/NewEquipment/Weapons/halberd.tres"),
	"hammer" : load("res://Resources/NewEquipment/Weapons/hammer.tres"),
	"handaxe" : load("res://Resources/NewEquipment/Weapons/handaxe.tres"),
	"hungering_blade" : load("res://Resources/NewEquipment/Weapons/hungering_blade.tres"),
	"ice_lance" : load("res://Resources/NewEquipment/Weapons/ice_lance.tres"),
	"ice_staff" : load("res://Resources/NewEquipment/Weapons/ice_staff.tres"),
	"ice_sword" : load("res://Resources/NewEquipment/Weapons/ice_sword.tres"),
	"katana" : load("res://Resources/NewEquipment/Weapons/katana.tres"),
	"khopesh" : load("res://Resources/NewEquipment/Weapons/khopesh.tres"),
	"kunai" : load("res://Resources/NewEquipment/Weapons/kunai.tres"),
	"lance" : load("res://Resources/NewEquipment/Weapons/lance.tres"),
	"lightning_rod" : load("res://Resources/NewEquipment/Weapons/lightning_rod.tres"),
	"long_axe" : load("res://Resources/NewEquipment/Weapons/long_axe.tres"),
	"longbow" : load("res://Resources/NewEquipment/Weapons/longbow.tres"),
	"longsword" : load("res://Resources/NewEquipment/Weapons/longsword.tres"),
	"mace" : load("res://Resources/NewEquipment/Weapons/mace.tres"),
	"magma_greatsword" : load("res://Resources/NewEquipment/Weapons/magma_greatsword.tres"),
	"masamune" : load("res://Resources/NewEquipment/Weapons/masamune.tres"),
	"morning_star" : load("res://Resources/NewEquipment/Weapons/morning_star.tres"),
	"muramasa" : load("res://Resources/NewEquipment/Weapons/muramasa.tres"),
	"occult_wand" : load("res://Resources/NewEquipment/Weapons/occult_wand.tres"),
	"phantasm" : load("res://Resources/NewEquipment/Weapons/phantasm.tres"),
	"pike" : load("res://Resources/NewEquipment/Weapons/pike.tres"),
	"rapier" : load("res://Resources/NewEquipment/Weapons/rapier.tres"),
	"recurve_bow" : load("res://Resources/NewEquipment/Weapons/recurve_bow.tres"),
	"rhomphaia" : load("res://Resources/NewEquipment/Weapons/rhomphaia.tres"),
	"rondel_dagger" : load("res://Resources/NewEquipment/Weapons/rondel_dagger.tres"),
	"ruby_staff_new" : load("res://Resources/NewEquipment/Weapons/ruby_staff_new.tres"),
	"sapphire_staff" : load("res://Resources/NewEquipment/Weapons/sapphire_staff.tres"),
	"scimitar" : load("res://Resources/NewEquipment/Weapons/scimitar.tres"),
	"scythe" : load("res://Resources/NewEquipment/Weapons/scythe.tres"),
	"shamshir" : load("res://Resources/NewEquipment/Weapons/shamshir.tres"),
	"shortbow" : load("res://Resources/NewEquipment/Weapons/shortbow.tres"),
	"shortsword" : load("res://Resources/NewEquipment/Weapons/shortsword.tres"),
	"shotel" : load("res://Resources/NewEquipment/Weapons/shotel.tres"),
	"sling_new" : load("res://Resources/NewEquipment/Weapons/sling_new.tres"),
	"spear" : load("res://Resources/NewEquipment/Weapons/spear.tres"),
	"staff_of_pain" : load("res://Resources/NewEquipment/Weapons/staff_of_pain.tres"),
	"star_rod" : load("res://Resources/NewEquipment/Weapons/star_rod.tres"),
	"throwing_axe" : load("res://Resources/NewEquipment/Weapons/throwing_axe.tres"),
	"topaz_staff" : load("res://Resources/NewEquipment/Weapons/topaz_staff.tres"),
	"trident" : load("res://Resources/NewEquipment/Weapons/trident.tres"),
	"twisted_sword" : load("res://Resources/NewEquipment/Weapons/twisted_sword.tres"),
	"umbral_dagger" : load("res://Resources/NewEquipment/Weapons/umbral_dagger.tres"),
	"void_blade" : load("res://Resources/NewEquipment/Weapons/void_blade.tres"),
	"wand_of_disintigration" : load("res://Resources/NewEquipment/Weapons/wand_of_disintigration.tres"),
	"warbrand" : load("res://Resources/NewEquipment/Weapons/warbrand.tres"),
	"water_staff" : load("res://Resources/NewEquipment/Weapons/water_staff.tres"),
	"whip" : load("res://Resources/NewEquipment/Weapons/whip.tres")}


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
var OldAction_AttacksDictionary = {
	"bite" : load("res://Resources/OldAction/Attacks/Enemies/bite.tres"),
	"leech_bolt_1_enemy" : load("res://Resources/OldAction/Attacks/Enemies/leech_bolt_1_enemy.tres"),
	"mandragora" : load("res://Resources/OldAction/Attacks/Enemies/mandragora.tres"),
	"multi_peck" : load("res://Resources/OldAction/Attacks/Enemies/multi_peck.tres"),
	"peck" : load("res://Resources/OldAction/Attacks/Enemies/peck.tres"),
	"ram" : load("res://Resources/OldAction/Attacks/Enemies/ram.tres"),
	"vampiric_bite" : load("res://Resources/OldAction/Attacks/Enemies/vampiric_bite.tres"),
	"bludgeon" : load("res://Resources/OldAction/Attacks/Tutorial/bludgeon.tres"),
	"claw_zombie" : load("res://Resources/OldAction/Attacks/Tutorial/claw_zombie.tres"),
	"knifehand" : load("res://Resources/OldAction/Attacks/Tutorial/knifehand.tres"),
	"knifehand_2" : load("res://Resources/OldAction/Attacks/Tutorial/knifehand_2.tres"),
	"nibble" : load("res://Resources/OldAction/Attacks/Tutorial/nibble.tres"),
	"psy_blast" : load("res://Resources/OldAction/Attacks/Tutorial/psy_blast.tres"),
	"psy_blast_2" : load("res://Resources/OldAction/Attacks/Tutorial/psy_blast_2.tres"),
	"punch" : load("res://Resources/OldAction/Attacks/Tutorial/punch.tres"),
	"punch_2" : load("res://Resources/OldAction/Attacks/Tutorial/punch_2.tres"),
	"weak_stab" : load("res://Resources/OldAction/Attacks/Tutorial/weak_stab.tres"),
	"claw_basic" : load("res://Resources/OldAction/Attacks/claw_basic.tres"),
	"dagger_basic" : load("res://Resources/OldAction/Attacks/dagger_basic.tres"),
	"dart_basic" : load("res://Resources/OldAction/Attacks/dart_basic.tres"),
	"executioners_sword_basic" : load("res://Resources/OldAction/Attacks/executioners_sword_basic.tres"),
	"godpunch" : load("res://Resources/OldAction/Attacks/godpunch.tres"),
	"kinetic_blast" : load("res://Resources/OldAction/Attacks/kinetic_blast.tres"),
	"leech_bolt_1" : load("res://Resources/OldAction/Attacks/leech_bolt_1.tres"),
	"light_laser_1" : load("res://Resources/OldAction/Attacks/light_laser_1.tres"),
	"sling_basic" : load("res://Resources/OldAction/Attacks/sling_basic.tres"),
	"spear_basic" : load("res://Resources/OldAction/Attacks/spear_basic.tres"),
	"triple_peck" : load("res://Resources/OldAction/Attacks/triple_peck.tres")}


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
var OldActor_Floor0Dictionary = {
	"goblin" : load("res://Resources/OldActor/Floor0/goblin.tres"),
	"hobgoblin" : load("res://Resources/OldActor/Floor0/hobgoblin.tres"),
	"orc" : load("res://Resources/OldActor/Floor0/orc.tres"),
	"rat" : load("res://Resources/OldActor/Floor0/rat.tres"),
	"zombie" : load("res://Resources/OldActor/Floor0/zombie.tres")}


func getOldActor_Floor0(resourceName : String) :
	return OldActor_Floor0Dictionary.get(resourceName)

var OldActor_Floor1Dictionary = {
	"cave_mole" : load("res://Resources/OldActor/Floor1/cave_mole.tres"),
	"dire_rat" : load("res://Resources/OldActor/Floor1/dire_rat.tres"),
	"giant_magpie" : load("res://Resources/OldActor/Floor1/giant_magpie.tres"),
	"goose_hydra" : load("res://Resources/OldActor/Floor1/goose_hydra.tres"),
	"kiwi" : load("res://Resources/OldActor/Floor1/kiwi.tres"),
	"mandragora" : load("res://Resources/OldActor/Floor1/mandragora.tres"),
	"manticore" : load("res://Resources/OldActor/Floor1/manticore.tres"),
	"mini_roc" : load("res://Resources/OldActor/Floor1/mini_roc.tres"),
	"mountain_goat" : load("res://Resources/OldActor/Floor1/mountain_goat.tres"),
	"mountain_lion" : load("res://Resources/OldActor/Floor1/mountain_lion.tres"),
	"rock_dove" : load("res://Resources/OldActor/Floor1/rock_dove.tres"),
	"vampire_bat" : load("res://Resources/OldActor/Floor1/vampire_bat.tres")}


func getOldActor_Floor1(resourceName : String) :
	return OldActor_Floor1Dictionary.get(resourceName)

var OldActor_MiscDictionary = {
	"human" : load("res://Resources/OldActor/Misc/human.tres")}


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
var OldEquipment_AccessoriesDictionary = {
	"clown_horn" : load("res://Resources/OldEquipment/Accessories/clown_horn.tres"),
	"kiwi" : load("res://Resources/OldEquipment/Accessories/kiwi.tres"),
	"magpie_eye" : load("res://Resources/OldEquipment/Accessories/magpie_eye.tres"),
	"mandragora_potion" : load("res://Resources/OldEquipment/Accessories/mandragora_potion.tres"),
	"manticore_tooth" : load("res://Resources/OldEquipment/Accessories/manticore_tooth.tres"),
	"manticore_venom" : load("res://Resources/OldEquipment/Accessories/manticore_venom.tres"),
	"ram_horn" : load("res://Resources/OldEquipment/Accessories/ram_horn.tres")}


func getOldEquipment_Accessories(resourceName : String) :
	return OldEquipment_AccessoriesDictionary.get(resourceName)

var OldEquipment_ArmorDictionary = {
	"casual" : load("res://Resources/OldEquipment/Armor/casual.tres"),
	"enchanted_robes" : load("res://Resources/OldEquipment/Armor/enchanted_robes.tres"),
	"feathered_cloak" : load("res://Resources/OldEquipment/Armor/feathered_cloak.tres"),
	"lion_armor" : load("res://Resources/OldEquipment/Armor/lion_armor.tres"),
	"manticore_armor" : load("res://Resources/OldEquipment/Armor/manticore_armor.tres"),
	"mole_armor" : load("res://Resources/OldEquipment/Armor/mole_armor.tres"),
	"scraps" : load("res://Resources/OldEquipment/Armor/scraps.tres")}


func getOldEquipment_Armor(resourceName : String) :
	return OldEquipment_ArmorDictionary.get(resourceName)

var OldEquipment_CurrencyDictionary = {
	"pebble" : load("res://Resources/OldEquipment/Currency/pebble.tres")}


func getOldEquipment_Currency(resourceName : String) :
	return OldEquipment_CurrencyDictionary.get(resourceName)

var OldEquipment_WeaponsDictionary = {
	"bat_bat" : load("res://Resources/OldEquipment/Weapons/bat_bat.tres"),
	"bat_staff" : load("res://Resources/OldEquipment/Weapons/bat_staff.tres"),
	"cat_claws" : load("res://Resources/OldEquipment/Weapons/cat_claws.tres"),
	"goose_flail" : load("res://Resources/OldEquipment/Weapons/goose_flail.tres"),
	"kiwi_dagger" : load("res://Resources/OldEquipment/Weapons/kiwi_dagger.tres"),
	"magic_stick_int" : load("res://Resources/OldEquipment/Weapons/magic_stick_int.tres"),
	"magic_stick_str" : load("res://Resources/OldEquipment/Weapons/magic_stick_str.tres"),
	"manticore_dart" : load("res://Resources/OldEquipment/Weapons/manticore_dart.tres"),
	"manticore_wand" : load("res://Resources/OldEquipment/Weapons/manticore_wand.tres"),
	"mole_pickaxe" : load("res://Resources/OldEquipment/Weapons/mole_pickaxe.tres"),
	"ram_spear" : load("res://Resources/OldEquipment/Weapons/ram_spear.tres"),
	"ruby_staff" : load("res://Resources/OldEquipment/Weapons/ruby_staff.tres"),
	"shiv" : load("res://Resources/OldEquipment/Weapons/shiv.tres"),
	"sling" : load("res://Resources/OldEquipment/Weapons/sling.tres"),
	"unarmed_Fighter" : load("res://Resources/OldEquipment/Weapons/unarmed_Fighter.tres"),
	"unarmed_Mage" : load("res://Resources/OldEquipment/Weapons/unarmed_Mage.tres"),
	"unarmed_Rogue" : load("res://Resources/OldEquipment/Weapons/unarmed_Rogue.tres")}


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
var Routine_FilesDictionary = {
	"barefoot" : load("res://Resources/Routine/Files/barefoot.tres"),
	"earth" : load("res://Resources/Routine/Files/earth.tres"),
	"fence_swordfish" : load("res://Resources/Routine/Files/fence_swordfish.tres"),
	"fire" : load("res://Resources/Routine/Files/fire.tres"),
	"hug_cacti" : load("res://Resources/Routine/Files/hug_cacti.tres"),
	"hunt" : load("res://Resources/Routine/Files/hunt.tres"),
	"ice" : load("res://Resources/Routine/Files/ice.tres"),
	"lift_weights" : load("res://Resources/Routine/Files/lift_weights.tres"),
	"offhand" : load("res://Resources/Routine/Files/offhand.tres"),
	"parrying" : load("res://Resources/Routine/Files/parrying.tres"),
	"pickpocket_goblins" : load("res://Resources/Routine/Files/pickpocket_goblins.tres"),
	"punch_walls" : load("res://Resources/Routine/Files/punch_walls.tres"),
	"read_novels" : load("res://Resources/Routine/Files/read_novels.tres"),
	"spar" : load("res://Resources/Routine/Files/spar.tres"),
	"spar_herophile" : load("res://Resources/Routine/Files/spar_herophile.tres"),
	"tactics" : load("res://Resources/Routine/Files/tactics.tres")}


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
var PlayerTexture_bardingDictionary = {
	"black_knight" : load("res://Resources/Textures/PlayerTexture/barding/black_knight.png"),
	"centaur_barding_blue" : load("res://Resources/Textures/PlayerTexture/barding/centaur_barding_blue.png"),
	"centaur_barding_magenta" : load("res://Resources/Textures/PlayerTexture/barding/centaur_barding_magenta.png"),
	"centaur_barding_metal" : load("res://Resources/Textures/PlayerTexture/barding/centaur_barding_metal.png"),
	"centaur_barding_red" : load("res://Resources/Textures/PlayerTexture/barding/centaur_barding_red.png"),
	"lightning_scales" : load("res://Resources/Textures/PlayerTexture/barding/lightning_scales.png"),
	"naga_barding_blue" : load("res://Resources/Textures/PlayerTexture/barding/naga_barding_blue.png"),
	"naga_barding_magenta" : load("res://Resources/Textures/PlayerTexture/barding/naga_barding_magenta.png"),
	"naga_barding_metal" : load("res://Resources/Textures/PlayerTexture/barding/naga_barding_metal.png"),
	"naga_barding_red" : load("res://Resources/Textures/PlayerTexture/barding/naga_barding_red.png")}


func getPlayerTexture_barding(resourceName : String) :
	return PlayerTexture_bardingDictionary.get(resourceName)

var PlayerTexture_baseDictionary = {
	"ahuman_female" : load("res://Resources/Textures/PlayerTexture/base/ahuman_female.png"),
	"ahuman_male" : load("res://Resources/Textures/PlayerTexture/base/ahuman_male.png"),
	"centaur_brown_female" : load("res://Resources/Textures/PlayerTexture/base/centaur_brown_female.png"),
	"centaur_brown_male" : load("res://Resources/Textures/PlayerTexture/base/centaur_brown_male.png"),
	"centaur_darkbrown_female" : load("res://Resources/Textures/PlayerTexture/base/centaur_darkbrown_female.png"),
	"centaur_darkbrown_male" : load("res://Resources/Textures/PlayerTexture/base/centaur_darkbrown_male.png"),
	"centaur_darkgrey_female" : load("res://Resources/Textures/PlayerTexture/base/centaur_darkgrey_female.png"),
	"centaur_darkgrey_male" : load("res://Resources/Textures/PlayerTexture/base/centaur_darkgrey_male.png"),
	"centaur_lightbrown_female" : load("res://Resources/Textures/PlayerTexture/base/centaur_lightbrown_female.png"),
	"centaur_lightbrown_male" : load("res://Resources/Textures/PlayerTexture/base/centaur_lightbrown_male.png"),
	"centaur_lightgrey_female" : load("res://Resources/Textures/PlayerTexture/base/centaur_lightgrey_female.png"),
	"centaur_lightgrey_male" : load("res://Resources/Textures/PlayerTexture/base/centaur_lightgrey_male.png"),
	"demigod_male" : load("res://Resources/Textures/PlayerTexture/base/demigod_male.png"),
	"demonspawn_black_female" : load("res://Resources/Textures/PlayerTexture/base/demonspawn_black_female.png"),
	"demonspawn_black_male" : load("res://Resources/Textures/PlayerTexture/base/demonspawn_black_male.png"),
	"demonspawn_pink" : load("res://Resources/Textures/PlayerTexture/base/demonspawn_pink.png"),
	"demonspawn_red_female" : load("res://Resources/Textures/PlayerTexture/base/demonspawn_red_female.png"),
	"demonspawn_red_male" : load("res://Resources/Textures/PlayerTexture/base/demonspawn_red_male.png"),
	"draconian_black_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_black_female.png"),
	"draconian_black_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_black_male.png"),
	"draconian_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_female.png"),
	"draconian_gold_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_gold_female.png"),
	"draconian_gold_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_gold_male.png"),
	"draconian_gray_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_gray_female.png"),
	"draconian_gray_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_gray_male.png"),
	"draconian_green_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_green_female.png"),
	"draconian_green_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_green_male.png"),
	"draconian_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_male.png"),
	"draconian_mottled_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_mottled_female.png"),
	"draconian_mottled_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_mottled_male.png"),
	"draconian_pale_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_pale_female.png"),
	"draconian_pale_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_pale_male.png"),
	"draconian_purple_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_purple_female.png"),
	"draconian_purple_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_purple_male.png"),
	"draconian_red_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_red_female.png"),
	"draconian_red_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_red_male.png"),
	"draconian_white_female" : load("res://Resources/Textures/PlayerTexture/base/draconian_white_female.png"),
	"draconian_white_male" : load("res://Resources/Textures/PlayerTexture/base/draconian_white_male.png"),
	"dwarf_female" : load("res://Resources/Textures/PlayerTexture/base/dwarf_female.png"),
	"dwarf_male" : load("res://Resources/Textures/PlayerTexture/base/dwarf_male.png"),
	"elf_female" : load("res://Resources/Textures/PlayerTexture/base/elf_female.png"),
	"elf_male" : load("res://Resources/Textures/PlayerTexture/base/elf_male.png"),
	"formicid" : load("res://Resources/Textures/PlayerTexture/base/formicid.png"),
	"gargoyle_female" : load("res://Resources/Textures/PlayerTexture/base/gargoyle_female.png"),
	"gargoyle_male" : load("res://Resources/Textures/PlayerTexture/base/gargoyle_male.png"),
	"ghoul" : load("res://Resources/Textures/PlayerTexture/base/ghoul.png"),
	"ghoul_2_female" : load("res://Resources/Textures/PlayerTexture/base/ghoul_2_female.png"),
	"ghoul_2_male" : load("res://Resources/Textures/PlayerTexture/base/ghoul_2_male.png"),
	"gnome_female" : load("res://Resources/Textures/PlayerTexture/base/gnome_female.png"),
	"gnome_male" : load("res://Resources/Textures/PlayerTexture/base/gnome_male.png"),
	"halfling_female" : load("res://Resources/Textures/PlayerTexture/base/halfling_female.png"),
	"halfling_male" : load("res://Resources/Textures/PlayerTexture/base/halfling_male.png"),
	"kenku_winged_female" : load("res://Resources/Textures/PlayerTexture/base/kenku_winged_female.png"),
	"kenku_winged_male" : load("res://Resources/Textures/PlayerTexture/base/kenku_winged_male.png"),
	"kenku_wingless_female" : load("res://Resources/Textures/PlayerTexture/base/kenku_wingless_female.png"),
	"kenku_wingless_male" : load("res://Resources/Textures/PlayerTexture/base/kenku_wingless_male.png"),
	"kobold_female_new" : load("res://Resources/Textures/PlayerTexture/base/kobold_female_new.png"),
	"kobold_female_old" : load("res://Resources/Textures/PlayerTexture/base/kobold_female_old.png"),
	"kobold_male_new" : load("res://Resources/Textures/PlayerTexture/base/kobold_male_new.png"),
	"kobold_male_old" : load("res://Resources/Textures/PlayerTexture/base/kobold_male_old.png"),
	"lorc_female_0" : load("res://Resources/Textures/PlayerTexture/base/lorc_female_0.png"),
	"lorc_female_1" : load("res://Resources/Textures/PlayerTexture/base/lorc_female_1.png"),
	"lorc_female_2" : load("res://Resources/Textures/PlayerTexture/base/lorc_female_2.png"),
	"lorc_female_3" : load("res://Resources/Textures/PlayerTexture/base/lorc_female_3.png"),
	"lorc_female_4" : load("res://Resources/Textures/PlayerTexture/base/lorc_female_4.png"),
	"lorc_female_5" : load("res://Resources/Textures/PlayerTexture/base/lorc_female_5.png"),
	"lorc_female_6" : load("res://Resources/Textures/PlayerTexture/base/lorc_female_6.png"),
	"lorc_male_0" : load("res://Resources/Textures/PlayerTexture/base/lorc_male_0.png"),
	"lorc_male_1" : load("res://Resources/Textures/PlayerTexture/base/lorc_male_1.png"),
	"lorc_male_2" : load("res://Resources/Textures/PlayerTexture/base/lorc_male_2.png"),
	"lorc_male_3" : load("res://Resources/Textures/PlayerTexture/base/lorc_male_3.png"),
	"lorc_male_4" : load("res://Resources/Textures/PlayerTexture/base/lorc_male_4.png"),
	"lorc_male_5" : load("res://Resources/Textures/PlayerTexture/base/lorc_male_5.png"),
	"lorc_male_6" : load("res://Resources/Textures/PlayerTexture/base/lorc_male_6.png"),
	"merfolk_female" : load("res://Resources/Textures/PlayerTexture/base/merfolk_female.png"),
	"merfolk_male" : load("res://Resources/Textures/PlayerTexture/base/merfolk_male.png"),
	"merfolk_water_female" : load("res://Resources/Textures/PlayerTexture/base/merfolk_water_female.png"),
	"merfolk_water_male" : load("res://Resources/Textures/PlayerTexture/base/merfolk_water_male.png"),
	"minotaur_brown_1_male" : load("res://Resources/Textures/PlayerTexture/base/minotaur_brown_1_male.png"),
	"minotaur_brown_2_male" : load("res://Resources/Textures/PlayerTexture/base/minotaur_brown_2_male.png"),
	"minotaur_female" : load("res://Resources/Textures/PlayerTexture/base/minotaur_female.png"),
	"minotaur_male" : load("res://Resources/Textures/PlayerTexture/base/minotaur_male.png"),
	"mummy_female" : load("res://Resources/Textures/PlayerTexture/base/mummy_female.png"),
	"mummy_male" : load("res://Resources/Textures/PlayerTexture/base/mummy_male.png"),
	"naga_blue_female" : load("res://Resources/Textures/PlayerTexture/base/naga_blue_female.png"),
	"naga_blue_male" : load("res://Resources/Textures/PlayerTexture/base/naga_blue_male.png"),
	"naga_darkgreen_female" : load("res://Resources/Textures/PlayerTexture/base/naga_darkgreen_female.png"),
	"naga_darkgreen_male" : load("res://Resources/Textures/PlayerTexture/base/naga_darkgreen_male.png"),
	"naga_female" : load("res://Resources/Textures/PlayerTexture/base/naga_female.png"),
	"naga_lightgreen_female" : load("res://Resources/Textures/PlayerTexture/base/naga_lightgreen_female.png"),
	"naga_lightgreen_male" : load("res://Resources/Textures/PlayerTexture/base/naga_lightgreen_male.png"),
	"naga_male" : load("res://Resources/Textures/PlayerTexture/base/naga_male.png"),
	"naga_red_female" : load("res://Resources/Textures/PlayerTexture/base/naga_red_female.png"),
	"naga_red_male" : load("res://Resources/Textures/PlayerTexture/base/naga_red_male.png"),
	"octopode_1" : load("res://Resources/Textures/PlayerTexture/base/octopode_1.png"),
	"octopode_2" : load("res://Resources/Textures/PlayerTexture/base/octopode_2.png"),
	"octopode_3" : load("res://Resources/Textures/PlayerTexture/base/octopode_3.png"),
	"octopode_4" : load("res://Resources/Textures/PlayerTexture/base/octopode_4.png"),
	"octopode_5" : load("res://Resources/Textures/PlayerTexture/base/octopode_5.png"),
	"ogre_female" : load("res://Resources/Textures/PlayerTexture/base/ogre_female.png"),
	"ogre_male" : load("res://Resources/Textures/PlayerTexture/base/ogre_male.png"),
	"orc_female" : load("res://Resources/Textures/PlayerTexture/base/orc_female.png"),
	"orc_male" : load("res://Resources/Textures/PlayerTexture/base/orc_male.png"),
	"shadow" : load("res://Resources/Textures/PlayerTexture/base/shadow.png"),
	"spriggan_female" : load("res://Resources/Textures/PlayerTexture/base/spriggan_female.png"),
	"spriggan_male" : load("res://Resources/Textures/PlayerTexture/base/spriggan_male.png"),
	"tengu_wingless_brown_female" : load("res://Resources/Textures/PlayerTexture/base/tengu_wingless_brown_female.png"),
	"tengu_wingless_brown_male" : load("res://Resources/Textures/PlayerTexture/base/tengu_wingless_brown_male.png"),
	"troll_female" : load("res://Resources/Textures/PlayerTexture/base/troll_female.png"),
	"troll_male" : load("res://Resources/Textures/PlayerTexture/base/troll_male.png"),
	"vampire_female" : load("res://Resources/Textures/PlayerTexture/base/vampire_female.png"),
	"vampire_male" : load("res://Resources/Textures/PlayerTexture/base/vampire_male.png"),
	"zdeep_dwarf_female" : load("res://Resources/Textures/PlayerTexture/base/zdeep_dwarf_female.png"),
	"zdeep_dwarf_male" : load("res://Resources/Textures/PlayerTexture/base/zdeep_dwarf_male.png"),
	"zdeep_elf_female" : load("res://Resources/Textures/PlayerTexture/base/zdeep_elf_female.png"),
	"zdeep_elf_male" : load("res://Resources/Textures/PlayerTexture/base/zdeep_elf_male.png")}


func getPlayerTexture_base(resourceName : String) :
	return PlayerTexture_baseDictionary.get(resourceName)

var PlayerTexture_beardDictionary = {
	"long_black" : load("res://Resources/Textures/PlayerTexture/beard/long_black.png"),
	"long_green" : load("res://Resources/Textures/PlayerTexture/beard/long_green.png"),
	"long_red" : load("res://Resources/Textures/PlayerTexture/beard/long_red.png"),
	"long_white" : load("res://Resources/Textures/PlayerTexture/beard/long_white.png"),
	"long_yellow" : load("res://Resources/Textures/PlayerTexture/beard/long_yellow.png"),
	"pj" : load("res://Resources/Textures/PlayerTexture/beard/pj.png"),
	"short_black" : load("res://Resources/Textures/PlayerTexture/beard/short_black.png"),
	"short_green" : load("res://Resources/Textures/PlayerTexture/beard/short_green.png"),
	"short_red" : load("res://Resources/Textures/PlayerTexture/beard/short_red.png"),
	"short_white" : load("res://Resources/Textures/PlayerTexture/beard/short_white.png"),
	"short_yellow" : load("res://Resources/Textures/PlayerTexture/beard/short_yellow.png")}


func getPlayerTexture_beard(resourceName : String) :
	return PlayerTexture_beardDictionary.get(resourceName)

var PlayerTexture_bodyDictionary = {
	"animal_skin" : load("res://Resources/Textures/PlayerTexture/body/animal_skin.png"),
	"aragorn" : load("res://Resources/Textures/PlayerTexture/body/aragorn.png"),
	"aragorn_2" : load("res://Resources/Textures/PlayerTexture/body/aragorn_2.png"),
	"armor_blue_gold" : load("res://Resources/Textures/PlayerTexture/body/armor_blue_gold.png"),
	"armor_mummy" : load("res://Resources/Textures/PlayerTexture/body/armor_mummy.png"),
	"arwen" : load("res://Resources/Textures/PlayerTexture/body/arwen.png"),
	"banded" : load("res://Resources/Textures/PlayerTexture/body/banded.png"),
	"banded_2" : load("res://Resources/Textures/PlayerTexture/body/banded_2.png"),
	"belt_1" : load("res://Resources/Textures/PlayerTexture/body/belt_1.png"),
	"belt_2" : load("res://Resources/Textures/PlayerTexture/body/belt_2.png"),
	"bikini_red" : load("res://Resources/Textures/PlayerTexture/body/bikini_red.png"),
	"bloody" : load("res://Resources/Textures/PlayerTexture/body/bloody.png"),
	"boromir" : load("res://Resources/Textures/PlayerTexture/body/boromir.png"),
	"bplate_green" : load("res://Resources/Textures/PlayerTexture/body/bplate_green.png"),
	"bplate_metal_1" : load("res://Resources/Textures/PlayerTexture/body/bplate_metal_1.png"),
	"breast_black" : load("res://Resources/Textures/PlayerTexture/body/breast_black.png"),
	"chainmail" : load("res://Resources/Textures/PlayerTexture/body/chainmail.png"),
	"chainmail_3" : load("res://Resources/Textures/PlayerTexture/body/chainmail_3.png"),
	"china_red" : load("res://Resources/Textures/PlayerTexture/body/china_red.png"),
	"china_red_2" : load("res://Resources/Textures/PlayerTexture/body/china_red_2.png"),
	"chunli" : load("res://Resources/Textures/PlayerTexture/body/chunli.png"),
	"coat_black" : load("res://Resources/Textures/PlayerTexture/body/coat_black.png"),
	"coat_red" : load("res://Resources/Textures/PlayerTexture/body/coat_red.png"),
	"crystal_plate" : load("res://Resources/Textures/PlayerTexture/body/crystal_plate.png"),
	"dragon_armor_blue_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_blue_new.png"),
	"dragon_armor_blue_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_blue_old.png"),
	"dragon_armor_brown_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_brown_new.png"),
	"dragon_armor_brown_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_brown_old.png"),
	"dragon_armor_cyan_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_cyan_new.png"),
	"dragon_armor_cyan_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_cyan_old.png"),
	"dragon_armor_gold_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_gold_new.png"),
	"dragon_armor_gold_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_gold_old.png"),
	"dragon_armor_green" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_green.png"),
	"dragon_armor_magenta_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_magenta_new.png"),
	"dragon_armor_magenta_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_magenta_old.png"),
	"dragon_armor_pearl" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_pearl.png"),
	"dragon_armor_quicksilver" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_quicksilver.png"),
	"dragon_armor_shadow" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_shadow.png"),
	"dragon_armor_white_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_white_new.png"),
	"dragon_armor_white_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_armor_white_old.png"),
	"dragon_scale_blue_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_blue_new.png"),
	"dragon_scale_blue_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_blue_old.png"),
	"dragon_scale_brown_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_brown_new.png"),
	"dragon_scale_brown_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_brown_old.png"),
	"dragon_scale_cyan_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_cyan_new.png"),
	"dragon_scale_cyan_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_cyan_old.png"),
	"dragon_scale_gold_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_gold_new.png"),
	"dragon_scale_gold_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_gold_old.png"),
	"dragon_scale_green" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_green.png"),
	"dragon_scale_magenta_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_magenta_new.png"),
	"dragon_scale_magenta_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_magenta_old.png"),
	"dragon_scale_pearl" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_pearl.png"),
	"dragon_scale_quicksilver" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_quicksilver.png"),
	"dragon_scale_shadow" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_shadow.png"),
	"dragon_scale_white_new" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_white_new.png"),
	"dragon_scale_white_old" : load("res://Resources/Textures/PlayerTexture/body/dragon_scale_white_old.png"),
	"dress_green" : load("res://Resources/Textures/PlayerTexture/body/dress_green.png"),
	"dress_white" : load("res://Resources/Textures/PlayerTexture/body/dress_white.png"),
	"faerie_dragon_armor" : load("res://Resources/Textures/PlayerTexture/body/faerie_dragon_armor.png"),
	"frodo" : load("res://Resources/Textures/PlayerTexture/body/frodo.png"),
	"gandalf_g" : load("res://Resources/Textures/PlayerTexture/body/gandalf_g.png"),
	"gil-galad" : load("res://Resources/Textures/PlayerTexture/body/gil-galad.png"),
	"gimli" : load("res://Resources/Textures/PlayerTexture/body/gimli.png"),
	"green_chain" : load("res://Resources/Textures/PlayerTexture/body/green_chain.png"),
	"green_susp" : load("res://Resources/Textures/PlayerTexture/body/green_susp.png"),
	"half_plate" : load("res://Resources/Textures/PlayerTexture/body/half_plate.png"),
	"half_plate_2" : load("res://Resources/Textures/PlayerTexture/body/half_plate_2.png"),
	"half_plate_3" : load("res://Resources/Textures/PlayerTexture/body/half_plate_3.png"),
	"isildur" : load("res://Resources/Textures/PlayerTexture/body/isildur.png"),
	"jacket_2" : load("res://Resources/Textures/PlayerTexture/body/jacket_2.png"),
	"jacket_3" : load("res://Resources/Textures/PlayerTexture/body/jacket_3.png"),
	"jacket_stud" : load("res://Resources/Textures/PlayerTexture/body/jacket_stud.png"),
	"jessica" : load("res://Resources/Textures/PlayerTexture/body/jessica.png"),
	"karate" : load("res://Resources/Textures/PlayerTexture/body/karate.png"),
	"karate_2" : load("res://Resources/Textures/PlayerTexture/body/karate_2.png"),
	"lears_chain_mail" : load("res://Resources/Textures/PlayerTexture/body/lears_chain_mail.png"),
	"leather_2" : load("res://Resources/Textures/PlayerTexture/body/leather_2.png"),
	"leather_armor" : load("res://Resources/Textures/PlayerTexture/body/leather_armor.png"),
	"leather_armor_2" : load("res://Resources/Textures/PlayerTexture/body/leather_armor_2.png"),
	"leather_armor_3" : load("res://Resources/Textures/PlayerTexture/body/leather_armor_3.png"),
	"leather_green" : load("res://Resources/Textures/PlayerTexture/body/leather_green.png"),
	"leather_heavy" : load("res://Resources/Textures/PlayerTexture/body/leather_heavy.png"),
	"leather_jacket" : load("res://Resources/Textures/PlayerTexture/body/leather_jacket.png"),
	"leather_metal" : load("res://Resources/Textures/PlayerTexture/body/leather_metal.png"),
	"leather_red" : load("res://Resources/Textures/PlayerTexture/body/leather_red.png"),
	"leather_short" : load("res://Resources/Textures/PlayerTexture/body/leather_short.png"),
	"leather_stud" : load("res://Resources/Textures/PlayerTexture/body/leather_stud.png"),
	"legolas" : load("res://Resources/Textures/PlayerTexture/body/legolas.png"),
	"maxwell_new" : load("res://Resources/Textures/PlayerTexture/body/maxwell_new.png"),
	"maxwell_old" : load("res://Resources/Textures/PlayerTexture/body/maxwell_old.png"),
	"merry" : load("res://Resources/Textures/PlayerTexture/body/merry.png"),
	"mesh_black" : load("res://Resources/Textures/PlayerTexture/body/mesh_black.png"),
	"mesh_red" : load("res://Resources/Textures/PlayerTexture/body/mesh_red.png"),
	"metal_blue" : load("res://Resources/Textures/PlayerTexture/body/metal_blue.png"),
	"monk_black" : load("res://Resources/Textures/PlayerTexture/body/monk_black.png"),
	"monk_blue" : load("res://Resources/Textures/PlayerTexture/body/monk_blue.png"),
	"neck" : load("res://Resources/Textures/PlayerTexture/body/neck.png"),
	"orange_crystal" : load("res://Resources/Textures/PlayerTexture/body/orange_crystal.png"),
	"pipin" : load("res://Resources/Textures/PlayerTexture/body/pipin.png"),
	"pj" : load("res://Resources/Textures/PlayerTexture/body/pj.png"),
	"plate" : load("res://Resources/Textures/PlayerTexture/body/plate.png"),
	"plate_2" : load("res://Resources/Textures/PlayerTexture/body/plate_2.png"),
	"plate_and_cloth" : load("res://Resources/Textures/PlayerTexture/body/plate_and_cloth.png"),
	"plate_and_cloth_2" : load("res://Resources/Textures/PlayerTexture/body/plate_and_cloth_2.png"),
	"plate_black" : load("res://Resources/Textures/PlayerTexture/body/plate_black.png"),
	"ringmail" : load("res://Resources/Textures/PlayerTexture/body/ringmail.png"),
	"robe_black" : load("res://Resources/Textures/PlayerTexture/body/robe_black.png"),
	"robe_black_gold" : load("res://Resources/Textures/PlayerTexture/body/robe_black_gold.png"),
	"robe_black_hood" : load("res://Resources/Textures/PlayerTexture/body/robe_black_hood.png"),
	"robe_black_red" : load("res://Resources/Textures/PlayerTexture/body/robe_black_red.png"),
	"robe_blue" : load("res://Resources/Textures/PlayerTexture/body/robe_blue.png"),
	"robe_blue_green" : load("res://Resources/Textures/PlayerTexture/body/robe_blue_green.png"),
	"robe_blue_white" : load("res://Resources/Textures/PlayerTexture/body/robe_blue_white.png"),
	"robe_brown" : load("res://Resources/Textures/PlayerTexture/body/robe_brown.png"),
	"robe_brown_2" : load("res://Resources/Textures/PlayerTexture/body/robe_brown_2.png"),
	"robe_brown_3" : load("res://Resources/Textures/PlayerTexture/body/robe_brown_3.png"),
	"robe_clouds" : load("res://Resources/Textures/PlayerTexture/body/robe_clouds.png"),
	"robe_cyan" : load("res://Resources/Textures/PlayerTexture/body/robe_cyan.png"),
	"robe_gray_2" : load("res://Resources/Textures/PlayerTexture/body/robe_gray_2.png"),
	"robe_green" : load("res://Resources/Textures/PlayerTexture/body/robe_green.png"),
	"robe_green_gold" : load("res://Resources/Textures/PlayerTexture/body/robe_green_gold.png"),
	"robe_jester" : load("res://Resources/Textures/PlayerTexture/body/robe_jester.png"),
	"robe_misfortune" : load("res://Resources/Textures/PlayerTexture/body/robe_misfortune.png"),
	"robe_of_night" : load("res://Resources/Textures/PlayerTexture/body/robe_of_night.png"),
	"robe_purple" : load("res://Resources/Textures/PlayerTexture/body/robe_purple.png"),
	"robe_rainbow" : load("res://Resources/Textures/PlayerTexture/body/robe_rainbow.png"),
	"robe_red" : load("res://Resources/Textures/PlayerTexture/body/robe_red.png"),
	"robe_red_2" : load("res://Resources/Textures/PlayerTexture/body/robe_red_2.png"),
	"robe_red_3" : load("res://Resources/Textures/PlayerTexture/body/robe_red_3.png"),
	"robe_red_gold" : load("res://Resources/Textures/PlayerTexture/body/robe_red_gold.png"),
	"robe_white" : load("res://Resources/Textures/PlayerTexture/body/robe_white.png"),
	"robe_white_2" : load("res://Resources/Textures/PlayerTexture/body/robe_white_2.png"),
	"robe_white_blue" : load("res://Resources/Textures/PlayerTexture/body/robe_white_blue.png"),
	"robe_white_green" : load("res://Resources/Textures/PlayerTexture/body/robe_white_green.png"),
	"robe_white_red" : load("res://Resources/Textures/PlayerTexture/body/robe_white_red.png"),
	"robe_yellow" : load("res://Resources/Textures/PlayerTexture/body/robe_yellow.png"),
	"sam" : load("res://Resources/Textures/PlayerTexture/body/sam.png"),
	"saruman" : load("res://Resources/Textures/PlayerTexture/body/saruman.png"),
	"scalemail" : load("res://Resources/Textures/PlayerTexture/body/scalemail.png"),
	"scalemail_2" : load("res://Resources/Textures/PlayerTexture/body/scalemail_2.png"),
	"shirt_black" : load("res://Resources/Textures/PlayerTexture/body/shirt_black.png"),
	"shirt_black_3" : load("res://Resources/Textures/PlayerTexture/body/shirt_black_3.png"),
	"shirt_black_and_cloth" : load("res://Resources/Textures/PlayerTexture/body/shirt_black_and_cloth.png"),
	"shirt_blue" : load("res://Resources/Textures/PlayerTexture/body/shirt_blue.png"),
	"shirt_check" : load("res://Resources/Textures/PlayerTexture/body/shirt_check.png"),
	"shirt_hawaii" : load("res://Resources/Textures/PlayerTexture/body/shirt_hawaii.png"),
	"shirt_vest" : load("res://Resources/Textures/PlayerTexture/body/shirt_vest.png"),
	"shirt_white_1" : load("res://Resources/Textures/PlayerTexture/body/shirt_white_1.png"),
	"shirt_white_2" : load("res://Resources/Textures/PlayerTexture/body/shirt_white_2.png"),
	"shirt_white_3" : load("res://Resources/Textures/PlayerTexture/body/shirt_white_3.png"),
	"shirt_white_yellow" : load("res://Resources/Textures/PlayerTexture/body/shirt_white_yellow.png"),
	"shoulder_pad" : load("res://Resources/Textures/PlayerTexture/body/shoulder_pad.png"),
	"skirt_onep_grey" : load("res://Resources/Textures/PlayerTexture/body/skirt_onep_grey.png"),
	"slit_black" : load("res://Resources/Textures/PlayerTexture/body/slit_black.png"),
	"susp_black" : load("res://Resources/Textures/PlayerTexture/body/susp_black.png"),
	"troll_hide" : load("res://Resources/Textures/PlayerTexture/body/troll_hide.png"),
	"vanhel_1" : load("res://Resources/Textures/PlayerTexture/body/vanhel_1.png"),
	"vest_red" : load("res://Resources/Textures/PlayerTexture/body/vest_red.png"),
	"vest_red_2" : load("res://Resources/Textures/PlayerTexture/body/vest_red_2.png"),
	"zhor" : load("res://Resources/Textures/PlayerTexture/body/zhor.png")}


func getPlayerTexture_body(resourceName : String) :
	return PlayerTexture_bodyDictionary.get(resourceName)

var PlayerTexture_bootsDictionary = {
	"blue_gold" : load("res://Resources/Textures/PlayerTexture/boots/blue_gold.png"),
	"hooves" : load("res://Resources/Textures/PlayerTexture/boots/hooves.png"),
	"long_red" : load("res://Resources/Textures/PlayerTexture/boots/long_red.png"),
	"long_white" : load("res://Resources/Textures/PlayerTexture/boots/long_white.png"),
	"mesh_black" : load("res://Resources/Textures/PlayerTexture/boots/mesh_black.png"),
	"mesh_blue" : load("res://Resources/Textures/PlayerTexture/boots/mesh_blue.png"),
	"mesh_red" : load("res://Resources/Textures/PlayerTexture/boots/mesh_red.png"),
	"mesh_white" : load("res://Resources/Textures/PlayerTexture/boots/mesh_white.png"),
	"middle_brown" : load("res://Resources/Textures/PlayerTexture/boots/middle_brown.png"),
	"middle_brown_2" : load("res://Resources/Textures/PlayerTexture/boots/middle_brown_2.png"),
	"middle_brown_3" : load("res://Resources/Textures/PlayerTexture/boots/middle_brown_3.png"),
	"middle_gold" : load("res://Resources/Textures/PlayerTexture/boots/middle_gold.png"),
	"middle_gray" : load("res://Resources/Textures/PlayerTexture/boots/middle_gray.png"),
	"middle_green" : load("res://Resources/Textures/PlayerTexture/boots/middle_green.png"),
	"middle_purple" : load("res://Resources/Textures/PlayerTexture/boots/middle_purple.png"),
	"middle_ybrown" : load("res://Resources/Textures/PlayerTexture/boots/middle_ybrown.png"),
	"pj" : load("res://Resources/Textures/PlayerTexture/boots/pj.png"),
	"short_brown" : load("res://Resources/Textures/PlayerTexture/boots/short_brown.png"),
	"short_brown_2" : load("res://Resources/Textures/PlayerTexture/boots/short_brown_2.png"),
	"short_purple" : load("res://Resources/Textures/PlayerTexture/boots/short_purple.png"),
	"short_red" : load("res://Resources/Textures/PlayerTexture/boots/short_red.png"),
	"spider" : load("res://Resources/Textures/PlayerTexture/boots/spider.png")}


func getPlayerTexture_boots(resourceName : String) :
	return PlayerTexture_bootsDictionary.get(resourceName)

var PlayerTexture_cloakDictionary = {
	"black" : load("res://Resources/Textures/PlayerTexture/cloak/black.png"),
	"blue" : load("res://Resources/Textures/PlayerTexture/cloak/blue.png"),
	"brown" : load("res://Resources/Textures/PlayerTexture/cloak/brown.png"),
	"cyan" : load("res://Resources/Textures/PlayerTexture/cloak/cyan.png"),
	"dragonskin" : load("res://Resources/Textures/PlayerTexture/cloak/dragonskin.png"),
	"gray" : load("res://Resources/Textures/PlayerTexture/cloak/gray.png"),
	"green" : load("res://Resources/Textures/PlayerTexture/cloak/green.png"),
	"magenta" : load("res://Resources/Textures/PlayerTexture/cloak/magenta.png"),
	"ratskin" : load("res://Resources/Textures/PlayerTexture/cloak/ratskin.png"),
	"red" : load("res://Resources/Textures/PlayerTexture/cloak/red.png"),
	"white" : load("res://Resources/Textures/PlayerTexture/cloak/white.png"),
	"yellow" : load("res://Resources/Textures/PlayerTexture/cloak/yellow.png")}


func getPlayerTexture_cloak(resourceName : String) :
	return PlayerTexture_cloakDictionary.get(resourceName)

var PlayerTexture_draconic_headDictionary = {
	"draconic_head_black" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_black.png"),
	"draconic_head_brown" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_brown.png"),
	"draconic_head_green" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_green.png"),
	"draconic_head_grey" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_grey.png"),
	"draconic_head_mottled" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_mottled.png"),
	"draconic_head_pale" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_pale.png"),
	"draconic_head_purple" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_purple.png"),
	"draconic_head_red" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_red.png"),
	"draconic_head_white" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_white.png"),
	"draconic_head_yellow" : load("res://Resources/Textures/PlayerTexture/draconic_head/draconic_head_yellow.png")}


func getPlayerTexture_draconic_head(resourceName : String) :
	return PlayerTexture_draconic_headDictionary.get(resourceName)

var PlayerTexture_draconic_wingDictionary = {
	"draconic_wing_black" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_black.png"),
	"draconic_wing_brown" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_brown.png"),
	"draconic_wing_green" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_green.png"),
	"draconic_wing_grey" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_grey.png"),
	"draconic_wing_mottled" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_mottled.png"),
	"draconic_wing_pale" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_pale.png"),
	"draconic_wing_purple" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_purple.png"),
	"draconic_wing_red" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_red.png"),
	"draconic_wing_white" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_white.png"),
	"draconic_wing_yellow" : load("res://Resources/Textures/PlayerTexture/draconic_wing/draconic_wing_yellow.png")}


func getPlayerTexture_draconic_wing(resourceName : String) :
	return PlayerTexture_draconic_wingDictionary.get(resourceName)

var PlayerTexture_enchantmentDictionary = {
	"sticky_flame" : load("res://Resources/Textures/PlayerTexture/enchantment/sticky_flame.png")}


func getPlayerTexture_enchantment(resourceName : String) :
	return PlayerTexture_enchantmentDictionary.get(resourceName)

var PlayerTexture_felidsDictionary = {
	"cat_1" : load("res://Resources/Textures/PlayerTexture/felids/cat_1.png"),
	"cat_2" : load("res://Resources/Textures/PlayerTexture/felids/cat_2.png"),
	"cat_3" : load("res://Resources/Textures/PlayerTexture/felids/cat_3.png"),
	"cat_4" : load("res://Resources/Textures/PlayerTexture/felids/cat_4.png"),
	"cat_5" : load("res://Resources/Textures/PlayerTexture/felids/cat_5.png")}


func getPlayerTexture_felids(resourceName : String) :
	return PlayerTexture_felidsDictionary.get(resourceName)

var PlayerTexture_glovesDictionary = {
	"claws" : load("res://Resources/Textures/PlayerTexture/gloves/claws.png"),
	"gauntlet_blue" : load("res://Resources/Textures/PlayerTexture/gloves/gauntlet_blue.png"),
	"glove_black" : load("res://Resources/Textures/PlayerTexture/gloves/glove_black.png"),
	"glove_black_2" : load("res://Resources/Textures/PlayerTexture/gloves/glove_black_2.png"),
	"glove_blue" : load("res://Resources/Textures/PlayerTexture/gloves/glove_blue.png"),
	"glove_brown" : load("res://Resources/Textures/PlayerTexture/gloves/glove_brown.png"),
	"glove_chunli" : load("res://Resources/Textures/PlayerTexture/gloves/glove_chunli.png"),
	"glove_gold" : load("res://Resources/Textures/PlayerTexture/gloves/glove_gold.png"),
	"glove_gray" : load("res://Resources/Textures/PlayerTexture/gloves/glove_gray.png"),
	"glove_grayfist" : load("res://Resources/Textures/PlayerTexture/gloves/glove_grayfist.png"),
	"glove_orange" : load("res://Resources/Textures/PlayerTexture/gloves/glove_orange.png"),
	"glove_purple" : load("res://Resources/Textures/PlayerTexture/gloves/glove_purple.png"),
	"glove_red" : load("res://Resources/Textures/PlayerTexture/gloves/glove_red.png"),
	"glove_short_blue" : load("res://Resources/Textures/PlayerTexture/gloves/glove_short_blue.png"),
	"glove_short_gray" : load("res://Resources/Textures/PlayerTexture/gloves/glove_short_gray.png"),
	"glove_short_green" : load("res://Resources/Textures/PlayerTexture/gloves/glove_short_green.png"),
	"glove_short_red" : load("res://Resources/Textures/PlayerTexture/gloves/glove_short_red.png"),
	"glove_short_white" : load("res://Resources/Textures/PlayerTexture/gloves/glove_short_white.png"),
	"glove_short_yellow" : load("res://Resources/Textures/PlayerTexture/gloves/glove_short_yellow.png"),
	"glove_white" : load("res://Resources/Textures/PlayerTexture/gloves/glove_white.png"),
	"glove_wrist_purple" : load("res://Resources/Textures/PlayerTexture/gloves/glove_wrist_purple.png")}


func getPlayerTexture_gloves(resourceName : String) :
	return PlayerTexture_glovesDictionary.get(resourceName)

var PlayerTexture_hairDictionary = {
	"aragorn" : load("res://Resources/Textures/PlayerTexture/hair/aragorn.png"),
	"arwen" : load("res://Resources/Textures/PlayerTexture/hair/arwen.png"),
	"boromir" : load("res://Resources/Textures/PlayerTexture/hair/boromir.png"),
	"brown_1" : load("res://Resources/Textures/PlayerTexture/hair/brown_1.png"),
	"brown_2" : load("res://Resources/Textures/PlayerTexture/hair/brown_2.png"),
	"djinn_1" : load("res://Resources/Textures/PlayerTexture/hair/djinn_1.png"),
	"djinn_2" : load("res://Resources/Textures/PlayerTexture/hair/djinn_2.png"),
	"elf_black" : load("res://Resources/Textures/PlayerTexture/hair/elf_black.png"),
	"elf_red" : load("res://Resources/Textures/PlayerTexture/hair/elf_red.png"),
	"elf_white" : load("res://Resources/Textures/PlayerTexture/hair/elf_white.png"),
	"elf_yellow" : load("res://Resources/Textures/PlayerTexture/hair/elf_yellow.png"),
	"fem_black" : load("res://Resources/Textures/PlayerTexture/hair/fem_black.png"),
	"fem_red" : load("res://Resources/Textures/PlayerTexture/hair/fem_red.png"),
	"fem_white" : load("res://Resources/Textures/PlayerTexture/hair/fem_white.png"),
	"fem_yellow" : load("res://Resources/Textures/PlayerTexture/hair/fem_yellow.png"),
	"frodo" : load("res://Resources/Textures/PlayerTexture/hair/frodo.png"),
	"green" : load("res://Resources/Textures/PlayerTexture/hair/green.png"),
	"knot_red" : load("res://Resources/Textures/PlayerTexture/hair/knot_red.png"),
	"legolas" : load("res://Resources/Textures/PlayerTexture/hair/legolas.png"),
	"long_black" : load("res://Resources/Textures/PlayerTexture/hair/long_black.png"),
	"long_red" : load("res://Resources/Textures/PlayerTexture/hair/long_red.png"),
	"long_white" : load("res://Resources/Textures/PlayerTexture/hair/long_white.png"),
	"long_yellow" : load("res://Resources/Textures/PlayerTexture/hair/long_yellow.png"),
	"merry" : load("res://Resources/Textures/PlayerTexture/hair/merry.png"),
	"pigtail_red" : load("res://Resources/Textures/PlayerTexture/hair/pigtail_red.png"),
	"pigtails_brown" : load("res://Resources/Textures/PlayerTexture/hair/pigtails_brown.png"),
	"pigtails_yellow" : load("res://Resources/Textures/PlayerTexture/hair/pigtails_yellow.png"),
	"pj" : load("res://Resources/Textures/PlayerTexture/hair/pj.png"),
	"ponytail_yellow" : load("res://Resources/Textures/PlayerTexture/hair/ponytail_yellow.png"),
	"sam" : load("res://Resources/Textures/PlayerTexture/hair/sam.png"),
	"short_black" : load("res://Resources/Textures/PlayerTexture/hair/short_black.png"),
	"short_red" : load("res://Resources/Textures/PlayerTexture/hair/short_red.png"),
	"short_white" : load("res://Resources/Textures/PlayerTexture/hair/short_white.png"),
	"short_yellow" : load("res://Resources/Textures/PlayerTexture/hair/short_yellow.png"),
	"tengu_comb" : load("res://Resources/Textures/PlayerTexture/hair/tengu_comb.png")}


func getPlayerTexture_hair(resourceName : String) :
	return PlayerTexture_hairDictionary.get(resourceName)

var PlayerTexture_haloDictionary = {
	"halo_player" : load("res://Resources/Textures/PlayerTexture/halo/halo_player.png")}


func getPlayerTexture_halo(resourceName : String) :
	return PlayerTexture_haloDictionary.get(resourceName)

var PlayerTexture_hand_leftDictionary = {
	"book_black" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_black.png"),
	"book_blue" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_blue.png"),
	"book_blue_dim" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_blue_dim.png"),
	"book_cyan" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_cyan.png"),
	"book_cyan_dim" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_cyan_dim.png"),
	"book_green" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_green.png"),
	"book_green_dim" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_green_dim.png"),
	"book_magenta" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_magenta.png"),
	"book_magenta_dim" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_magenta_dim.png"),
	"book_red" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_red.png"),
	"book_red_dim" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_red_dim.png"),
	"book_sky" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_sky.png"),
	"book_white" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_white.png"),
	"book_yellow" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_yellow.png"),
	"book_yellow_dim" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/book_yellow_dim.png"),
	"dagger_new" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/dagger_new.png"),
	"dagger_old" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/dagger_old.png"),
	"fire_cyan" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_cyan.png"),
	"fire_dark" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_dark.png"),
	"fire_green" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_green.png"),
	"fire_white" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_white.png"),
	"fire_white_2" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/fire_white_2.png"),
	"flail_great" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/flail_great.png"),
	"flail_great_2" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/flail_great_2.png"),
	"giant_club" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club.png"),
	"giant_club_plain" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club_plain.png"),
	"giant_club_slant" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club_slant.png"),
	"giant_club_spike" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club_spike.png"),
	"giant_club_spike_slant" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/giant_club_spike_slant.png"),
	"great_mace" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/great_mace.png"),
	"great_mace_2" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/great_mace_2.png"),
	"lantern" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/lantern.png"),
	"light_blue" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/light_blue.png"),
	"light_red" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/light_red.png"),
	"light_yellow" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/light_yellow.png"),
	"pj" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/pj.png"),
	"rapier_2" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/rapier_2.png"),
	"sabre" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/sabre.png"),
	"short_sword_slant_2" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/short_sword_slant_2.png"),
	"short_sword_slant_new" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/short_sword_slant_new.png"),
	"short_sword_slant_old" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/short_sword_slant_old.png"),
	"spark" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/spark.png"),
	"torch" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/torch.png"),
	"torch_2" : load("res://Resources/Textures/PlayerTexture/hand_left/misc/torch_2.png"),
	"boromir" : load("res://Resources/Textures/PlayerTexture/hand_left/boromir.png"),
	"buckler_green" : load("res://Resources/Textures/PlayerTexture/hand_left/buckler_green.png"),
	"buckler_rb" : load("res://Resources/Textures/PlayerTexture/hand_left/buckler_rb.png"),
	"buckler_round_2" : load("res://Resources/Textures/PlayerTexture/hand_left/buckler_round_2.png"),
	"buckler_round_3" : load("res://Resources/Textures/PlayerTexture/hand_left/buckler_round_3.png"),
	"buckler_spiral" : load("res://Resources/Textures/PlayerTexture/hand_left/buckler_spiral.png"),
	"bullseye" : load("res://Resources/Textures/PlayerTexture/hand_left/bullseye.png"),
	"gil-galad" : load("res://Resources/Textures/PlayerTexture/hand_left/gil-galad.png"),
	"gong" : load("res://Resources/Textures/PlayerTexture/hand_left/gong.png"),
	"lshield_gold" : load("res://Resources/Textures/PlayerTexture/hand_left/lshield_gold.png"),
	"lshield_green" : load("res://Resources/Textures/PlayerTexture/hand_left/lshield_green.png"),
	"lshield_long_red" : load("res://Resources/Textures/PlayerTexture/hand_left/lshield_long_red.png"),
	"lshield_louise" : load("res://Resources/Textures/PlayerTexture/hand_left/lshield_louise.png"),
	"lshield_quartered" : load("res://Resources/Textures/PlayerTexture/hand_left/lshield_quartered.png"),
	"lshield_spiral" : load("res://Resources/Textures/PlayerTexture/hand_left/lshield_spiral.png"),
	"lshield_teal" : load("res://Resources/Textures/PlayerTexture/hand_left/lshield_teal.png"),
	"shield_dd" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_dd.png"),
	"shield_dd_scion" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_dd_scion.png"),
	"shield_diamond_yellow" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_diamond_yellow.png"),
	"shield_donald" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_donald.png"),
	"shield_draconic_knight" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_draconic_knight.png"),
	"shield_goblin" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_goblin.png"),
	"shield_holy" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_holy.png"),
	"shield_kite_1" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_kite_1.png"),
	"shield_kite_2" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_kite_2.png"),
	"shield_kite_3" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_kite_3.png"),
	"shield_kite_4" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_kite_4.png"),
	"shield_knight_blue" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_knight_blue.png"),
	"shield_knight_gray" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_knight_gray.png"),
	"shield_knight_rw" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_knight_rw.png"),
	"shield_large_dd_dk" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_large_dd_dk.png"),
	"shield_long_cross" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_long_cross.png"),
	"shield_long_red" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_long_red.png"),
	"shield_middle_black" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_black.png"),
	"shield_middle_brown" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_brown.png"),
	"shield_middle_cyan" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_cyan.png"),
	"shield_middle_ethn" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_ethn.png"),
	"shield_middle_gray" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_gray.png"),
	"shield_middle_round" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_round.png"),
	"shield_middle_unicorn" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_middle_unicorn.png"),
	"shield_of_ignorance" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_of_ignorance.png"),
	"shield_of_resistance" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_of_resistance.png"),
	"shield_round_1" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_1.png"),
	"shield_round_2" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_2.png"),
	"shield_round_3" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_3.png"),
	"shield_round_4" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_4.png"),
	"shield_round_5" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_5.png"),
	"shield_round_6" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_6.png"),
	"shield_round_7" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_7.png"),
	"shield_round_small" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_small.png"),
	"shield_round_white" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_round_white.png"),
	"shield_shaman" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_shaman.png"),
	"shield_skull" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_skull.png"),
	"shield_spriggan" : load("res://Resources/Textures/PlayerTexture/hand_left/shield_spriggan.png")}


func getPlayerTexture_hand_left(resourceName : String) :
	return PlayerTexture_hand_leftDictionary.get(resourceName)

var PlayerTexture_hand_rightDictionary = {
	"arc_blade" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/arc_blade.png"),
	"arga_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/arga_new.png"),
	"arga_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/arga_old.png"),
	"asmodeus_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/asmodeus_new.png"),
	"asmodeus_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/asmodeus_old.png"),
	"axe_of_woe" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/axe_of_woe.png"),
	"axe_trog" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/axe_trog.png"),
	"bloodbane_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/bloodbane_new.png"),
	"bloodbane_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/bloodbane_old.png"),
	"blowgun_assassin" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/blowgun_assassin.png"),
	"botono" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/botono.png"),
	"chilly_death_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/chilly_death_new.png"),
	"chilly_death_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/chilly_death_old.png"),
	"crossbow_fire" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/crossbow_fire.png"),
	"crystal_spear_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/crystal_spear_new.png"),
	"crystal_spear_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/crystal_spear_old.png"),
	"cutlass" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/cutlass.png"),
	"dire_lajatang" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/dire_lajatang.png"),
	"dispater_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/dispater_new.png"),
	"dispater_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/dispater_old.png"),
	"doom_knight_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/doom_knight_new.png"),
	"doom_knight_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/doom_knight_old.png"),
	"elemental_staff" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/elemental_staff.png"),
	"eos" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/eos.png"),
	"finisher" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/finisher.png"),
	"firestarter" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/firestarter.png"),
	"flaming_death_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/flaming_death_new.png"),
	"flaming_death_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/flaming_death_old.png"),
	"glaive_of_prune_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/glaive_of_prune_new.png"),
	"glaive_of_prune_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/glaive_of_prune_old.png"),
	"glaive_of_the_guard_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/glaive_of_the_guard_new.png"),
	"glaive_of_the_guard_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/glaive_of_the_guard_old.png"),
	"gyre" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/gyre.png"),
	"jihad" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/jihad.png"),
	"knife_of_accuracy" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/knife_of_accuracy.png"),
	"krishna" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/krishna.png"),
	"leech" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/leech.png"),
	"mace_of_brilliance" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/mace_of_brilliance.png"),
	"mace_of_variability" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/mace_of_variability.png"),
	"majin" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/majin.png"),
	"morg" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/morg.png"),
	"olgreb" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/olgreb.png"),
	"order" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/order.png"),
	"plutonium_sword_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/plutonium_sword_new.png"),
	"plutonium_sword_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/plutonium_sword_old.png"),
	"punk" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/punk.png"),
	"serpent_scourge" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/serpent_scourge.png"),
	"shillelagh" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/shillelagh.png"),
	"singing_sword" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/singing_sword.png"),
	"sniper" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/sniper.png"),
	"spriggans_knife_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/spriggans_knife_new.png"),
	"spriggans_knife_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/spriggans_knife_old.png"),
	"sword_of_power_new" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/sword_of_power_new.png"),
	"sword_of_power_old" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/sword_of_power_old.png"),
	"trident_octopus_king" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/trident_octopus_king.png"),
	"undeadhunter" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/undeadhunter.png"),
	"vampires_tooth" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/vampires_tooth.png"),
	"wucad_mu" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/wucad_mu.png"),
	"wyrmbane" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/wyrmbane.png"),
	"zonguldrok" : load("res://Resources/Textures/PlayerTexture/hand_right/artefact/zonguldrok.png"),
	"bladehands_fe" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/bladehands_fe.png"),
	"bladehands_new" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/bladehands_new.png"),
	"bladehands_old" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/bladehands_old.png"),
	"bladehands_op" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/bladehands_op.png"),
	"bone_lantern" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/bone_lantern.png"),
	"bottle" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/bottle.png"),
	"box" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/box.png"),
	"crystal" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/crystal.png"),
	"deck" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/deck.png"),
	"disc" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/disc.png"),
	"fan" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/fan.png"),
	"fire_blue" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_blue.png"),
	"fire_cyan" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_cyan.png"),
	"fire_dark" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_dark.png"),
	"fire_green" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_green.png"),
	"fire_red" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_red.png"),
	"fire_white" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_white.png"),
	"fire_white_2" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/fire_white_2.png"),
	"head" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/head.png"),
	"horn" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/horn.png"),
	"lantern" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/lantern.png"),
	"light_blue" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/light_blue.png"),
	"light_red" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/light_red.png"),
	"light_yellow" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/light_yellow.png"),
	"orb" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/orb.png"),
	"skull" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/skull.png"),
	"spark" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/spark.png"),
	"stone" : load("res://Resources/Textures/PlayerTexture/hand_right/misc/stone.png"),
	"aragorn" : load("res://Resources/Textures/PlayerTexture/hand_right/aragorn.png"),
	"arwen" : load("res://Resources/Textures/PlayerTexture/hand_right/arwen.png"),
	"axe" : load("res://Resources/Textures/PlayerTexture/hand_right/axe.png"),
	"axe_blood" : load("res://Resources/Textures/PlayerTexture/hand_right/axe_blood.png"),
	"axe_double" : load("res://Resources/Textures/PlayerTexture/hand_right/axe_double.png"),
	"axe_executioner_2" : load("res://Resources/Textures/PlayerTexture/hand_right/axe_executioner_2.png"),
	"axe_executioner_new" : load("res://Resources/Textures/PlayerTexture/hand_right/axe_executioner_new.png"),
	"axe_executioner_old" : load("res://Resources/Textures/PlayerTexture/hand_right/axe_executioner_old.png"),
	"axe_short" : load("res://Resources/Textures/PlayerTexture/hand_right/axe_short.png"),
	"axe_small" : load("res://Resources/Textures/PlayerTexture/hand_right/axe_small.png"),
	"battleaxe" : load("res://Resources/Textures/PlayerTexture/hand_right/battleaxe.png"),
	"battleaxe_2" : load("res://Resources/Textures/PlayerTexture/hand_right/battleaxe_2.png"),
	"black_sword" : load("res://Resources/Textures/PlayerTexture/hand_right/black_sword.png"),
	"black_whip_new" : load("res://Resources/Textures/PlayerTexture/hand_right/black_whip_new.png"),
	"black_whip_old" : load("res://Resources/Textures/PlayerTexture/hand_right/black_whip_old.png"),
	"blessed_blade" : load("res://Resources/Textures/PlayerTexture/hand_right/blessed_blade.png"),
	"blowgun" : load("res://Resources/Textures/PlayerTexture/hand_right/blowgun.png"),
	"boromir" : load("res://Resources/Textures/PlayerTexture/hand_right/boromir.png"),
	"bow" : load("res://Resources/Textures/PlayerTexture/hand_right/bow.png"),
	"bow_2" : load("res://Resources/Textures/PlayerTexture/hand_right/bow_2.png"),
	"bow_3" : load("res://Resources/Textures/PlayerTexture/hand_right/bow_3.png"),
	"bow_blue" : load("res://Resources/Textures/PlayerTexture/hand_right/bow_blue.png"),
	"broad_axe" : load("res://Resources/Textures/PlayerTexture/hand_right/broad_axe.png"),
	"broadsword" : load("res://Resources/Textures/PlayerTexture/hand_right/broadsword.png"),
	"club" : load("res://Resources/Textures/PlayerTexture/hand_right/club.png"),
	"club_2" : load("res://Resources/Textures/PlayerTexture/hand_right/club_2.png"),
	"club_3" : load("res://Resources/Textures/PlayerTexture/hand_right/club_3.png"),
	"club_slant" : load("res://Resources/Textures/PlayerTexture/hand_right/club_slant.png"),
	"crossbow" : load("res://Resources/Textures/PlayerTexture/hand_right/crossbow.png"),
	"crossbow_2" : load("res://Resources/Textures/PlayerTexture/hand_right/crossbow_2.png"),
	"crossbow_3" : load("res://Resources/Textures/PlayerTexture/hand_right/crossbow_3.png"),
	"crossbow_4" : load("res://Resources/Textures/PlayerTexture/hand_right/crossbow_4.png"),
	"d_glaive" : load("res://Resources/Textures/PlayerTexture/hand_right/d_glaive.png"),
	"dagger_new" : load("res://Resources/Textures/PlayerTexture/hand_right/dagger_new.png"),
	"dagger_old" : load("res://Resources/Textures/PlayerTexture/hand_right/dagger_old.png"),
	"dagger_slant_2" : load("res://Resources/Textures/PlayerTexture/hand_right/dagger_slant_2.png"),
	"dagger_slant_new" : load("res://Resources/Textures/PlayerTexture/hand_right/dagger_slant_new.png"),
	"dagger_slant_old" : load("res://Resources/Textures/PlayerTexture/hand_right/dagger_slant_old.png"),
	"dart" : load("res://Resources/Textures/PlayerTexture/hand_right/dart.png"),
	"double_sword_2" : load("res://Resources/Textures/PlayerTexture/hand_right/double_sword_2.png"),
	"double_sword_3" : load("res://Resources/Textures/PlayerTexture/hand_right/double_sword_3.png"),
	"double_sword_new" : load("res://Resources/Textures/PlayerTexture/hand_right/double_sword_new.png"),
	"double_sword_old" : load("res://Resources/Textures/PlayerTexture/hand_right/double_sword_old.png"),
	"enchantress_dagger" : load("res://Resources/Textures/PlayerTexture/hand_right/enchantress_dagger.png"),
	"eveningstar_2" : load("res://Resources/Textures/PlayerTexture/hand_right/eveningstar_2.png"),
	"eveningstar_new" : load("res://Resources/Textures/PlayerTexture/hand_right/eveningstar_new.png"),
	"eveningstar_old" : load("res://Resources/Textures/PlayerTexture/hand_right/eveningstar_old.png"),
	"falchion_2" : load("res://Resources/Textures/PlayerTexture/hand_right/falchion_2.png"),
	"falchion_new" : load("res://Resources/Textures/PlayerTexture/hand_right/falchion_new.png"),
	"falchion_old" : load("res://Resources/Textures/PlayerTexture/hand_right/falchion_old.png"),
	"flail_ball_2_new" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_2_new.png"),
	"flail_ball_2_old" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_2_old.png"),
	"flail_ball_3" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_3.png"),
	"flail_ball_4" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_4.png"),
	"flail_ball_new" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_new.png"),
	"flail_ball_old" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_ball_old.png"),
	"flail_balls" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_balls.png"),
	"flail_great" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_great.png"),
	"flail_great_2" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_great_2.png"),
	"flail_spike" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_spike.png"),
	"flail_spike_2" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_spike_2.png"),
	"flail_stick" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_stick.png"),
	"flail_stick_slant" : load("res://Resources/Textures/PlayerTexture/hand_right/flail_stick_slant.png"),
	"fork_2" : load("res://Resources/Textures/PlayerTexture/hand_right/fork_2.png"),
	"frodo" : load("res://Resources/Textures/PlayerTexture/hand_right/frodo.png"),
	"gandalf" : load("res://Resources/Textures/PlayerTexture/hand_right/gandalf.png"),
	"giant_club" : load("res://Resources/Textures/PlayerTexture/hand_right/giant_club.png"),
	"giant_club_plain" : load("res://Resources/Textures/PlayerTexture/hand_right/giant_club_plain.png"),
	"giant_club_slant" : load("res://Resources/Textures/PlayerTexture/hand_right/giant_club_slant.png"),
	"giant_club_spike" : load("res://Resources/Textures/PlayerTexture/hand_right/giant_club_spike.png"),
	"giant_club_spike_slant" : load("res://Resources/Textures/PlayerTexture/hand_right/giant_club_spike_slant.png"),
	"gimli" : load("res://Resources/Textures/PlayerTexture/hand_right/gimli.png"),
	"glaive_2" : load("res://Resources/Textures/PlayerTexture/hand_right/glaive_2.png"),
	"glaive_3" : load("res://Resources/Textures/PlayerTexture/hand_right/glaive_3.png"),
	"glaive_new" : load("res://Resources/Textures/PlayerTexture/hand_right/glaive_new.png"),
	"glaive_old" : load("res://Resources/Textures/PlayerTexture/hand_right/glaive_old.png"),
	"glaive_three" : load("res://Resources/Textures/PlayerTexture/hand_right/glaive_three.png"),
	"glaive_three_2" : load("res://Resources/Textures/PlayerTexture/hand_right/glaive_three_2.png"),
	"great_axe" : load("res://Resources/Textures/PlayerTexture/hand_right/great_axe.png"),
	"great_bow" : load("res://Resources/Textures/PlayerTexture/hand_right/great_bow.png"),
	"great_mace" : load("res://Resources/Textures/PlayerTexture/hand_right/great_mace.png"),
	"great_mace_2" : load("res://Resources/Textures/PlayerTexture/hand_right/great_mace_2.png"),
	"great_staff" : load("res://Resources/Textures/PlayerTexture/hand_right/great_staff.png"),
	"great_sword" : load("res://Resources/Textures/PlayerTexture/hand_right/great_sword.png"),
	"great_sword_slant_2" : load("res://Resources/Textures/PlayerTexture/hand_right/great_sword_slant_2.png"),
	"great_sword_slant_new" : load("res://Resources/Textures/PlayerTexture/hand_right/great_sword_slant_new.png"),
	"great_sword_slant_old" : load("res://Resources/Textures/PlayerTexture/hand_right/great_sword_slant_old.png"),
	"greatsling" : load("res://Resources/Textures/PlayerTexture/hand_right/greatsling.png"),
	"halberd_new" : load("res://Resources/Textures/PlayerTexture/hand_right/halberd_new.png"),
	"halberd_old" : load("res://Resources/Textures/PlayerTexture/hand_right/halberd_old.png"),
	"hammer_2_new" : load("res://Resources/Textures/PlayerTexture/hand_right/hammer_2_new.png"),
	"hammer_2_old" : load("res://Resources/Textures/PlayerTexture/hand_right/hammer_2_old.png"),
	"hammer_3" : load("res://Resources/Textures/PlayerTexture/hand_right/hammer_3.png"),
	"hammer_new" : load("res://Resources/Textures/PlayerTexture/hand_right/hammer_new.png"),
	"hammer_old" : load("res://Resources/Textures/PlayerTexture/hand_right/hammer_old.png"),
	"hand_axe_2" : load("res://Resources/Textures/PlayerTexture/hand_right/hand_axe_2.png"),
	"hand_axe_new" : load("res://Resources/Textures/PlayerTexture/hand_right/hand_axe_new.png"),
	"hand_axe_old" : load("res://Resources/Textures/PlayerTexture/hand_right/hand_axe_old.png"),
	"hand_crossbow" : load("res://Resources/Textures/PlayerTexture/hand_right/hand_crossbow.png"),
	"heavy_sword" : load("res://Resources/Textures/PlayerTexture/hand_right/heavy_sword.png"),
	"holy_scourge_1" : load("res://Resources/Textures/PlayerTexture/hand_right/holy_scourge_1.png"),
	"holy_scourge_2" : load("res://Resources/Textures/PlayerTexture/hand_right/holy_scourge_2.png"),
	"hook" : load("res://Resources/Textures/PlayerTexture/hand_right/hook.png"),
	"katana" : load("res://Resources/Textures/PlayerTexture/hand_right/katana.png"),
	"katana_slant" : load("res://Resources/Textures/PlayerTexture/hand_right/katana_slant.png"),
	"knife" : load("res://Resources/Textures/PlayerTexture/hand_right/knife.png"),
	"lance" : load("res://Resources/Textures/PlayerTexture/hand_right/lance.png"),
	"lance_2" : load("res://Resources/Textures/PlayerTexture/hand_right/lance_2.png"),
	"large_mace" : load("res://Resources/Textures/PlayerTexture/hand_right/large_mace.png"),
	"legolas" : load("res://Resources/Textures/PlayerTexture/hand_right/legolas.png"),
	"long_sword" : load("res://Resources/Textures/PlayerTexture/hand_right/long_sword.png"),
	"long_sword_slant_2" : load("res://Resources/Textures/PlayerTexture/hand_right/long_sword_slant_2.png"),
	"long_sword_slant_new" : load("res://Resources/Textures/PlayerTexture/hand_right/long_sword_slant_new.png"),
	"long_sword_slant_old" : load("res://Resources/Textures/PlayerTexture/hand_right/long_sword_slant_old.png"),
	"mace_2_new" : load("res://Resources/Textures/PlayerTexture/hand_right/mace_2_new.png"),
	"mace_2_old" : load("res://Resources/Textures/PlayerTexture/hand_right/mace_2_old.png"),
	"mace_3" : load("res://Resources/Textures/PlayerTexture/hand_right/mace_3.png"),
	"mace_new" : load("res://Resources/Textures/PlayerTexture/hand_right/mace_new.png"),
	"mace_old" : load("res://Resources/Textures/PlayerTexture/hand_right/mace_old.png"),
	"mace_ruby_new" : load("res://Resources/Textures/PlayerTexture/hand_right/mace_ruby_new.png"),
	"mace_ruby_old" : load("res://Resources/Textures/PlayerTexture/hand_right/mace_ruby_old.png"),
	"morningstar_2_new" : load("res://Resources/Textures/PlayerTexture/hand_right/morningstar_2_new.png"),
	"morningstar_2_old" : load("res://Resources/Textures/PlayerTexture/hand_right/morningstar_2_old.png"),
	"morningstar_new" : load("res://Resources/Textures/PlayerTexture/hand_right/morningstar_new.png"),
	"morningstar_old" : load("res://Resources/Textures/PlayerTexture/hand_right/morningstar_old.png"),
	"nunchaku" : load("res://Resources/Textures/PlayerTexture/hand_right/nunchaku.png"),
	"pick_axe" : load("res://Resources/Textures/PlayerTexture/hand_right/pick_axe.png"),
	"pike" : load("res://Resources/Textures/PlayerTexture/hand_right/pike.png"),
	"pole_forked" : load("res://Resources/Textures/PlayerTexture/hand_right/pole_forked.png"),
	"quarterstaff" : load("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff.png"),
	"quarterstaff_1" : load("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_1.png"),
	"quarterstaff_2_new" : load("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_2_new.png"),
	"quarterstaff_2_old" : load("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_2_old.png"),
	"quarterstaff_3" : load("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_3.png"),
	"quarterstaff_4" : load("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_4.png"),
	"quarterstaff_jester" : load("res://Resources/Textures/PlayerTexture/hand_right/quarterstaff_jester.png"),
	"rapier" : load("res://Resources/Textures/PlayerTexture/hand_right/rapier.png"),
	"rapier_2" : load("res://Resources/Textures/PlayerTexture/hand_right/rapier_2.png"),
	"rod_aries_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_aries_new.png"),
	"rod_aries_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_aries_old.png"),
	"rod_blue_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_blue_new.png"),
	"rod_blue_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_blue_old.png"),
	"rod_brown_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_brown_new.png"),
	"rod_brown_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_brown_old.png"),
	"rod_emerald_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_emerald_new.png"),
	"rod_emerald_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_emerald_old.png"),
	"rod_forked_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_forked_new.png"),
	"rod_forked_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_forked_old.png"),
	"rod_hammer_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_hammer_new.png"),
	"rod_hammer_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_hammer_old.png"),
	"rod_magenta_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_magenta_new.png"),
	"rod_magenta_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_magenta_old.png"),
	"rod_moon_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_moon_new.png"),
	"rod_moon_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_moon_old.png"),
	"rod_ruby_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_ruby_new.png"),
	"rod_ruby_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_ruby_old.png"),
	"rod_thick_new" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_thick_new.png"),
	"rod_thick_old" : load("res://Resources/Textures/PlayerTexture/hand_right/rod_thick_old.png"),
	"sabre" : load("res://Resources/Textures/PlayerTexture/hand_right/sabre.png"),
	"saruman" : load("res://Resources/Textures/PlayerTexture/hand_right/saruman.png"),
	"scepter" : load("res://Resources/Textures/PlayerTexture/hand_right/scepter.png"),
	"scimitar_new" : load("res://Resources/Textures/PlayerTexture/hand_right/scimitar_new.png"),
	"scimitar_old" : load("res://Resources/Textures/PlayerTexture/hand_right/scimitar_old.png"),
	"scythe_2" : load("res://Resources/Textures/PlayerTexture/hand_right/scythe_2.png"),
	"scythe_new" : load("res://Resources/Textures/PlayerTexture/hand_right/scythe_new.png"),
	"scythe_old" : load("res://Resources/Textures/PlayerTexture/hand_right/scythe_old.png"),
	"scythe_slant" : load("res://Resources/Textures/PlayerTexture/hand_right/scythe_slant.png"),
	"short_sword" : load("res://Resources/Textures/PlayerTexture/hand_right/short_sword.png"),
	"short_sword_2" : load("res://Resources/Textures/PlayerTexture/hand_right/short_sword_2.png"),
	"short_sword_slant_2" : load("res://Resources/Textures/PlayerTexture/hand_right/short_sword_slant_2.png"),
	"short_sword_slant_3" : load("res://Resources/Textures/PlayerTexture/hand_right/short_sword_slant_3.png"),
	"short_sword_slant_new" : load("res://Resources/Textures/PlayerTexture/hand_right/short_sword_slant_new.png"),
	"short_sword_slant_old" : load("res://Resources/Textures/PlayerTexture/hand_right/short_sword_slant_old.png"),
	"sickle" : load("res://Resources/Textures/PlayerTexture/hand_right/sickle.png"),
	"sling" : load("res://Resources/Textures/PlayerTexture/hand_right/sling.png"),
	"spear" : load("res://Resources/Textures/PlayerTexture/hand_right/spear.png"),
	"spear_1" : load("res://Resources/Textures/PlayerTexture/hand_right/spear_1.png"),
	"spear_2_new" : load("res://Resources/Textures/PlayerTexture/hand_right/spear_2_new.png"),
	"spear_2_old" : load("res://Resources/Textures/PlayerTexture/hand_right/spear_2_old.png"),
	"spear_3" : load("res://Resources/Textures/PlayerTexture/hand_right/spear_3.png"),
	"spear_4" : load("res://Resources/Textures/PlayerTexture/hand_right/spear_4.png"),
	"spear_5" : load("res://Resources/Textures/PlayerTexture/hand_right/spear_5.png"),
	"staff_evil" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_evil.png"),
	"staff_fancy" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_fancy.png"),
	"staff_fork" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_fork.png"),
	"staff_large" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_large.png"),
	"staff_mage" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_mage.png"),
	"staff_mage_2" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_mage_2.png"),
	"staff_mummy" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_mummy.png"),
	"staff_organic" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_organic.png"),
	"staff_plain" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_plain.png"),
	"staff_ring_blue" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_ring_blue.png"),
	"staff_ruby" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_ruby.png"),
	"staff_scepter" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_scepter.png"),
	"staff_skull" : load("res://Resources/Textures/PlayerTexture/hand_right/staff_skull.png"),
	"stick" : load("res://Resources/Textures/PlayerTexture/hand_right/stick.png"),
	"sword_2" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_2.png"),
	"sword_3" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_3.png"),
	"sword_black" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_black.png"),
	"sword_breaker" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_breaker.png"),
	"sword_jag" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_jag.png"),
	"sword_seven" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_seven.png"),
	"sword_thief" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_thief.png"),
	"sword_tri" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_tri.png"),
	"sword_twist" : load("res://Resources/Textures/PlayerTexture/hand_right/sword_twist.png"),
	"trident" : load("res://Resources/Textures/PlayerTexture/hand_right/trident.png"),
	"trident_2" : load("res://Resources/Textures/PlayerTexture/hand_right/trident_2.png"),
	"trident_3" : load("res://Resources/Textures/PlayerTexture/hand_right/trident_3.png"),
	"trident_demon" : load("res://Resources/Textures/PlayerTexture/hand_right/trident_demon.png"),
	"trident_elec" : load("res://Resources/Textures/PlayerTexture/hand_right/trident_elec.png"),
	"trident_two" : load("res://Resources/Textures/PlayerTexture/hand_right/trident_two.png"),
	"trident_two_2" : load("res://Resources/Textures/PlayerTexture/hand_right/trident_two_2.png"),
	"triple_sword_2" : load("res://Resources/Textures/PlayerTexture/hand_right/triple_sword_2.png"),
	"triple_sword_new" : load("res://Resources/Textures/PlayerTexture/hand_right/triple_sword_new.png"),
	"triple_sword_old" : load("res://Resources/Textures/PlayerTexture/hand_right/triple_sword_old.png"),
	"trishula" : load("res://Resources/Textures/PlayerTexture/hand_right/trishula.png"),
	"war_axe_new" : load("res://Resources/Textures/PlayerTexture/hand_right/war_axe_new.png"),
	"war_axe_old" : load("res://Resources/Textures/PlayerTexture/hand_right/war_axe_old.png"),
	"whip_2" : load("res://Resources/Textures/PlayerTexture/hand_right/whip_2.png"),
	"whip_new" : load("res://Resources/Textures/PlayerTexture/hand_right/whip_new.png"),
	"whip_old" : load("res://Resources/Textures/PlayerTexture/hand_right/whip_old.png")}


func getPlayerTexture_hand_right(resourceName : String) :
	return PlayerTexture_hand_rightDictionary.get(resourceName)

var PlayerTexture_headDictionary = {
	"art_dragonhelm" : load("res://Resources/Textures/PlayerTexture/head/art_dragonhelm.png"),
	"band_blue" : load("res://Resources/Textures/PlayerTexture/head/band_blue.png"),
	"band_magenta" : load("res://Resources/Textures/PlayerTexture/head/band_magenta.png"),
	"band_red" : load("res://Resources/Textures/PlayerTexture/head/band_red.png"),
	"band_white" : load("res://Resources/Textures/PlayerTexture/head/band_white.png"),
	"band_yellow" : load("res://Resources/Textures/PlayerTexture/head/band_yellow.png"),
	"bandana_ybrown" : load("res://Resources/Textures/PlayerTexture/head/bandana_ybrown.png"),
	"bear" : load("res://Resources/Textures/PlayerTexture/head/bear.png"),
	"black_horn" : load("res://Resources/Textures/PlayerTexture/head/black_horn.png"),
	"black_horn_2" : load("res://Resources/Textures/PlayerTexture/head/black_horn_2.png"),
	"blue_horn_gold" : load("res://Resources/Textures/PlayerTexture/head/blue_horn_gold.png"),
	"brown_gold" : load("res://Resources/Textures/PlayerTexture/head/brown_gold.png"),
	"cap_black_1" : load("res://Resources/Textures/PlayerTexture/head/cap_black_1.png"),
	"cap_blue" : load("res://Resources/Textures/PlayerTexture/head/cap_blue.png"),
	"chain" : load("res://Resources/Textures/PlayerTexture/head/chain.png"),
	"cheek_red" : load("res://Resources/Textures/PlayerTexture/head/cheek_red.png"),
	"clown_1" : load("res://Resources/Textures/PlayerTexture/head/clown_1.png"),
	"clown_2" : load("res://Resources/Textures/PlayerTexture/head/clown_2.png"),
	"cone_blue" : load("res://Resources/Textures/PlayerTexture/head/cone_blue.png"),
	"cone_red" : load("res://Resources/Textures/PlayerTexture/head/cone_red.png"),
	"crown_gold_1" : load("res://Resources/Textures/PlayerTexture/head/crown_gold_1.png"),
	"crown_gold_2" : load("res://Resources/Textures/PlayerTexture/head/crown_gold_2.png"),
	"crown_gold_3" : load("res://Resources/Textures/PlayerTexture/head/crown_gold_3.png"),
	"dyrovepreva_new" : load("res://Resources/Textures/PlayerTexture/head/dyrovepreva_new.png"),
	"dyrovepreva_old" : load("res://Resources/Textures/PlayerTexture/head/dyrovepreva_old.png"),
	"eternal_torment" : load("res://Resources/Textures/PlayerTexture/head/eternal_torment.png"),
	"etheric_cage" : load("res://Resources/Textures/PlayerTexture/head/etheric_cage.png"),
	"feather_blue" : load("res://Resources/Textures/PlayerTexture/head/feather_blue.png"),
	"feather_green" : load("res://Resources/Textures/PlayerTexture/head/feather_green.png"),
	"feather_red" : load("res://Resources/Textures/PlayerTexture/head/feather_red.png"),
	"feather_white" : load("res://Resources/Textures/PlayerTexture/head/feather_white.png"),
	"feather_yellow" : load("res://Resources/Textures/PlayerTexture/head/feather_yellow.png"),
	"fhelm_gray_3" : load("res://Resources/Textures/PlayerTexture/head/fhelm_gray_3.png"),
	"fhelm_horn_2" : load("res://Resources/Textures/PlayerTexture/head/fhelm_horn_2.png"),
	"fhelm_horn_yellow" : load("res://Resources/Textures/PlayerTexture/head/fhelm_horn_yellow.png"),
	"full_black" : load("res://Resources/Textures/PlayerTexture/head/full_black.png"),
	"full_gold" : load("res://Resources/Textures/PlayerTexture/head/full_gold.png"),
	"gandalf" : load("res://Resources/Textures/PlayerTexture/head/gandalf.png"),
	"hat_black" : load("res://Resources/Textures/PlayerTexture/head/hat_black.png"),
	"healer" : load("res://Resources/Textures/PlayerTexture/head/healer.png"),
	"helm_gimli" : load("res://Resources/Textures/PlayerTexture/head/helm_gimli.png"),
	"helm_green" : load("res://Resources/Textures/PlayerTexture/head/helm_green.png"),
	"helm_plume" : load("res://Resources/Textures/PlayerTexture/head/helm_plume.png"),
	"helm_red" : load("res://Resources/Textures/PlayerTexture/head/helm_red.png"),
	"hood_black_2" : load("res://Resources/Textures/PlayerTexture/head/hood_black_2.png"),
	"hood_cyan" : load("res://Resources/Textures/PlayerTexture/head/hood_cyan.png"),
	"hood_gray" : load("res://Resources/Textures/PlayerTexture/head/hood_gray.png"),
	"hood_green" : load("res://Resources/Textures/PlayerTexture/head/hood_green.png"),
	"hood_green_2" : load("res://Resources/Textures/PlayerTexture/head/hood_green_2.png"),
	"hood_orange" : load("res://Resources/Textures/PlayerTexture/head/hood_orange.png"),
	"hood_red" : load("res://Resources/Textures/PlayerTexture/head/hood_red.png"),
	"hood_red_2" : load("res://Resources/Textures/PlayerTexture/head/hood_red_2.png"),
	"hood_white" : load("res://Resources/Textures/PlayerTexture/head/hood_white.png"),
	"hood_white_2" : load("res://Resources/Textures/PlayerTexture/head/hood_white_2.png"),
	"hood_ybrown" : load("res://Resources/Textures/PlayerTexture/head/hood_ybrown.png"),
	"horn_evil" : load("res://Resources/Textures/PlayerTexture/head/horn_evil.png"),
	"horn_gray" : load("res://Resources/Textures/PlayerTexture/head/horn_gray.png"),
	"horned" : load("res://Resources/Textures/PlayerTexture/head/horned.png"),
	"horns_1" : load("res://Resources/Textures/PlayerTexture/head/horns_1.png"),
	"horns_2" : load("res://Resources/Textures/PlayerTexture/head/horns_2.png"),
	"horns_3" : load("res://Resources/Textures/PlayerTexture/head/horns_3.png"),
	"iron_1" : load("res://Resources/Textures/PlayerTexture/head/iron_1.png"),
	"iron_2" : load("res://Resources/Textures/PlayerTexture/head/iron_2.png"),
	"iron_3" : load("res://Resources/Textures/PlayerTexture/head/iron_3.png"),
	"iron_red" : load("res://Resources/Textures/PlayerTexture/head/iron_red.png"),
	"isildur" : load("res://Resources/Textures/PlayerTexture/head/isildur.png"),
	"mummy" : load("res://Resources/Textures/PlayerTexture/head/mummy.png"),
	"ninja_black" : load("res://Resources/Textures/PlayerTexture/head/ninja_black.png"),
	"straw" : load("res://Resources/Textures/PlayerTexture/head/straw.png"),
	"taiso_blue" : load("res://Resources/Textures/PlayerTexture/head/taiso_blue.png"),
	"taiso_magenta" : load("res://Resources/Textures/PlayerTexture/head/taiso_magenta.png"),
	"taiso_red" : load("res://Resources/Textures/PlayerTexture/head/taiso_red.png"),
	"taiso_white" : load("res://Resources/Textures/PlayerTexture/head/taiso_white.png"),
	"taiso_yellow" : load("res://Resources/Textures/PlayerTexture/head/taiso_yellow.png"),
	"turban_brown" : load("res://Resources/Textures/PlayerTexture/head/turban_brown.png"),
	"turban_purple" : load("res://Resources/Textures/PlayerTexture/head/turban_purple.png"),
	"turban_white" : load("res://Resources/Textures/PlayerTexture/head/turban_white.png"),
	"viking_brown_1" : load("res://Resources/Textures/PlayerTexture/head/viking_brown_1.png"),
	"viking_brown_2" : load("res://Resources/Textures/PlayerTexture/head/viking_brown_2.png"),
	"viking_gold" : load("res://Resources/Textures/PlayerTexture/head/viking_gold.png"),
	"wizard_blackgold" : load("res://Resources/Textures/PlayerTexture/head/wizard_blackgold.png"),
	"wizard_blackred" : load("res://Resources/Textures/PlayerTexture/head/wizard_blackred.png"),
	"wizard_blue" : load("res://Resources/Textures/PlayerTexture/head/wizard_blue.png"),
	"wizard_bluegreen" : load("res://Resources/Textures/PlayerTexture/head/wizard_bluegreen.png"),
	"wizard_brown" : load("res://Resources/Textures/PlayerTexture/head/wizard_brown.png"),
	"wizard_darkgreen" : load("res://Resources/Textures/PlayerTexture/head/wizard_darkgreen.png"),
	"wizard_lightgreen" : load("res://Resources/Textures/PlayerTexture/head/wizard_lightgreen.png"),
	"wizard_purple" : load("res://Resources/Textures/PlayerTexture/head/wizard_purple.png"),
	"wizard_red" : load("res://Resources/Textures/PlayerTexture/head/wizard_red.png"),
	"wizard_white" : load("res://Resources/Textures/PlayerTexture/head/wizard_white.png"),
	"yellow_wing" : load("res://Resources/Textures/PlayerTexture/head/yellow_wing.png")}


func getPlayerTexture_head(resourceName : String) :
	return PlayerTexture_headDictionary.get(resourceName)

var PlayerTexture_legsDictionary = {
	"belt_gray" : load("res://Resources/Textures/PlayerTexture/legs/belt_gray.png"),
	"belt_redbrown" : load("res://Resources/Textures/PlayerTexture/legs/belt_redbrown.png"),
	"bikini_red" : load("res://Resources/Textures/PlayerTexture/legs/bikini_red.png"),
	"chunli" : load("res://Resources/Textures/PlayerTexture/legs/chunli.png"),
	"garter" : load("res://Resources/Textures/PlayerTexture/legs/garter.png"),
	"leg_armor_0" : load("res://Resources/Textures/PlayerTexture/legs/leg_armor_0.png"),
	"leg_armor_1" : load("res://Resources/Textures/PlayerTexture/legs/leg_armor_1.png"),
	"leg_armor_2" : load("res://Resources/Textures/PlayerTexture/legs/leg_armor_2.png"),
	"leg_armor_3" : load("res://Resources/Textures/PlayerTexture/legs/leg_armor_3.png"),
	"leg_armor_4" : load("res://Resources/Textures/PlayerTexture/legs/leg_armor_4.png"),
	"leg_armor_5" : load("res://Resources/Textures/PlayerTexture/legs/leg_armor_5.png"),
	"loincloth_red" : load("res://Resources/Textures/PlayerTexture/legs/loincloth_red.png"),
	"long_red" : load("res://Resources/Textures/PlayerTexture/legs/long_red.png"),
	"metal_gray" : load("res://Resources/Textures/PlayerTexture/legs/metal_gray.png"),
	"metal_green" : load("res://Resources/Textures/PlayerTexture/legs/metal_green.png"),
	"pants_16" : load("res://Resources/Textures/PlayerTexture/legs/pants_16.png"),
	"pants_black" : load("res://Resources/Textures/PlayerTexture/legs/pants_black.png"),
	"pants_blue" : load("res://Resources/Textures/PlayerTexture/legs/pants_blue.png"),
	"pants_brown" : load("res://Resources/Textures/PlayerTexture/legs/pants_brown.png"),
	"pants_darkgreen" : load("res://Resources/Textures/PlayerTexture/legs/pants_darkgreen.png"),
	"pants_l_white" : load("res://Resources/Textures/PlayerTexture/legs/pants_l_white.png"),
	"pants_orange" : load("res://Resources/Textures/PlayerTexture/legs/pants_orange.png"),
	"pants_red" : load("res://Resources/Textures/PlayerTexture/legs/pants_red.png"),
	"pants_short_brown" : load("res://Resources/Textures/PlayerTexture/legs/pants_short_brown.png"),
	"pants_short_brown_3" : load("res://Resources/Textures/PlayerTexture/legs/pants_short_brown_3.png"),
	"pants_short_darkbrown" : load("res://Resources/Textures/PlayerTexture/legs/pants_short_darkbrown.png"),
	"pants_short_gray" : load("res://Resources/Textures/PlayerTexture/legs/pants_short_gray.png"),
	"pj" : load("res://Resources/Textures/PlayerTexture/legs/pj.png"),
	"skirt_blue" : load("res://Resources/Textures/PlayerTexture/legs/skirt_blue.png"),
	"skirt_green" : load("res://Resources/Textures/PlayerTexture/legs/skirt_green.png"),
	"skirt_red" : load("res://Resources/Textures/PlayerTexture/legs/skirt_red.png"),
	"skirt_white" : load("res://Resources/Textures/PlayerTexture/legs/skirt_white.png"),
	"skirt_white_2" : load("res://Resources/Textures/PlayerTexture/legs/skirt_white_2.png"),
	"trouser_green" : load("res://Resources/Textures/PlayerTexture/legs/trouser_green.png")}


func getPlayerTexture_legs(resourceName : String) :
	return PlayerTexture_legsDictionary.get(resourceName)

var PlayerTexture_mutationsDictionary = {
	"cat_10" : load("res://Resources/Textures/PlayerTexture/mutations/cat_10.png"),
	"cat_6" : load("res://Resources/Textures/PlayerTexture/mutations/cat_6.png"),
	"cat_7" : load("res://Resources/Textures/PlayerTexture/mutations/cat_7.png"),
	"cat_8" : load("res://Resources/Textures/PlayerTexture/mutations/cat_8.png"),
	"cat_9" : load("res://Resources/Textures/PlayerTexture/mutations/cat_9.png"),
	"octopode_1" : load("res://Resources/Textures/PlayerTexture/mutations/octopode_1.png")}


func getPlayerTexture_mutations(resourceName : String) :
	return PlayerTexture_mutationsDictionary.get(resourceName)

var PlayerTexture_transformDictionary = {
	"bat_form" : load("res://Resources/Textures/PlayerTexture/transform/bat_form.png"),
	"dragon_form" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form.png"),
	"dragon_form_black" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_black.png"),
	"dragon_form_green" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_green.png"),
	"dragon_form_grey" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_grey.png"),
	"dragon_form_mottled" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_mottled.png"),
	"dragon_form_pale" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_pale.png"),
	"dragon_form_purple" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_purple.png"),
	"dragon_form_red" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_red.png"),
	"dragon_form_white" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_white.png"),
	"dragon_form_yellow" : load("res://Resources/Textures/PlayerTexture/transform/dragon_form_yellow.png"),
	"ice_form" : load("res://Resources/Textures/PlayerTexture/transform/ice_form.png"),
	"lich_form" : load("res://Resources/Textures/PlayerTexture/transform/lich_form.png"),
	"lich_form_octopode" : load("res://Resources/Textures/PlayerTexture/transform/lich_form_octopode.png"),
	"mushroom_form" : load("res://Resources/Textures/PlayerTexture/transform/mushroom_form.png"),
	"pig_form_new" : load("res://Resources/Textures/PlayerTexture/transform/pig_form_new.png"),
	"pig_form_old" : load("res://Resources/Textures/PlayerTexture/transform/pig_form_old.png"),
	"shadow_form" : load("res://Resources/Textures/PlayerTexture/transform/shadow_form.png"),
	"statue_form_centaur" : load("res://Resources/Textures/PlayerTexture/transform/statue_form_centaur.png"),
	"statue_form_felid" : load("res://Resources/Textures/PlayerTexture/transform/statue_form_felid.png"),
	"statue_form_humanoid" : load("res://Resources/Textures/PlayerTexture/transform/statue_form_humanoid.png"),
	"statue_form_naga" : load("res://Resources/Textures/PlayerTexture/transform/statue_form_naga.png"),
	"tree_form" : load("res://Resources/Textures/PlayerTexture/transform/tree_form.png")}


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
