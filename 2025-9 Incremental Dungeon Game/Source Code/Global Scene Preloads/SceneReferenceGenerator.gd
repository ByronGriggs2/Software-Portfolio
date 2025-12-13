@tool
extends EditorScript

const mainPath = "res://Global Scene Preloads"
const equipmentPath = mainPath + "/Equipment"
const equipmentResourcePath = equipmentPath + "/Resources"
const equipmentScenePath = equipmentPath + "/Scenes"
const trainingPath = mainPath + "/Training"

func _run() :
	var scriptContent : String = "#This auto-generated singleton preloads small nodes such as equipment and can instantiate them as needed\n"
	scriptContent += "extends Node\n\n"
	scriptContent = writeEquipment(scriptContent)
	#scriptContent = writeTraining(scriptContent)
	const outPath = "res://Global Scene Preloads/SceneLoader.gd"
	var file = FileAccess.open(outPath, FileAccess.WRITE)
	file.store_string(scriptContent)
	file.close()
	
func writeTraining(scriptContent) :
	scriptContent += "#Training section\n"
	scriptContent += "const trainingResourceDictionary = {\n"
	var dir = DirAccess.open(trainingPath)
	dir.list_dir_begin()
	var filename = dir.get_next()
	var trainingName = filename.get_basename().get_file()
	scriptContent += "\t\"%s\" : preload(\"%s\")" % [trainingName, trainingPath + "/" + filename]
	filename = dir.get_next()
	while(filename != "") : 
		trainingName = filename.get_basename().get_file()
		scriptContent += ",\n\t\"%s\" : preload(\"%s\")" % [trainingName, trainingPath + "/" + filename]
		filename = dir.get_next()
	scriptContent += "}\n\n"
	scriptContent += "func createTrainingResource(trainingName : String) :\n"
	scriptContent += "\treturn trainingResourceDictionary[trainingName]\n"
	scriptContent += "func trainingResourceKeys() :\n\treturn trainingResourceDictionary.keys()\n\n\n"
	return scriptContent
	
func writeEquipment(scriptContent) :
	scriptContent += "#Equipment section\n"
	scriptContent += "const equipmentResourceDictionary = {\n"
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (key != Definitions.equipmentTypeDictionary.keys()[0]) :
			scriptContent += ",\n"
		scriptContent += "#%s\n" % Definitions.equipmentTypeDictionary[key]
		var resourcePaths : Dictionary
		var resourceDir = equipmentResourcePath + "/" + Definitions.equipmentTypeDictionary[key]
		var dir = DirAccess.open(resourceDir)
		dir.list_dir_begin()
		var filename = dir.get_next()
		while (filename != "") :
			var itemName = filename.get_basename().get_file()
			resourcePaths[itemName] = resourceDir + "/" + filename
			filename = dir.get_next()
		var itemNames = resourcePaths.keys()
		if (!itemNames.is_empty()) :
			scriptContent += "\t\"%s\" : preload(\"%s\")" % [itemNames[0], resourcePaths[itemNames[0]]]
			for index in range(1,itemNames.size()) :
				scriptContent += ",\n"
				scriptContent += "\t\"%s\" : preload(\"%s\")" % [itemNames[index], resourcePaths[itemNames[index]]]
	scriptContent += "}\n\n"
	
	scriptContent += "const equipmentSceneDictionary = {\n"
	for key in Definitions.equipmentTypeDictionary.keys() :
		if (key != Definitions.equipmentTypeDictionary.keys()[0]) :
			scriptContent += ",\n"
		scriptContent += "#%s\n" % Definitions.equipmentTypeDictionary[key]
		var sceneDir = equipmentScenePath + "/" + Definitions.equipmentTypeDictionary[key] + "/"
		var scenePaths : Dictionary
		var dir = DirAccess.open(sceneDir)
		dir.list_dir_begin()
		var filename = dir.get_next()
		while (filename != "") :
			var itemName = filename.get_basename().get_file()
			scenePaths[itemName] = sceneDir + filename
			filename = dir.get_next()
		var itemNames = scenePaths.keys()
		if (!itemNames.is_empty()) :
			scriptContent += "\t\"%s\" : preload(\"%s\")" % [itemNames[0], scenePaths[itemNames[0]]]
			for index in range(1,itemNames.size()) :
				scriptContent += ",\n"
				scriptContent += "\t\"%s\" : preload(\"%s\")" % [itemNames[index], scenePaths[itemNames[index]]]
	scriptContent += "}\n\n"
	
	scriptContent += "func createEquipmentScene(itemName : String) :\n"
	scriptContent += "\tvar newScene = equipmentSceneDictionary[itemName].instantiate()\n"
	scriptContent += "\tnewScene.core = equipmentResourceDictionary[itemName]\n"
	scriptContent += "\treturn newScene\n\n"
	return scriptContent
