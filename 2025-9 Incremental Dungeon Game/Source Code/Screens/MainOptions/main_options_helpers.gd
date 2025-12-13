extends Node

class_name MainOptionsHelpers

static func getDefaultSettings() -> Dictionary :
	var tempDict : Dictionary = {}
	tempDict["Window Mode"] = 0
	return tempDict

static func applyWindowMode(val) :
	if (val == 0) :
		DisplayServer.window_set_mode(DisplayServer.WINDOW_MODE_FULLSCREEN)
	elif (val == 1) :
		DisplayServer.window_set_mode(DisplayServer.WINDOW_MODE_WINDOWED)
	else :
		return
	
static func loadSettings() :
	var text = FileAccess.open(Definitions.mainSettingsPath, FileAccess.READ).get_as_text()
	return JSON.parse_string(text)
	
static func saveSettings(settings) :
	var saveFile = FileAccess.open(Definitions.mainSettingsPath, FileAccess.WRITE)
	saveFile.store_string(JSON.stringify(settings))
	saveFile.close()
