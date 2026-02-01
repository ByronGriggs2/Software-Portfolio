@tool
extends EditorScript

func _run() :
	performOnEquipmentFiles(func(a):a.addMultiplicity())
	
func resetAllEquipmentFiles() :
	performOnEquipmentFiles(func(a):a.reset())
	
func resetNewEquipmentFiles() :
	performOnEquipmentFiles(func(a):a.resetNew())
	
func resetOtherInEquipmentFiles() :
	performOnEquipmentFiles(func(a):a.resetOtherMods())
	
func resetNewEquipmentGrouping() :
	performOnEquipmentFiles(func(a):
		a.equipmentGroups = EquipmentGroups.new()
		a.equipmentGroups.isEligible = false
		)
		
func tagUpdate() :
	performOnNewEquipmentFiles(func(a):a.tagUpdate())
	
func resetActorPresets() :
	performOnResourcesInFolderRecursive("res://Screens/GameScreen/Tabs/Combat/Actors", func(a):a.reset())

const oldDir = "res://Resources/OldEquipment/"
const newDir = "res://Resources/NewEquipment/"
const paths = ["Accessories","Armor","Currency","Weapons"]
func performOnEquipmentFiles(toCall : Callable) :
	for key in Definitions.equipmentTypeDictionary.keys() :
		for index in range(0,2) :
			var path
			if (index == 0) :
				path = oldDir
			else :
				path = newDir
			path += paths[key]
			var dir = DirAccess.open(path)
			dir.list_dir_begin()
			var filename = dir.get_next()
			while (filename != "") :
				var equipment : Equipment = load(path + "/" + filename)
				toCall.call(equipment)
				ResourceSaver.save(equipment, path + "/" + filename)
				filename = dir.get_next()
			
func performOnNewEquipmentFiles(toCall : Callable) :
	performOnResourcesInFolderRecursive("res://Resources/NewEquipment/", toCall)
			
func performOnWeaponFiles(toCall : Callable) :
	var path = Definitions.equipmentPaths[Definitions.equipmentTypeEnum.weapon]
	var dir = DirAccess.open(path)
	dir.list_dir_begin()
	var filename = dir.get_next()
	while (filename != "") :
		var equipment : Equipment = load(path + "/" + filename)
		toCall.call(equipment)
		ResourceSaver.save(equipment, path + "/" + filename)
		filename = dir.get_next()
	
func performOnResourcesInFolderRecursive(directory : String, toCall : Callable) :
	var dir = DirAccess.open(directory)
	var directories = dir.get_directories()
	var currentIndex = 0
	while (currentIndex < directories.size()) :
		#print ("Entering directory: " + directories[currentIndex])
		performOnResourcesInFolderRecursive(directory + "/" + directories[currentIndex], toCall)
		currentIndex += 1
	var files = dir.get_files()
	currentIndex = 0
	while (currentIndex < files.size()) :
		if (files[currentIndex].get_extension() == "tres") :
			#print("Running on: " + files[currentIndex])
			var temp : Resource = load(dir.get_current_dir().path_join(files[currentIndex]))
			toCall.call(temp)
			ResourceSaver.save(temp, dir.get_current_dir().path_join(files[currentIndex]))
		currentIndex += 1
		
#func syncClassFiles() :
	#for key in Definitions.equipmentTypeDictionary.keys() :
		#var path = "res://Definitions/Classes"
		#var dir = DirAccess.open(path)
		#dir.list_dir_begin()
		#var filename = dir.get_next()
		#while (filename != "") :
			#var tempClass : CharacterClass = (path + "/" + filename)
			#tempClass.initDictionary()
			#ResourceSaver.save(tempClass, path + "/" + filename)
			#filename = dir.get_next()
