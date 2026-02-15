extends Node

var steamManager = null
func _ready() :
	if (!OS.has_feature("web")) :
		steamManager = load("res://Globals/steam_manager.gd").new()
	initialise()

func initialise() :
	if (steamManager == null) :
		return
	steamManager.initialise()

func unlockAchievement(val : Definitions.achievementEnum) :
	if (steamManager == null) :
		return
	steamManager.unlockAchievement(val)

func unlockLastAchievement() :
	if (steamManager == null) :
		return
	steamManager.unlockLastAchievement()

func unlockAllEquipsAchievement() :
	if (steamManager == null) :
		return
	steamManager.unlockAllEquipsAchievement()

func handleBiomeAchievement(val : MyEnvironment) :
	if (steamManager == null) :
		return
	steamManager.handleBiomeAchievement(val)
