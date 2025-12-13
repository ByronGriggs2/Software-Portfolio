@tool
extends EditorScript

func _run() :
	resetNewEquipmentGrouping()
	pass
	
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
	
func resetActorPresets() :
	performOnResourcesInFolderRecursive("res://Screens/GameScreen/Tabs/Combat/Actors", func(a):a.reset())

func performOnEquipmentFiles(toCall : Callable) :
	for key in Definitions.equipmentTypeDictionary.keys() :
		var path = Definitions.equipmentPaths[key]
		var dir = DirAccess.open(path)
		dir.list_dir_begin()
		var filename = dir.get_next()
		while (filename != "") :
			var equipment : Equipment = load(path + "/" + filename)
			toCall.call(equipment)
			ResourceSaver.save(equipment, path + "/" + filename)
			filename = dir.get_next()
			
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
		performOnResourcesInFolderRecursive(directories[currentIndex], toCall)
		currentIndex += 1
	var files = dir.get_files()
	currentIndex = 0
	while (currentIndex < directories.size()) :
		if (files[currentIndex].get_extension() == "tres") :
			var temp : Resource = load(dir.get_current_dir().path_join(files[currentIndex]))
			toCall.call(temp)
			ResourceSaver.save(temp, dir.get_current_dir().path_join(files[currentIndex]))
		
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
