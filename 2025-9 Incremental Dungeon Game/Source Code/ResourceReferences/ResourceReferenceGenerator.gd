@tool
extends EditorScript

const actorPath = "res://Screens/GameScreen/Tabs/Combat/Actors/"
const routinePath = "res://Screens/GameScreen/Tabs/Attributes/TrainingPanel/Routines/"
const environmentPath = "res://Screens/GameScreen/ProceduralGeneration/Environments/"
const equipmentPath = "res://Global Scene Preloads/Equipment/Resources/"
const excludedResources = ["fighter_preset.tres","human.tres","unarmed_Rogue.tres","unarmed_Mage.tres","unarmed_Fighter.tres"]

const includedDirectories = {
	"ActorPreset" : actorPath,
	"Routine" : routinePath,
	"Environment" : environmentPath,
	"Equipment" : equipmentPath
}

func _run() :
	print("NEW RUNNING")
	for resourceType in includedDirectories.keys() :
		var scriptContent : String = "extends Node\n\n"
		var values : Array[String] = []
		values = getAllResourcesInDirectoryRecursive(includedDirectories[resourceType], values)
		print("resources acquired")
		var keys : Array[String] = []
		for path in values :
			var tempResource = ResourceLoader.load(path)
			if (tempResource.has_method("runMe")) :
				tempResource.runMe()
			keys.append(tempResource.resource_path.get_file().get_basename())
		for filename in excludedResources : 
			var pos = values.find(includedDirectories[resourceType] + filename)
			if (pos != -1) :
				keys.remove_at(pos)
				values.remove_at(pos)
				print("removed ", filename)
		scriptContent += createDictionary(resourceType, keys, values)
		var outPath = "res://ResourceReferences/" + resourceType + "References.gd"
		var file = FileAccess.open(outPath, FileAccess.WRITE)
		file.store_string(scriptContent)
		file.close()
		print("FINISHED")
	
func createDictionary(myName : String, keys : Array[String], values : Array[String]) :
	var retVal : String = ""
	retVal += "const " + myName + "Dictionary = {\n"
	for index in range(0,keys.size()) :
		if (index != 0) :
			retVal += ",\n"
		retVal += "\t\"%s\" : preload(\"%s\")" % [keys[index], values[index]]
	retVal += "}\n\n"
	retVal += "func get" + myName + "(key : String) :\n"
	retVal += "\treturn " + myName + "Dictionary.get(key)\n\n"
	return retVal
	
func getAllResourcesInDirectoryRecursive(directory : String, currentValues : Array[String]) -> Array[String] :
	var retVal = currentValues
	var dir = DirAccess.open(directory)
	var directories = dir.get_directories()
	print(directories)
	for tempDir in directories :
		print("Entering directory", tempDir)
		retVal = getAllResourcesInDirectoryRecursive(directory + tempDir + "/", retVal)
	var files = dir.get_files()
	for file in files :
		if (file.get_extension() == "tres") :
			print("adding file ", file)
			retVal.append(directory + file)
	return retVal
