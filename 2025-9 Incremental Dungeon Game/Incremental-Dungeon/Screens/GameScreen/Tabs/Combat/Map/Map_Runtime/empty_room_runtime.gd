extends "res://Screens/GameScreen/Tabs/Combat/Map/Map_Runtime/room_runtime.gd"

func isEmpty() :
	return true
	
func _ready() :
	super()
	
func setEncounter(val) :
	pass
func setVisibility(val) :
	pass
func onCombatComplete() :
	pass
func onCombatLoss() :
	pass
func onCombatRetreat() :
	pass
func getSaveDictionary() :
	return {}
func beforeLoad() :
	visited = true
	completed = true
	set_disabled(true)
	text = " Stairwell "
func onLoad(loadDict) :
	pass
