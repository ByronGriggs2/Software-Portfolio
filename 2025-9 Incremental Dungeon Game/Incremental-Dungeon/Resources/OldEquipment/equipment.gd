@tool
extends Resource

class_name Equipment
@export var title : String = "Sample title"
@export_multiline var description : String = "Sample description"
@export var myPacket : ModifierPacket = ModifierPacket.new()

## For procedural generation
@export var equipmentGroups : EquipmentGroups = EquipmentGroups.new()

var resourceName : String = ""
func getItemName() :
	if (resourceName == "") :
		resourceName = resource_path.get_file().get_basename()
	return resourceName
func getAdjustedCopy(_scalingFactor : float) -> Equipment :
	var retVal = EquipmentDatabase.getEquipment(getItemName()).duplicate(true)
	retVal.resourceName = getItemName()
	return retVal
func createSampleCopy() -> Equipment :
	return getAdjustedCopy(1)
	
func addMultiplicity() :
	myPacket.otherMods[Definitions.otherStatDictionary[Definitions.otherStatEnum.routineMultiplicity]] = ModifierPacket.StandardModifier.duplicate()

func reset() :
	myPacket = ModifierPacket.new()
func resetNew() :
	if (myPacket == null) :
		myPacket = ModifierPacket.new()
	elif (myPacket.attributeMods.size() != Definitions.attributeDictionary.keys().size()) :
		myPacket = ModifierPacket.new()
	elif (myPacket.statMods.size() != Definitions.baseStatDictionary.keys().size()) :
		myPacket = ModifierPacket.new()
func resetOtherModsIfWrong() :
	var partialReset : bool = myPacket.otherMods.size() != Definitions.otherStatDictionary.keys().size()
	var fullReset = partialReset && myPacket.otherMods.size() < Definitions.otherStatEnum.routineSpeed_0 +1
	if (fullReset) :
		#print("Full reset: " + getItemName())
		var oldPacket = myPacket.duplicate()
		myPacket = ModifierPacket.new()
		myPacket.attributeMods = oldPacket.attributeMods
		myPacket.statMods = oldPacket.statMods
		#print(resource_path.get_file().get_basename())
	elif (partialReset) :
		#print("Partial reset: " + getItemName())
		var oldPacket = myPacket.duplicate()
		myPacket = ModifierPacket.new()
		myPacket.attributeMods = oldPacket.attributeMods
		myPacket.statMods = oldPacket.statMods
		## routine speed and routine effect used to be swapped
		for index in range(0,Definitions.otherStatEnum.routineEffect) :
			myPacket.otherMods[Definitions.otherStatDictionary[index]] = oldPacket.otherMods[Definitions.otherStatDictionary[index]]
		myPacket.otherMods["Routine Effect"] = oldPacket.otherMods["Routine Effect"]
		for index in range(Definitions.otherStatEnum.routineSpeed_0,Definitions.otherStatEnum.routineSpeed_5+1) :
			myPacket.otherMods[Definitions.otherStatDictionary[index]] = oldPacket.otherMods["Routine Speed"]
	else :
		pass
		#print("NO RESET: " + getItemName())

func getModifierPacket() -> ModifierPacket :
	return myPacket
func getName() :
	return title
func getTitle() :
	return getName()
func setTitle(val) :
	title = val
func getType() :
	return Definitions.equipmentTypeEnum.currency
func getSaveDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["resourceName"] = resourceName
	retVal["title"] = title
	return retVal
func createFromSaveDictionary(loadDict : Dictionary) -> Equipment :
	var resource = EquipmentDatabase.getEquipment(loadDict["resourceName"])
	if (!Helpers.equipmentIsNew(resource)) :
		return resource
	var retVal = resource.duplicate()
	retVal.resourceName = loadDict["resourceName"]
	retVal.title = loadDict["title"]
	return retVal
