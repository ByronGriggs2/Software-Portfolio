@tool
extends EditorScript

const playerPath = "res://Images/CC0 Assets/Dungeon Crawl Stone Soup Full/player/"

const excludedResources = []

const includedDirectories = {
	"playerAsset" : playerPath
}

func _run() :
	print("NEW RUNNING")
	for textureType in includedDirectories.keys() :
		var scriptContent : String = "extends Node\n\n"
		var textures : Dictionary
		textures = getAllTexturesAsDictionary(includedDirectories[textureType])
		print("textures acquired")
		for filename in excludedResources : 
			for subdirectory in textures.keys() :
				var pos = textures[subdirectory].find(includedDirectories[textureType] + filename)
				if (pos != -1) :
					textures[subdirectory].remove_at(pos)
					print("removed ", filename)
		scriptContent += createDictionariesFromFilesystem(textures, textureType)
		var outPath = "res://ResourceReferences/" + textureType + "References.gd"
		var file = FileAccess.open(outPath, FileAccess.WRITE)
		file.store_string(scriptContent)
		file.close()
		print("FINISHED")
	
func createDictionary(myName : String, keys : Array, values : Array) :
	var retVal : String = ""
	#print("creating ", myName)
	retVal += "const " + myName + "Dictionary = {\n"
	for index in range(0,keys.size()) :
		#print ("creating ", keys[index], " ", values[index])
		if (index != 0) :
			retVal += ",\n"
		retVal += "\t\"%s\" : preload(\"%s\")" % [keys[index], values[index]]
	retVal += "}\n"
	return retVal
	
## This stringifies the dictionary given by the function directly below this one. It also creates a getter function for each subdirectory
## as well as a getter function for the main directory, so the coder doesn't have to know the filesystem off the top of their head
func createDictionariesFromFilesystem(textures : Dictionary, textureType : String) -> String :
	#print ("Running stringify")
	var retVal : String = ""
	for subdirectory in textures.keys() :
		#print("Stringifying ", subdirectory)
		#print("Size is ", str(textures[subdirectory].keys().size()), " ", str(textures[subdirectory].values().size()))
		retVal += createDictionary(subdirectory, textures[subdirectory].keys(), textures[subdirectory].values())
		retVal += "\n"
		retVal += "static func get" + subdirectory + "(textureName : String) :\n"
		retVal += "\treturn " + subdirectory + "Dictionary.get(textureName)"
		retVal += "\n\n"
	retVal += "static func get" + textureType + "(textureName : String) :\n"
	retVal += "\tvar retVal\n"
	for subdirectory in textures.keys() :
		retVal += "\tretVal = " + subdirectory + "Dictionary.get(textureName)\n"
		retVal += "\tif retVal != null : return retVal\n"
	retVal += "\treturn null\n\n"
	retVal += "static func getDictionary(type : String) :\n"
	for subdirectory in textures.keys() :
		retVal += "\tif type == \"" + subdirectory + "\" : \n"
		retVal += "\t\treturn " + subdirectory + "Dictionary\n" 
	#print("Stringified Dictionary as ", retVal)
	return retVal
	
## I can't think of a descriptive and accurate name for this function so I'll just describe it
## You give it a file directory, and it returns a dictionary containing all .png in the directory and its subdirectories.
## The Dictionary contains a nested dictionary for each subdirectory in the original directory, but the nesting stops there. 
## If subdirectory A contains sub-sub-directories AA and AB, the contents of AA and AB will be combined into a nested dictionary A
func getAllTexturesAsDictionary(directory : String) -> Dictionary :
	var retVal : Dictionary = {}
	var dir = DirAccess.open(directory)
	var directories = dir.get_directories()
	for myDir in directories :
		#print("Entering directory ", directory + myDir)
		#var shortDir = myDir.rstrip("/")
		retVal[myDir] = {}
		var texturePaths : Array[String] = getAllTexturesInDirectoryRecursive(directory + myDir, [])
		for texture in texturePaths :
			retVal[myDir][texture.get_file().get_basename()] = texture
	return retVal
	
func getAllTexturesInDirectoryRecursive(directory : String, currentValues : Array[String]) -> Array[String] :
	var retVal = currentValues
	
	var dir = DirAccess.open(directory)
	print ("dir is ", directory)
	var directories = dir.get_directories()
	#print(directories)
	for tempDir in directories :
		#print("Entering directory", tempDir)
		retVal = getAllTexturesInDirectoryRecursive(directory + "/" + tempDir + "/", retVal)
	var files = dir.get_files()
	for file in files :
		if (file.get_extension() == "png") :
			#print("adding file ", file)
			retVal.append(directory + "/" + file)
		elif (file.get_extension() != "import"):
			pass
			#print("did NOT add file \"", file, "\", incorrect file extension")
	return retVal
