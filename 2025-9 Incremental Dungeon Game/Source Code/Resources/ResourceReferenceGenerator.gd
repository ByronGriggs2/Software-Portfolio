@tool
extends EditorScript

const myResourcePath = "res://Resources/"
const texturePath = "res://Resources/Textures/"

const excludedResources = []

func _run() :
	print("NEW RUNNING")
	var scriptContent : String = ""
	var resourceNames = getResourceNames(false)
	resourceNames.append_array(getResourceNames(true))
	var resourcePaths = getResourceDirectories(false)
	resourcePaths.append_array(getResourceDirectories(true))
	scriptContent = addMegaFile(scriptContent, resourceNames, resourcePaths)
	writeToDisk(scriptContent)
	#createResourceReference(resourceTypes, false)
	#createResourceReference(textureTypes, true)
	
func addMegaFile(scriptContent : String, resourceNames : Array[String], resourcePaths : Array[String]) -> String:
	var retVal = scriptContent +  "extends Node\n\n"
	for index in range(0, resourceNames.size()) :
		retVal = addDictionariesFromResource(retVal, resourceNames[index], resourcePaths[index])
		retVal += "\n################################################################################\n"
	return retVal
	
func getResourceNames(isTextures : bool) -> Array[String] :
	var retVal : Array[String] = []
	var dir
	if (isTextures) :
		dir = DirAccess.open(texturePath)
	else :
		dir = DirAccess.open(myResourcePath)
	var directories = dir.get_directories()
	for directory in directories :
		if (isTextures) :
			retVal.append(directory)
		else :
			if (directory == "Textures") :
				continue
			retVal.append(directory)
	return retVal

func getResourceDirectories(isTextures : bool) -> Array[String] :
	var retVal : Array[String] = []
	var dir
	if (isTextures) :
		dir = DirAccess.open(texturePath)
	else :
		dir = DirAccess.open(myResourcePath)
	var directories = dir.get_directories()
	for directory in directories :
		if (isTextures) :
			retVal.append(texturePath + directory)
		else :
			if (directory == "Textures") :
				continue
			retVal.append(myResourcePath + directory)
	return retVal

func addDictionariesFromResource(scriptContent : String, resourceName : String, resourcePath : String) -> String :
	var retVal = scriptContent
	var resourceFiles : Dictionary = getFileDictionary(resourcePath + "/")
	for filename in excludedResources : 
		for subdirectory in resourceFiles.keys() :
			#print ("looking for ", resourcePath + "/" + subdirectory + "/" + filename)
			var pos = resourceFiles[subdirectory].find_key(resourcePath + "/" + subdirectory + "/" + filename)
			if (pos != null) :
				resourceFiles[subdirectory].erase(pos)
				print("removed ", filename)
	retVal = addDictionariesFromFilesystem(retVal, resourceFiles, resourceName)
	return retVal

#func createResourceReference(resourceTypes, isTexture : bool) :
	#for resourceType in resourceTypes.keys() :
		#var scriptContent : String = "extends Node\n\n"
		#var resources : Dictionary = getAllResourcesAsDictionary(resourceTypes[resourceType] + "/", isTexture)
		#for filename in excludedResources : 
			#for subdirectory in resources.keys() :
				#var pos = resources[subdirectory].find_key(resourceTypes[resourceType] + filename)
				#if (pos != null) :
					#resources[subdirectory].erase(pos)
					#print("removed ", filename)
		#scriptContent += createDictionariesFromFilesystem(resources, resourceType)
		#var outPath
		#if (isTexture) :
			#outPath = texturePath + resourceType + "/" + resourceType + "References.gd"
		#else :
			#outPath = resourcePath + resourceType + "/" + resourceType + "References.gd"
		#var file = FileAccess.open(outPath, FileAccess.WRITE)
		##print ("outfile: ", outPath)
		##print(scriptContent)
		#file.store_string(scriptContent)
		#file.close()
		
func writeToDisk(val : String) :
	const outpath = "res://Resources/mega_resource_reference_file.gd"
	var file = FileAccess.open(outpath, FileAccess.WRITE)
	file.store_string(val)
	file.close()
	
func getFileDictionary(directory : String) :
	var retVal : Dictionary = {}
	#print("opening directory: ", directory)
	var dir = DirAccess.open(directory)
	var subDirectories = dir.get_directories()
	for myDir in subDirectories :
		#print("Entering subDirectory ", directory + myDir)
		retVal[myDir] = {}
		var resourceFilePaths : Array[String] = getAllResourceFilePathsInDirectoryRecursive(directory + myDir + "/", [])
		for resource in resourceFilePaths :
			retVal[myDir][resource.get_file().get_basename()] = resource
	return retVal
	
func getAllResourceFilePathsInDirectoryRecursive(directory : String, currentValues : Array[String]) -> Array[String] :
	var retVal = currentValues
	var dir = DirAccess.open(directory)
	#print("directory is ", directory)
	var directories = dir.get_directories()
	#print(directories)
	for tempDir in directories :
		#print("Entering directory", tempDir)
		retVal = getAllResourceFilePathsInDirectoryRecursive(directory + tempDir + "/", retVal)
	var files = dir.get_files()
	for file in files :
		if (file.get_extension() == "tres" || file.get_extension() == "png") :
			#print("adding file ", file)
			retVal.append(directory + file)
	return retVal
	
func createDictionary(prefix : String, affix : String, keys : Array, values : Array) :
	var retVal : String = ""
	retVal += "const " + prefix + "_" + affix + "Dictionary = {\n"
	for index in range(0,keys.size()) :
		if (index != 0) :
			retVal += ",\n"
		retVal += "\t\"%s\" : preload(\"%s\")" % [keys[index], values[index]]
	retVal += "}\n\n"
	return retVal

func addDictionariesFromFilesystem(scriptContent : String, resourceFiles : Dictionary, resourceName : String) -> String :
	#print ("Running stringify")
	var retVal = scriptContent
	for subdirectory in resourceFiles.keys() :
		#print("Stringifying ", subdirectory)
		#print("Size is ", str(resources[subdirectory].keys().size()), " ", str(resources[subdirectory].values().size()))
		retVal += createDictionary(resourceName, subdirectory, resourceFiles[subdirectory].keys(), resourceFiles[subdirectory].values())
		retVal += "\n"
		retVal += "func get" + resourceName + "_" + subdirectory + "(resourceName : String) :\n"
		retVal += "\treturn " + resourceName + "_" + subdirectory + "Dictionary.get(resourceName)"
		retVal += "\n\n"
	retVal += "func get" + resourceName + "(resourceName : String) :\n"
	#retVal += "\tvar retVal\n"
	for subdirectory in resourceFiles.keys() :
		retVal += "\tif (" + resourceName + "_" + subdirectory + "Dictionary.has(resourceName)) :\n"
		retVal += "\t\treturn " + resourceName + "_" + subdirectory + "Dictionary[resourceName]\n"
		#retVal += "\tretVal = " + subdirectory + "Dictionary.get(resourceName)\n"
		#retVal += "\tif retVal != null : return retVal\n"
	retVal += "\treturn null\n\n"
	retVal += "func get" + resourceName + "Dictionary(type : String) :\n"
	for subdirectory in resourceFiles.keys() :
		retVal += "\tif type == \"" + resourceName + "_" + subdirectory + "\" : \n"
		retVal += "\t\treturn " + resourceName + "_" + subdirectory + "Dictionary\n" 
	retVal += "\n"
	retVal += "func getAll" + resourceName + "() -> Array :\n"
	retVal += "\tvar retVal : Array = []\n"
	for subdirectory in resourceFiles.keys() :
		retVal += "\tretVal.append_array(" + resourceName + "_" + subdirectory + "Dictionary.values())\n"
	retVal += "\treturn retVal\n"
	#print("Stringified Dictionary as ", retVal)
	return retVal
