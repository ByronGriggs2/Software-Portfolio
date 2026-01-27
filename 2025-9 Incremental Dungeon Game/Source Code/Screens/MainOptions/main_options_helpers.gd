extends Node

func getDefaultSettings() -> Dictionary :
	var tempDict : Dictionary = {}
	tempDict["Window Mode"] = 0
	tempDict["gameCompleted"] = false
	tempDict["globalEncyclopedia"] = {
		"beastiary" : {},
		"items" : []
	}
	tempDict["herophile"] = false
	return tempDict

func applyWindowMode(val) :
	if (val == 0) :
		DisplayServer.window_set_mode(DisplayServer.WINDOW_MODE_FULLSCREEN)
	elif (val == 1) :
		DisplayServer.window_set_mode(DisplayServer.WINDOW_MODE_WINDOWED)
	else :
		return
		
func loadSettings() :
	var text = FileAccess.open(Definitions.mainSettingsPath, FileAccess.READ).get_as_text()
	if (text == null || text == "") :
		return getDefaultSettings()
	var dict = JSON.parse_string(text)
	if (dict.get("gameCompleted") == null) :
		dict["gameCompleted"] = false
	if (dict.get("globalEncyclopedia") == null) :
		dict["globalEncyclopedia"] = {
			"beastiary" : {},
			"items" : []
		}
		createGlobalDictionary(dict)
	## because createGlobalDictionary is called before this line, old save files will unlock Athena if they've bought the DLC.
	## If the save files are new, this will not trigger, but that's fine because new save files track whether Herophile has been killed whether you have the DLC or note
	if (dict.get("herophile") == null) :
		dict["herophile"] = dict["globalEncyclopedia"]["beastiary"]["champion_of_poseidon"] >= 1
	return dict
	
## This function creates an up to date global beastiary for those with edited or outdated save files. It is not guaranteed to
## have the correct number of kills for every enemy, but it is guaranteed to correctly indicated whether an enemy has been killed before.
func createGlobalDictionary(dict) :
	for slot in range(1,5) :
		var loadDict = SaveManager.loadSaveDict(slot as Definitions.saveSlots)
		if (loadDict == null) :
			continue
		var enemies = loadDict["/root/EnemyDatabase"]["killed"]
		for key in enemies.keys() :
			if (dict["globalEncyclopedia"]["beastiary"].get(key) == null) :
				dict["globalEncyclopedia"]["beastiary"][key] = enemies[key]
			elif (dict["globalEncyclopedia"]["beastiary"][key] == 0 && enemies[key] != 0) :
				dict["globalEncyclopedia"]["beastiary"][key] = enemies[key]
	
var saveActive : bool = false
var saveMutex = Mutex.new()
func saveSettings(settings) :
	saveMutex.lock()
	if (saveActive) :
		saveMutex.unlock()
		return
	saveActive = true
	saveMutex.unlock()

	WorkerThreadPool.add_task(func():handleDiskAccess(settings))
	
func handleDiskAccess(settings) :
	#print("saving main settings. Window mode is " + str(settings["Window Mode"]) + ". Time is " + Time.get_datetime_string_from_system())
	var tempFile = FileAccess.open(Definitions.tempMainSlot, FileAccess.WRITE)
	var toStore = JSON.stringify(settings)
	tempFile.store_string(toStore)
	tempFile.close()
	
	var dir = DirAccess.open("user://")
	if (FileAccess.file_exists(Definitions.emergencyMainSlot) && FileAccess.file_exists(Definitions.mainSettingsPath)) :
		dir.remove(Definitions.emergencyMainSlot)
	if (FileAccess.file_exists(Definitions.mainSettingsPath)) :
		dir.copy(Definitions.mainSettingsPath,Definitions.emergencyMainSlot)
	if (FileAccess.file_exists(Definitions.mainSettingsPath)) :
		dir.remove(Definitions.mainSettingsPath)
	dir.rename(Definitions.tempMainSlot, Definitions.mainSettingsPath)
	saveMutex.lock()
	saveActive = false
	saveMutex.unlock()
