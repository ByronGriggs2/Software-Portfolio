@tool
extends Resource

class_name Equipment
@export var title : String = "Sample title"
@export_multiline var description : String = "Sample description"
@export var myPacket : ModifierPacket = ModifierPacket.new()

## For procedural generation
@export var equipmentGroups : EquipmentGroups = EquipmentGroups.new()

func reset() :
	myPacket = ModifierPacket.new()
func resetNew() :
	if (myPacket == null) :
		myPacket = ModifierPacket.new()
	elif (myPacket.attributeMods.size() != Definitions.attributeDictionary.keys().size()) :
		myPacket = ModifierPacket.new()
	elif (myPacket.statMods.size() != Definitions.baseStatDictionary.keys().size()) :
		myPacket = ModifierPacket.new()
func resetOtherMods() :
	var oldPacket = myPacket.duplicate()
	myPacket = ModifierPacket.new()
	myPacket.attributeMods = oldPacket.attributeMods
	myPacket.statMods = oldPacket.statMods

func getModifierPacket() -> ModifierPacket :
	return myPacket
func getName() :
	return title
func getItemName() :
	return resource_path.get_file().get_basename()
func getType() :
	return Definitions.equipmentTypeEnum.currency
