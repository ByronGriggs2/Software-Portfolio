@tool
extends Node

func _ready() :
	if (!Engine.is_editor_hint()) :
		queue_free() 
		return
	while (Definitions == null) :
		await get_tree().process_frame
	#syncEquipmentFiles()
	#syncClassFiles()
	
#func syncEquipmentFiles() :
	#for key in Definitions.equipmentTypeDictionary.keys() :
		#var path = Definitions.equipmentPaths[key]
		#var dir = DirAccess.open(path)
		#dir.list_dir_begin()
		#var filename = dir.get_next()
		#while (filename != "") :
			#var equipment : Equipment = load(path + "/" + filename)
			#equipment.initDictionary()
			#ResourceSaver.save(equipment, path + "/" + filename)
			#filename = dir.get_next()
			
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
