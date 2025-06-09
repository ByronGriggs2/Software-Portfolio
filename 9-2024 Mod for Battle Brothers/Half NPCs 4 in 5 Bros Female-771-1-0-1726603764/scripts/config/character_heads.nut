local gt = this.getroottable();

if (!("Faces" in gt.Const))
{
	gt.Const.Faces <- {};
}

if (!("Hair" in gt.Const))
{
	gt.Const.Hair <- {};
}

if (!("HairColors" in gt.Const))
{
	gt.Const.HairColors <- {};
}

if (!("Beards" in gt.Const))
{
	gt.Const.Beards <- {};
}

if (!("Tattoos" in gt.Const))
{
	gt.Const.Tattoos <- {};
}

if (!("Bodies" in gt.Const))
{
	gt.Const.Bodies <- {};
}

gt.Const.HairColors.All <- [
	"black",
	"grey",
	"blonde",
	"brown",
	"red"
];
gt.Const.HairColors.Old <- [
	"grey"
];
gt.Const.HairColors.Young <- [
	"black",
	"blonde",
	"brown",
	"red"
];
gt.Const.HairColors.Zombie <- [
	"black",
	"grey"
];
gt.Const.HairColors.Lineage <- [
	[
		"black",
		"black",
		"grey"
	],
	[
		"blonde",
		"blonde",
		"grey"
	],
	[
		"brown",
		"brown",
		"grey"
	],
	[
		"red",
		"red",
		"grey"
	]
];
gt.Const.HairColors.Southern <- [
	"brown",
	"grey",
	"brown",
	"brown",
	"brown"
];
gt.Const.HairColors.SouthernYoung <- [
	"brown"
];
gt.Const.Faces.Female <- [
	"bust_hexen_charmed_head_01",
	"bust_hexen_charmed_head_02"
];
gt.Const.Faces.AllMale <- [
	"bust_head_01",
	"bust_head_02",
	"bust_head_03",
	"bust_head_04",
	"bust_head_05",
	"bust_head_06",
	"bust_head_07",
	"bust_head_08",
	"bust_head_09",
	"bust_head_10",
	"bust_head_11",
	"bust_head_12",
	"bust_head_13",
	"bust_head_14",
	"bust_head_15",
	"bust_head_16",
	"bust_head_17",
	"bust_head_18"
];
gt.Const.Faces.Barber <- clone this.Const.Faces.AllMale;
gt.Const.Faces.SmartMale <- [
	"bust_head_01",
	"bust_head_04",
	"bust_head_05",
	"bust_head_06",
	"bust_head_07",
	"bust_head_08",
	"bust_head_09",
	"bust_head_12",
	"bust_head_13",
	"bust_head_14"
];
gt.Const.Faces.WildMale <- [
	"bust_head_02",
	"bust_head_03",
	"bust_head_09",
	"bust_head_10",
	"bust_head_11",
	"bust_head_12",
	"bust_head_14",
	"bust_head_15",
	"bust_head_16",
	"bust_head_19",
	"bust_head_20"
];
gt.Const.Faces.SouthernMale <- [
	"bust_head_southern_01",
	"bust_head_southern_02",
	"bust_head_southern_03",
	"bust_head_southern_04",
	"bust_head_southern_05",
	"bust_head_southern_06",
	"bust_head_southern_07",
	"bust_head_southern_08"
];
gt.Const.Faces.Necromancer <- [
	"bust_head_necro_01",
	"bust_head_necro_02"
];
gt.Const.Hair.Barber <- [
	"",
	"shaved",
	"01",
	"02",
	"03",
	"04",
	"05",
	"06",
	"07",
	"08",
	"09",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
	"16",
	"17",
	"18",
	"19",
	"20",
	"21"
];
gt.Const.Hair.Female <- [
	"bust_hexen_charmed_hair_01",
	"bust_hexen_charmed_hair_02",
	"bust_hexen_charmed_hair_03",
	"bust_hexen_charmed_hair_04",
	"bust_hexen_charmed_hair_05"
];
gt.Const.Hair.AllMale <- [
	"shaved",
	"01",
	"02",
	"03",
	"04",
	"05",
	"06",
	"07",
	"08",
	"09",
	"10",
	"11",
	"13",
	"14",
	"15",
	"16",
	"20",
	"21"
];
gt.Const.Hair.CommonMale <- [
	"shaved",
	"01",
	"02",
	"04",
	"05",
	"06",
	"07",
	"08",
	"10",
	"13",
	"14",
	"15",
	"16",
	"20"
];
gt.Const.Hair.TidyMale <- [
	"shaved",
	"01",
	"02",
	"04",
	"05",
	"06",
	"07",
	"08",
	"10",
	"13",
	"14",
	"15",
	"16",
	"19",
	"20"
];
gt.Const.Hair.UntidyMale <- [
	"shaved",
	"02",
	"03",
	"04",
	"05",
	"08",
	"09",
	"10",
	"11",
	"13",
	"15",
	"16",
	"17",
	"18",
	"21"
];
gt.Const.Hair.WildMale <- [
	"03",
	"08",
	"18",
	"21",
	"shaved"
];
gt.Const.Hair.YoungMale <- [
	"shaved",
	"01",
	"04",
	"05",
	"06",
	"07",
	"08",
	"09",
	"10",
	"11",
	"20"
];
gt.Const.Hair.Military <- [
	"shaved"
];
gt.Const.Hair.Monk <- [
	"12",
	"13",
	"14",
	"15",
	"16"
];
gt.Const.Hair.SouthernMaleOnly <- [
	"southern_01",
	"southern_02",
	"southern_03",
	"southern_04",
	"southern_05",
	"southern_06",
	"southern_07",
	"southern_08"
];
gt.Const.Hair.SouthernMale <- [
	"southern_01",
	"southern_02",
	"southern_03",
	"southern_04",
	"southern_05",
	"southern_06",
	"southern_07",
	"southern_08",
	"10",
	"15",
	"16",
	"19"
];
gt.Const.Hair.Vampire <- [
	"zombie_01",
	"zombie_02",
	"zombie_03",
	"zombie_05",
	"zombie_06",
	"zombie_07"
];
gt.Const.Hair.ZombieOnly <- [
	"zombie_01",
	"zombie_02",
	"zombie_03",
	"zombie_04",
	"zombie_05",
	"zombie_06",
	"zombie_07"
];
gt.Const.Hair.Zombie <- [
	"zombie_01",
	"zombie_02",
	"zombie_03",
	"zombie_04",
	"zombie_05",
	"zombie_06",
	"zombie_07",
	"09",
	"13",
	"15"
];
gt.Const.Hair.Necromancer <- [
	"shaved",
	"02",
	"04",
	"08",
	"10",
	"13",
	"15",
	"16",
	"17",
	"18"
];
gt.Const.Beards.Barber <- [
	"",
	"01",
	"02",
	"03",
	"04",
	"05",
	"06",
	"07",
	"08",
	"09",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
	"16",
	"17"
];
gt.Const.Beards.All <- [
	"01",
	"02",
	"03",
	"04",
	"05",
	"06",
	"07",
	"08",
	"09",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
	"16",
	"17"
];
gt.Const.Beards.Tidy <- [
	"01",
	"02",
	"03",
	"06",
	"10",
	"11",
	"16"
];
gt.Const.Beards.Untidy <- [
	"01",
	"03",
	"04",
	"05",
	"07",
	"08",
	"09",
	"14",
	"17"
];
gt.Const.Beards.SouthernUntidy <- [
	"southern_01",
	"southern_02",
	"southern_01",
	"southern_02",
	"01",
	"05",
	"07",
	"09",
	"14",
	"17"
];
gt.Const.Beards.Raider <- [
	"01",
	"03",
	"04",
	"05",
	"07",
	"08",
	"09",
	"12",
	"13",
	"14",
	"15"
];
gt.Const.Beards.Wild <- [
	"14"
];
gt.Const.Beards.WildExtended <- [
	"12",
	"13",
	"14",
	"15",
	"17"
];
gt.Const.Beards.Monk <- [
	"01",
	"05",
	"07",
	"09"
];
gt.Const.Beards.SouthernOnly <- [
	"southern_01",
	"southern_02"
];
gt.Const.Beards.Southern <- [
	"southern_01",
	"southern_02",
	"southern_01",
	"southern_02",
	"01",
	"06",
	"07",
	"09"
];
gt.Const.Beards.ZombieOnly <- [
	"zombie_01",
	"zombie_02",
	"zombie_03",
	"zombie_04"
];
gt.Const.Beards.Zombie <- [
	"zombie_01",
	"zombie_02",
	"zombie_03",
	"zombie_04",
	"01",
	"02",
	"03",
	"04",
	"05",
	"06",
	"07",
	"08",
	"09",
	"10",
	"11"
];
gt.Const.Beards.ZombieExtended <- [
	"zombie_01",
	"zombie_02",
	"zombie_03",
	"zombie_04",
	"01",
	"02",
	"03",
	"04",
	"05",
	"06",
	"07",
	"08",
	"09",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15"
];
gt.Const.Tattoos.All <- [
	"",
	"scar_01",
	"scar_02",
	"tattoo_01",
	"warpaint_01"
];
gt.Const.Tattoos.Wildman <- [];
gt.Const.Tattoos.Cultist <- [];
gt.Const.Bodies.AllMale <- [
	"bust_naked_body_00",
	"bust_naked_body_01",
	"bust_naked_body_02"
];
gt.Const.Bodies.Female <- [
	"bust_hexen_charmed_body_01"
];
gt.Const.Bodies.Barber <- clone this.Const.Bodies.AllMale;
gt.Const.Bodies.SouthernMale <- [
	"bust_naked_body_southern_00",
	"bust_naked_body_southern_01",
	"bust_naked_body_southern_02"
];
gt.Const.Bodies.Skinny <- [
	"bust_naked_body_00"
];
gt.Const.Bodies.SouthernSkinny <- [
	"bust_naked_body_southern_00"
];
gt.Const.Bodies.Muscular <- [
	"bust_naked_body_01"
];
gt.Const.Bodies.SouthernMuscular <- [
	"bust_naked_body_southern_01"
];
gt.Const.Bodies.Thick <- [
	"bust_naked_body_02"
];
gt.Const.Bodies.SouthernThick <- [
	"bust_naked_body_southern_01"
];
gt.Const.Bodies.Gladiator <- [
	"bust_naked_body_southern_01",
	"bust_naked_body_southern_02"
];
gt.Const.Bodies.SouthernSlave <- [
	"bust_naked_body_southern_00"
];
gt.Const.Bodies.NorthernSlave <- [
	"bust_naked_body_00"
];

