extends Node

const RoutineDictionary = {
	"lift_weights" : preload("res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/Routines/lift_weights.tres"),
	"pickpocket_goblins" : preload("res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/Routines/pickpocket_goblins.tres"),
	"punch_walls" : preload("res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/Routines/punch_walls.tres"),
	"read_novels" : preload("res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/Routines/read_novels.tres"),
	"spar" : preload("res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/Routines/spar.tres")}

func getRoutine(key : String) :
	return RoutineDictionary.get(key)

