extends Node

## createX() -> X implies that X.new() or X.duplicate() is called-- it returns a runtime instance
## getX() -> X implies that the "readonly" disk version of X is returned- or a reference to a member in another class

func createMap() -> MapData :
	var environment : MyEnvironment = createEnvironment()
	$EnemyPoolHandler.reset(environment, getAllEnemies())
	$ItemPoolHandler.reset(environment, getAllItems())
	var encounters : MapData = createEncounters()
	return encounters

const actorPresetList = preload("res://ResourceReferences/ActorPresetReferences.gd")
func getAllEnemies() :
	var list = actorPresetList.ActorPresetDictionary.values()
	for actor in list :
		if (!actor.enemyGroups.isEligible) :
			list.remove_at(list.find(actor))
		
const equipmentList = preload("res://ResourceReferences/EquipmentReferences.gd")
func getAllItems() :
	var list = equipmentList.EquipmentDictionary.values()
	for item in list :
		if (!item.equipmentGroups.isEligible) :
			list.remove_at(list.find(item))
	return list
	
const environmentList = preload("res://ResourceReferences/EnvironmentReferences.gd")
func createEnvironment() -> MyEnvironment :
	var range = environmentList.EnvironmentDictionary.size()-1
	var randKey = environmentList.EnvironmentDictionary.keys()[randi_range(0,range)]
	return environmentList.EnvironmentDictionary[randKey].duplicate()
	
func createEncounters() -> MapData :
	var retVal = MapData.new()
	var nodeCount = randi_range(3,6)
	for index in range(0,nodeCount) :
		retVal.centralEncounters.append(createCentralEncounter())
		var sideNodeCount = randi_range(1,4)
		for index2 in range(0, sideNodeCount) :
			retVal.sideEncounters.append(createSideEncounter())
	retVal.bossEncounter = createBossEncounter()
	return retVal
	
func addDrops(encounter : Encounter) -> void :
	for enemy in encounter.enemies :
		var itemPool = $ItemPoolHandler.getItemPoolForEnemy(enemy)
		$DropHandler.reset(itemPool)
		var newDrops = $Drophandler.getDrops()
		enemy.drops.append_array(newDrops)
		enemy.dropChances.append_array($DropHandler.getDropChances(newDrops.size()))
	
func createCentralEncounter() -> Encounter :
	var retVal = Encounter.new()
	retVal.enemies.append(getVeteran().duplicate())
	if (randi_range(0,2) == 0) :
		retVal.enemies.append(getVeteran().duplicate())
	else :
		for index in range(0,randi_range(2,3)) :
			retVal.enemies.append(getNormal().duplicate())
	addDrops(retVal)
	return retVal

func createSideEncounter() -> Encounter :
	var retVal = Encounter.new()
	if (randi_range(0,2)==0) :
		retVal.enemies.append(getVeteran().duplicate())
		if (randi_range(0,2) == 0) :
			retVal.enemies.append(getNormal().duplicate())
	elif (randi_range(0,9) == 0) :
		var enemy = getNormal().duplicate()
		for index in range(0,5) :
			retVal.enemies.append(enemy)
	else :
		for index in range(0, randi_range(2,3)) :
			retVal.enemies.append(getNormal().duplicate())
	addDrops(retVal)
	return retVal

func createBossEncounter() -> Encounter :
	var retVal = Encounter.new()
	retVal.enemies.append(getElite().duplicate())
	if (randi_range(0,1) == 0) :
		retVal.enemies.append(getVeteran().duplicate())
	else :
		for index in range(0,randi_range(2,3)) :
			retVal.enemies.append(getNormal().duplicate())
	addDrops(retVal)
	return retVal
		
func getNormal() -> ActorPreset :
	return $EnemyPoolHandler.getEnemyOfType(EnemyGroups.enemyQualityEnum.normal)
func getVeteran() -> ActorPreset :
	return $EnemyPoolHandler.getEnemyOfType(EnemyGroups.enemyQualityEnum.veteran)
func getElite() -> ActorPreset :
	return $EnemyPoolHandler.getEnemyOfType(EnemyGroups.enemyQualityEnum.elite)
