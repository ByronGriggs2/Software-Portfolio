extends "res://Graphic Elements/Buttons/super_button.gd"

var core : Equipment = null

const equipmentPath = "res://Screens/GameScreen/Tabs/EquipmentTab"
var defaultSize : Vector2

func _ready() :
	super()
	defaultSize = $HBoxContainer/TextureRect.custom_minimum_size
	setBorderWidth(3)

func loadCore(type : Definitions.equipmentTypeEnum, filename : String) :
	core = load(Definitions.equipmentPaths[type] + filename + ".tres")
	$HBoxContainer/TextureRect.texture = load(equipmentPath + "/EquipmentIcons/" + filename + ".png")

func getCorePath() -> String :
	return core.resource_path
func setCorePath(path : String) -> void :
	core = load(path)
func getTexturePath() -> String :
	return $HBoxContainer/TextureRect.texture.resource_path
func setTexturePath(path : String) -> void :
	$HBoxContainer/TextureRect.texture = load(path)
func getMultiplicity() :
	return core.getMultiplicity()
func setMultiplicity(val) :
	core.setMultiplicity(val)
	
func getCoreRef() -> Equipment :
	return core
	
func setBorderWidth(val) :
	super(val)
	$HBoxContainer/TextureRect.custom_minimum_size = Vector2(defaultSize.x-2*val, defaultSize.y-2*val)
	
func isItem(item) :
	if (item is String && core.resource_path.get_file().get_basename() == item) :
		return true
	elif (item is String && core.resource_path == item) :
		return true
	elif (item == self) :
		return true
	elif (item == core) :
		return true
	return false
