@tool
extends Resource

class_name ActorPreset
@export var portrait : Texture2D = null
@export var text : String = "Undef Actor"
@export_multiline var description : String = ""
@export var MAXHP : float = -1
@export var AR : float = -1
@export var DR : float = -1
@export var PHYSDEF : float = -1
@export var MAGDEF : float = -1
@export var actions : Array[Action] = []
@export var drops : Array[Equipment] = []
@export var dropChances : Array[float] = []
@export var enemyGroups : EnemyGroups = EnemyGroups.new()

func setStat(type : Definitions.baseStatEnum, val : float) :
	if (Engine.is_editor_hint()) :
		return
	if (type == Definitions.baseStatEnum.MAXHP) : MAXHP = val
	elif (type == Definitions.baseStatEnum.AR) : AR = val
	elif (type == Definitions.baseStatEnum.DR) : DR = val
	elif (type == Definitions.baseStatEnum.PHYSDEF) : PHYSDEF = val
	elif (type == Definitions.baseStatEnum.MAGDEF) : MAGDEF = val
	else :
		return
		
const defaultSkillcheck = 614000
var actualSkillcheck = defaultSkillcheck
func adjustSkillcheck(floorDifference : int) :
	actualSkillcheck = defaultSkillcheck * pow(2,5.0*floorDifference/4.0)
func getSkillcheck() :
	return actualSkillcheck

func getStat(type : Definitions.baseStatEnum) -> float :
	if (Engine.is_editor_hint()) :
		return 0
	if (type == Definitions.baseStatEnum.MAXHP) : return MAXHP
	elif (type == Definitions.baseStatEnum.AR) : return AR
	elif (type == Definitions.baseStatEnum.DR) : return DR
	elif (type == Definitions.baseStatEnum.PHYSDEF) : return PHYSDEF
	elif (type == Definitions.baseStatEnum.MAGDEF) : return MAGDEF
	else :
		return -1

## Normally, a central encounter has a total power of 2, a side encounter 1, and a boss encounter 3.
## The reference power level is half the power level of the orc-- the orc is treated as a central encounter (not a boss),
## but because it's alone, it has the strength of 2 veterans. 
## The very first procedurally generated node will have a veteran power of 2*reference.
const referencePowerLevel = 58936
var myScalingFactor : float = -1
func getAdjustedCopy(scalingFactor : float) -> ActorPreset :
	var retVal = self.duplicate()
	retVal.resourceName = self.getResourceName()
	retVal.myScalingFactor = scalingFactor
	if (!enemyGroups.isEligible && !(getResourceName() == "apophis") && !(getResourceName() == "athena")) :
		return retVal
	var strengthMultiplier
	if (enemyGroups.enemyQuality == enemyGroups.enemyQualityEnum.normal) :
		strengthMultiplier = 0.333
	elif (enemyGroups.enemyQuality == enemyGroups.enemyQualityEnum.veteran) :
		strengthMultiplier = 1.0
	elif (enemyGroups.enemyQuality == enemyGroups.enemyQualityEnum.elite) :
		strengthMultiplier = 2.0
	else :
		return retVal
	var powerLevel = referencePowerLevel*scalingFactor*strengthMultiplier
	var eDPS = sqrt(powerLevel/(30/enemyGroups.glassCannonIndex))
	var eHP = powerLevel/eDPS
	var theoreticalDefense = sqrt(eHP/10/enemyGroups.HPToDefRatio)
	## Algebra'd from theoreticalDefense = AVERAGE(PHYSDEF, MAGDEF)
	## and defRatio = PHYSDEF/MAGDEF
	retVal.MAGDEF = (2*theoreticalDefense)/(enemyGroups.defRatio+1)
	retVal.PHYSDEF = enemyGroups.defRatio*retVal.MAGDEF
	retVal.MAXHP = eHP/theoreticalDefense
	if (!(actions[0].has_method("getPower"))) :
		retVal.AR = 0
		retVal.DR = 0
	else :
		var product = eDPS/EnemyDatabase.getAdasRatio()
		retVal.AR = sqrt(product*enemyGroups.atkRatio)
		retVal.DR = product/retVal.AR
	#retVal.resourceName = getResourceName()
	return retVal
	
signal droppedItems
func getDrops(magicFind) -> Array[Equipment] :
	var retVal : Array[Equipment] = []
	for index in range(0,drops.size()) :
		if (randf() < dropChances[index]*magicFind) :
			retVal.append(drops[index])
	emit_signal("droppedItems", retVal)
	return retVal
	
func getName() : return text
var resourceName : String = ""
func getResourceName() : 
	if (resourceName == "") :
		resourceName = resource_path.get_file().get_basename()
	return resourceName
func getDesc() : return description

func getSaveDictionary() -> Dictionary :
	var retVal = {}
	if (!enemyGroups.isEligible && !(getResourceName() == "apophis") && !(getResourceName() == "athena")) :
		return retVal 
	retVal["resourceName"] = getResourceName()
	if retVal["resourceName"] == null :
		pass
	if (getResourceName() == "athena") :
		retVal["actualSkillcheck"] = actualSkillcheck
		#print("problem")
	#retVal["myScalingFactor"] = myScalingFactor
	#if (myScalingFactor != -1) :
		#retVal["drops"] = []
		#retVal["dropChances"] = []
		#for index in range (0,drops.size()) :
			#retVal["drops"].append(drops[index].getItemName())
			#retVal["dropChances"].append(dropChances[index])
	return retVal
	
## If you ever make this virtual you have to remove static and use the stupid pattern
static func createFromSaveDictionary(val : Dictionary, scaling) -> ActorPreset :
	if (val.is_empty()) :
		return ActorPreset.new()
	var scalingFactor
	if (scaling == null) :
		scalingFactor = val["myScalingFactor"]
	else :
		scalingFactor = scaling
	
	#if (scalingFactor == -1) :
	var enemy = EnemyDatabase.getEnemy(val["resourceName"]).getAdjustedCopy(scalingFactor)
	if (val.get("actualSkillcheck") != null) :
		enemy.actualSkillcheck = val.get("actualSkillcheck")
	return enemy
	#else :
		#var retVal = EnemyDatabase.getEnemy(val["resourceName"]).duplicate()
		#retVal.resourceName = val["resourceName"]
		#for index in range(0,val["drops"].size()) :
			#retVal.drops.append(EquipmentDatabase.getEquipment(val["drops"][index]))
			#retVal.dropChances.append(val["dropChances"][index])
		#return retVal
