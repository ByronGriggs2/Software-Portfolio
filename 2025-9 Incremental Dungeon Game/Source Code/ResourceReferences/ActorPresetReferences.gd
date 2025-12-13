extends Node

const ActorPresetDictionary = {
	"goblin" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor0/goblin.tres"),
	"hobgoblin" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor0/hobgoblin.tres"),
	"orc" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor0/orc.tres"),
	"rat" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor0/rat.tres"),
	"zombie" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor0/zombie.tres"),
	"cave_mole" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/cave_mole.tres"),
	"dire_rat" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/dire_rat.tres"),
	"giant_magpie" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/giant_magpie.tres"),
	"goose_hydra" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/goose_hydra.tres"),
	"kiwi" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/kiwi.tres"),
	"mandragora" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/mandragora.tres"),
	"manticore" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/manticore.tres"),
	"mini_roc" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/mini_roc.tres"),
	"mountain_goat" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/mountain_goat.tres"),
	"mountain_lion" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/mountain_lion.tres"),
	"rock_dove" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/rock_dove.tres"),
	"vampire_bat" : preload("res://Screens/GameScreen/Tabs/Combat/Actors/Floor1/vampire_bat.tres")}

func getActorPreset(key : String) :
	return ActorPresetDictionary.get(key)
