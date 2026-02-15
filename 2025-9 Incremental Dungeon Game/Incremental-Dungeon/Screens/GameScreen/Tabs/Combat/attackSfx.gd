extends Node

const dict : Dictionary = {
	"axe_1h" : AudioHandler.sfx.meleeSlice,
	"axe_2h" : AudioHandler.sfx.meleeSlice,
	"blunderbuss" : null,
	## An attack too fast to be seen- for the OP secret boss
	"blur" : null,
	"bow" : AudioHandler.sfx.rangedPhysical,
	"club_1h" : null,
	## Whipcrack
	"crack" : null,
	## Crossbows have a rather distinctive sound but I'll use ranged for now
	"crossbow" : AudioHandler.sfx.rangedPhysical,
	"curse" : AudioHandler.sfx.rangedMagic,
	"dagger" : AudioHandler.sfx.meleeSlice,
	"disintigrate" : AudioHandler.sfx.rangedMagic,
	"flame_whip" : null,
	"flurry" : AudioHandler.sfx.rangedPhysical,
	"freezeflame_double_slice" : AudioHandler.sfx.meleeSlice,
	"frost_slash" : AudioHandler.sfx.meleeSlice,
	## Called "Exanguinate" in game
	"generic_brutal" : AudioHandler.sfx.meleeSlice,
	"icicle_spike" : AudioHandler.sfx.rangedPhysical,
	"katana_attack" : AudioHandler.sfx.meleeSlice,
	"kinetic_blast" : null,
	"lance" : AudioHandler.sfx.meleeSlice,
	"laser" : null,
	## Called "electric arc" in game
	"lightning" : null,
	"lucky_star" : AudioHandler.sfx.rangedMagic,
	"mace_1h" : null,
	"mace_2h" : null,
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
	"spear_darkness" : AudioHandler.sfx.rangedMagic,
	"split_man" : AudioHandler.sfx.meleeSlice,
	"thresh" : null,
	"throw_axe" : AudioHandler.sfx.rangedPhysical,
	## For the earth staff
	"throw_boulder" : null,
	"throw_kunai" : AudioHandler.sfx.rangedPhysical,
	"thrust_sword_1h" : AudioHandler.sfx.meleeSlice,
	"thrust_sword_2h" : AudioHandler.sfx.meleeSlice,
	"water_jet_1" : AudioHandler.sfx.rangedMagic,
	## "Torrent"
	"water_jet_2" : AudioHandler.sfx.rangedMagic,
	## "Great wave
	"water_jet_3" : AudioHandler.sfx.rangedMagic,
	"arcane_bolt" : AudioHandler.sfx.rangedMagic,
	"bite_acid_2" : null,
	"bite_big" : null,
	"bite_fire" : null,
	## "Slavering Bite"
	"bite_generic" : null,
	"bite_ice" : null,
	## It's faster than ice 1
	"bite_ice_2" : null,
	"bite_venomous" : null,
	## Dragon breaths
	"breath_acid" : null,
	"breath_death" : null,
	"breath_fire" : null,
	"breath_frost" : null,
	"breath_iron" : null,
	"breath_midas" : null,
	"breath_shadow" : null,
	"breath_water" : null,
	"claw" : AudioHandler.sfx.meleeSlice,
	"claw_fire" : AudioHandler.sfx.meleeSlice,
	"fire_blast" : null,
	## "Explosion"
	"fire_blast_2" : null,
	## "Inferno"
	"fire_blast_3" : null,
	## as in goring with a tusk
	"gore" : AudioHandler.sfx.meleeSlice,
	"hellfire_cleave" : AudioHandler.sfx.meleeSlice,
	"ice_blast" : AudioHandler.sfx.rangedMagic,
	"javelin" : AudioHandler.sfx.rangedPhysical,
	## Sting
	"jellyfish" : null,
	## "Lava Spray"
	"lava" : null,
	## Maybe reuse dragon breath?
	"poison_spore" : null,
	## Exclusive to the Siren
	"psychic_scream" : null,
	## Giant attacks
	"stomp_fire" : null,
	"stomp_generic" : null,
	"stomp_ice" : null,
	###########################################
	## Tutorial
	############################################
	"bludgeon" : null,
	"claw_zombie" :AudioHandler.sfx.meleeSlice,
	## As in knifehand strike
	"knifehand" : null,
	## "Shiv"
	"knifehand_2" : AudioHandler.sfx.meleeSlice,
	## Rat
	"nibble" : null,
	"psy_blast" : AudioHandler.sfx.rangedMagic,
	"psy_blast_2" : AudioHandler.sfx.rangedMagic,
	"punch" : null,
	## "Bludgeon"
	"punch_2" : AudioHandler.sfx.rangedMagic,
	"weak_stab" : AudioHandler.sfx.meleeSlice,
	"dagger_basic" : AudioHandler.sfx.meleeSlice,
	"sling_basic" : AudioHandler.sfx.rangedPhysical
}

static func getSfx(action : Action) :
	return dict.get(action.getResourceName())
