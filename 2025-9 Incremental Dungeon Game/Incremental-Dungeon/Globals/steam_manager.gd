extends Node

func initialise() :
	if (steamInitialise()) :
		Definitions.steamEnabled = true
		## Compensate for old files
		unlockAllEquipsAchievement()
		unlockLastAchievement()

func steamInitialise() -> bool :
	var initialize_response: Dictionary = Steam.steamInitEx()
	print("Initialising steam...")
	if initialize_response['status'] <= Steam.STEAM_API_INIT_RESULT_OK:
		var dlc = Steam.getDLCData()
		var owned = Steam.isSubscribedApp(4352850)
		var installed = Steam.isDLCInstalled(4352850)
		print("DLC Data:")
		print(dlc)
		print("Owned: " + str(owned)) 
		print("Installed: " + str(installed))
		Definitions.hasDLC = owned && installed
		return true
	return false
	
func unlockAchievement(val : Definitions.achievementEnum) :
	if (!Definitions.steamEnabled):# || Definitions.GODMODE || Definitions.DEVMODE) :
		return
	if (!Steam.setAchievement(Definitions.achievementDictionary[val])) :
		print("Failed to unlock achievement: " + Definitions.achievementDictionary[val])
	if (!Steam.storeStats()) :
		print("Failed to store achievement: " + Definitions.achievementDictionary[val])
	unlockLastAchievement()
	
var oneHundredAchCache : bool = false
func unlockLastAchievement() :
	if (oneHundredAchCache) :
		return
	var oneHundredAch = Steam.getAchievement(Definitions.achievementDictionary[Definitions.achievementEnum.all_complete]) 
	var oneHundredCompleted : bool = oneHundredAch["achieved"]
	if (oneHundredCompleted) :
		oneHundredAchCache = true
		return
	var unlock : bool = true
	for key in Definitions.achievementDictionary.keys() :
		if (key == Definitions.achievementEnum.all_complete) :
			continue
		var ach = Steam.getAchievement(Definitions.achievementDictionary[key])
		if (!ach["achieved"]) :
			#print("achievement not unlocked: " + Definitions.achievementDictionary[key])
			unlock = false
			break
	if (unlock) :
		if (!Steam.setAchievement(Definitions.achievementDictionary[Definitions.achievementEnum.all_complete])) :
			print("Failed to unlock achievement: " + Definitions.achievementDictionary[Definitions.achievementEnum.all_complete])
		if (!Steam.storeStats()) :
			print("Failed to store achievement: " + Definitions.achievementDictionary[Definitions.achievementEnum.all_complete])

const forcedInclude = [
	"scraps",
	"shiv",
	"magic_stick_int",
	"magic_stick_str"
]
func unlockAllEquipsAchievement() :
	var achievementData = Steam.getAchievement(Definitions.achievementDictionary[Definitions.achievementEnum.all_equipment])
	if (achievementData["achieved"]) :
		return
	var items = MainOptionsHelpers.loadSettings()["globalEncyclopedia"]["items"]
	var allItems = EquipmentDatabase.getAllEquipment()
	for item : Equipment in allItems :
		var group = item.equipmentGroups
		if (!group.isEligible && !group.isSignature && forcedInclude.find(item.getItemName()) == -1) :
			continue
		if (Helpers.isDLC(item)) :
			continue
		if (items.find(item.getItemName()) == -1) :
			return
	unlockAchievement(Definitions.achievementEnum.all_equipment)
	
func handleBiomeAchievement(biome : MyEnvironment) :
	var achEnum = Definitions.biomeAchievementMap.get(biome.getFileName())
	if (achEnum != null) :
		unlockAchievement(achEnum)
