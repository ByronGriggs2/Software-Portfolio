@tool
extends Resource

class_name Equipment
@export var title : String = "Sample title"
@export_multiline var description : String = "Sample description"
@export var myPacket : ModifierPacket = ModifierPacket.new()

func reset() :
	myPacket = ModifierPacket.new()
func getModifierPacket() -> ModifierPacket :
	return myPacket
