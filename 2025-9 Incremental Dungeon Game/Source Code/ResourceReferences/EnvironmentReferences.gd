extends Node

const EnvironmentDictionary = {
	"desert" : preload("res://Screens/GameScreen/ProceduralGeneration/Environments/desert.tres"),
	"fort" : preload("res://Screens/GameScreen/ProceduralGeneration/Environments/fort.tres"),
	"freeze_flame" : preload("res://Screens/GameScreen/ProceduralGeneration/Environments/freeze_flame.tres"),
	"magic_forest" : preload("res://Screens/GameScreen/ProceduralGeneration/Environments/magic_forest.tres"),
	"ocean" : preload("res://Screens/GameScreen/ProceduralGeneration/Environments/ocean.tres"),
	"tundra" : preload("res://Screens/GameScreen/ProceduralGeneration/Environments/tundra.tres"),
	"volcano" : preload("res://Screens/GameScreen/ProceduralGeneration/Environments/volcano.tres")}

func getEnvironment(key : String) :
	return EnvironmentDictionary.get(key)
