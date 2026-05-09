extends Node

##!! denotes an attack that doesn't have an sfx that fits well, but I wanted to give every attack a sound.
const dict : Dictionary = {
	"axe_1h" : AudioHandler.sfx.meleeSlice,
	"axe_2h" : AudioHandler.sfx.meleeSlice,
	"blunderbuss" : AudioHandler.sfx.blunderbuss,
	## An attack too fast to be seen- for the OP secret boss
	"blur" : AudioHandler.sfx.boulder,                           ##!!
	"bow" : AudioHandler.sfx.rangedPhysical,
	"club_1h" : AudioHandler.sfx.meleeBlunt,
	## Whipcrack
	"crack" : AudioHandler.sfx.meleeWhip,
	"crossbow" : AudioHandler.sfx.rangedPhysical,                 ##!!
	"curse" : AudioHandler.sfx.magicGeneric,
	"dagger" : AudioHandler.sfx.meleeSlice,
	"disintigrate" : AudioHandler.sfx.magicGeneric,               ##!!
	"flame_whip" : AudioHandler.sfx.meleeWhip,
	"flurry" : AudioHandler.sfx.rangedPhysical,
	"freezeflame_double_slice" : AudioHandler.sfx.meleeSlice,
	"frost_slash" : AudioHandler.sfx.meleeSlice,
	## Called "Exanguinate" in game
	"generic_brutal" : AudioHandler.sfx.meleeSlice,
	"icicle_spike" : AudioHandler.sfx.rangedPhysical,
	"katana_attack" : AudioHandler.sfx.meleeSlice,
	"kinetic_blast" : AudioHandler.sfx.boulder,
	"lance" : AudioHandler.sfx.meleeSlice,
	"laser" : null,                   ##Would prefer more of a monotone sound, like the beam weapons from FTL
	## Called "electric arc" in game
	"lightning" : AudioHandler.sfx.magicLightning,
	"lucky_star" : AudioHandler.sfx.magicGeneric,
	"mace_1h" : AudioHandler.sfx.meleeBlunt,
	"mace_2h" : AudioHandler.sfx.meleeBlunt,
	"magic_lance" : AudioHandler.sfx.meleeSlice,
	"magic_repeater" : AudioHandler.sfx.rangedPhysical,
	"magic_slashing_sword_2h" : AudioHandler.sfx.meleeSlice,
	"magic_split_man" : AudioHandler.sfx.meleeSlice,
	"scythe" : AudioHandler.sfx.meleeSlice,
	"shortbow" : AudioHandler.sfx.rangedPhysical,
	"slashing_sword_1h" : AudioHandler.sfx.meleeSlice,
	"slashing_sword_2h" : AudioHandler.sfx.meleeSlice,
	"sling" : AudioHandler.sfx.rangedPhysical,
	"spear_1h" : AudioHandler.sfx.meleeSlice,
	"spear_2h" : AudioHandler.sfx.meleeSlice,
	"spear_darkness" : AudioHandler.sfx.magicGeneric,
	"split_man" : AudioHandler.sfx.meleeSlice,
	"thresh" : AudioHandler.sfx.meleeBlunt,
	"throw_axe" : AudioHandler.sfx.rangedPhysical,
	## For the earth staff
	"throw_boulder" : AudioHandler.sfx.boulder,
	"throw_kunai" : AudioHandler.sfx.rangedPhysical,
	"thrust_sword_1h" : AudioHandler.sfx.meleeSlice,
	"thrust_sword_2h" : AudioHandler.sfx.meleeSlice,
	"water_jet" : AudioHandler.sfx.magicGeneric,
	## "Torrent"
	"water_jet_2" : AudioHandler.sfx.magicGeneric,
	## "Great wave
	"water_jet_3" : AudioHandler.sfx.breathWater,
	"arcane_bolt" : AudioHandler.sfx.magicGeneric,
	"bite_acid_2" : AudioHandler.sfx.biteAcid,
	"bite_big" : AudioHandler.sfx.biteBig,
	"bite_fire" : AudioHandler.sfx.biteBark,
	## "Slavering Bite"
	"bite_generic" : AudioHandler.sfx.biteBark,
	"bite_ice" : AudioHandler.sfx.biteIce,
	## It's faster than ice 1
	"bite_ice_2" : AudioHandler.sfx.biteIceBig,
	"bite_venomous" : AudioHandler.sfx.biteVenom,
	## Dragon breaths
	"breath_acid" : AudioHandler.sfx.breathAcid,
	"breath_death" : AudioHandler.sfx.breathAcid,
	"breath_fire" : AudioHandler.sfx.breathFire,
	"breath_frost" : AudioHandler.sfx.breathIce,
	"breath_iron" : AudioHandler.sfx.breathIce,                         ##!!
	"breath_midas" : AudioHandler.sfx.breathIce,                        ##!!
	"breath_shadow" : AudioHandler.sfx.breathAcid,
	"breath_water" : AudioHandler.sfx.breathWater,
	"claw" : AudioHandler.sfx.meleeSlice,
	"claw_fire" : AudioHandler.sfx.meleeSlice,
	"fire_blast" : AudioHandler.sfx.magicFire,
	## "Explosion"
	"fire_blast_2" : AudioHandler.sfx.magicFire,
	## "Inferno"
	"fire_blast_3" : AudioHandler.sfx.magicFire,
	## as in goring with a tusk
	"gore" : AudioHandler.sfx.meleeSlice,
	"hellfire_cleave" : AudioHandler.sfx.meleeSlice,
	"ice_blast" : AudioHandler.sfx.magicGeneric,
	"javelin" : AudioHandler.sfx.rangedPhysical,
	## Sting
	"jellyfish" : AudioHandler.sfx.rangedPhysical,                     ##!!
	## "Lava Spray"
	"lava" : AudioHandler.sfx.lavaSpray,
	## Maybe reuse dragon breath?
	"poison_spore" : AudioHandler.sfx.breathAcid,
	## Exclusive to the Siren
	"psychic_scream" : AudioHandler.sfx.psychicScream,
	## Giant attacks
	"stomp_fire" : AudioHandler.sfx.boulder,
	"stomp_generic" : AudioHandler.sfx.boulder,
	"stomp_ice" : AudioHandler.sfx.boulder,
	###########################################
	## Tutorial
	############################################
	"bludgeon" : AudioHandler.sfx.meleeBlunt,
	"claw_zombie" :AudioHandler.sfx.meleeSlice,
	## As in knifehand strike
	"knifehand" : AudioHandler.sfx.meleeBlunt,
	## "Shiv"
	"knifehand_2" : AudioHandler.sfx.meleeSlice,
	## Rat
	"nibble" : AudioHandler.sfx.meleeBlunt,                           ##!!
	"psy_blast" : AudioHandler.sfx.magicGeneric,
	"psy_blast_2" : AudioHandler.sfx.magicGeneric,
	"punch" : AudioHandler.sfx.meleeBlunt,
	## "Bludgeon"
	"punch_2" : AudioHandler.sfx.meleeBlunt,
	"weak_stab" : AudioHandler.sfx.meleeSlice,
	"dagger_basic" : AudioHandler.sfx.meleeSlice,
	"sling_basic" : AudioHandler.sfx.rangedPhysical
}

static func getSfx(action : Action) :
	return dict.get(action.getResourceName())
