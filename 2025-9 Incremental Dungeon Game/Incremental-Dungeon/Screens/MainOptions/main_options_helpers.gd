extends Node

func getDefaultSettings() -> Dictionary :
	var tempDict : Dictionary = {}
	tempDict["version"] = Definitions.currentVersion
	tempDict["Window Mode"] = 0
	tempDict["gameCompleted"] = false
	tempDict["globalEncyclopedia"] = {
		"beastiary" : {},
		"items" : []
	}
	tempDict["herophile"] = false
	tempDict["audio"] = AudioHandler.getDefaultMainOptionsDictionary()
	return tempDict
	
func handleSafetySave() :
	var settings = loadSettings()
	if (settings.get("version") == null || settings["version"] != Definitions.currentVersion) :
		var oldVersion
		if (settings.get("version") == null) :
			oldVersion = "Pre-V1.05 release"
		else :
			oldVersion = settings["version"]
		var dir = DirAccess.open("user://")
		if (dir.file_exists(Definitions.tempSlot5)) :
			dir.remove_absolute(Definitions.tempSlot5)
		dir.copy(Definitions.mainSettingsPath, Definitions.tempSlot5)
		dir.rename(Definitions.tempSlot5, Helpers.removeAffix(Definitions.mainSettingsPath, ".json") + "_" + oldVersion + ".json")
		
		settings["version"] = Definitions.currentVersion
		await handleDiskAccess(settings.duplicate(true))

func applyWindowMode(val) :
	if (val == 0) :
		DisplayServer.window_set_mode(DisplayServer.WINDOW_MODE_FULLSCREEN)
	elif (val == 1) :
		DisplayServer.window_set_mode(DisplayServer.WINDOW_MODE_WINDOWED)
	else :
		return
		
func loadSettings() :
	var disk = FileAccess.open(Definitions.mainSettingsPath, FileAccess.READ)
	if (disk == null) :
		return getDefaultSettings()
	var text = disk.get_as_text()
	if (text == null || text == "") :
		return getDefaultSettings()
	var dict = JSON.parse_string(text)
	if (dict == {}) :
		return getDefaultSettings()
	if (dict.get("gameCompleted") == null) :
		dict["gameCompleted"] = false
	if (dict.get("globalEncyclopedia") == null) :
		dict["globalEncyclopedia"] = {
			"beastiary" : {},
			"items" : []
		}
	var audioDict = AudioHandler.getDefaultMainOptionsDictionary()
	if (dict.get("audio") == null) :
		dict["audio"] = audioDict
	else :
		for key in audioDict.keys() :
			if (dict["audio"].get(key) == null) :
				dict["audio"][key] = audioDict[key]
	
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
		if (loadDict == null || loadDict == {}) :
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
		#print("skipping save due to mutex")
		saveMutex.unlock()
		return
	saveActive = true
	saveSettings_noCheck(settings)
	saveMutex.unlock()
func saveSettings_noCheck(settings) :
	taskList.append(WorkerThreadPool.add_task(func():handleDiskAccess(settings.duplicate(true))))
	
var purgeMutex = Mutex.new()
var purging : bool = false
var taskList : Array = []
func _process(_delta) :
	purgeMutex.lock()
	if (purging) :
		purgeMutex.unlock()
		return
	saveMutex.lock()
	if (taskList.size() > 0 && !saveActive) :
		purging = true
		purgeMutex.unlock()
		for task in taskList :
			WorkerThreadPool.wait_for_task_completion(task)
		taskList = []
		saveMutex.unlock()
		purgeMutex.lock()
		purging = false
		purgeMutex.unlock()
		return
	purgeMutex.unlock()
	saveMutex.unlock()
		
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

var myReady : bool = false
signal myReadySignal
func _ready() :
	await handleSafetySave()
	myReady = true
	emit_signal("myReadySignal")

func queueSaveSettings(settings) :
	SaveManager.globalSettings = settings
	while (true) :
		saveMutex.lock()
		if (!saveActive) :
			break
		saveMutex.unlock()
		await get_tree().process_frame
	#saveSettings(settings)
	saveActive = true
	saveSettings_noCheck(settings)
	saveMutex.unlock()
	print("queued main settings success")
