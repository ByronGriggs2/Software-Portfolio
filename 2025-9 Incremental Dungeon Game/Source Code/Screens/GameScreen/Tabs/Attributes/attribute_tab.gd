extends Panel
func _process(_delta) :
	if (!doneLoading) :
		return
	if (!myReady) :
		return
	$AttributeMultipliers.myUpdate(playerModsCache)
	$AttributeBonuses.myUpdate(playerModsCache)
	#$AttributeLevels.updateRoutineSpeed(routineSpeedCache)

func _on_training_panel_training_changed(newTraining : AttributeTraining) -> void:
	$AttributeLevels.setMultipliers(newTraining)
	if (newTraining != null) :
		var multipliers = newTraining.getMultipliers()
		#$TrainingPanel.cacheMultipliers(multipliers)
		
func getAttributeLevels() -> Array[int] :
	var retVal : Array[int] = []
	for key in Definitions.attributeDictionary.keys() :
		retVal.append($AttributeLevels.getLevel(key))
	return retVal
	
signal tutorialRequested
var firstTimeSelected : bool = true
func _on_visibility_changed() -> void:
	if (visible && firstTimeSelected) :
		firstTimeSelected = false
		emit_signal("tutorialRequested", Encyclopedia.tutorialName.trainingTab, Vector2(0,0))

func getSaveDictionary() -> Dictionary :
	var tempDict = {}
	for key in Definitions.attributeDictionary.keys() :
		var attributeLevelKey : String = str(key) + "attributeLevel"
		var levelProgressKey : String = str(key) + "attributeLevelProgress"
		tempDict[attributeLevelKey] = $AttributeLevels.getLevel(key)
		tempDict[levelProgressKey] = $AttributeLevels.getProgress(key)
	tempDict["firstTimeSelected"] = firstTimeSelected
	return tempDict
	
var myReady : bool = false
signal myReadySignal
var doneLoading : bool = false
signal doneLoadingSignal
func _ready() :
	myReady = true
	emit_signal("myReadySignal")
		
func beforeLoad(_newSave : bool) :
	myReady = false
	$AttributeBonuses.setType("bonus")
	$AttributeMultipliers.setType("multiplier")
	for key in Definitions.attributeEnum.keys() :
		var attrNum = NumberClass.new()
		playerModsCache.append(attrNum)
	myReady = true
	emit_signal("myReadySignal")
	
func onLoad(loadDict) -> void :
	myReady = false
	for key in Definitions.attributeDictionary.keys() :
		var attributeLevelKey : String = str(key) + "attributeLevel"
		var levelProgressKey : String = str(key) + "attributeLevelProgress"
		$AttributeLevels.setLevel(key, loadDict[attributeLevelKey])
		$AttributeLevels.setProgress(key, loadDict[levelProgressKey])
	firstTimeSelected = loadDict["firstTimeSelected"]
	emit_signal("doneLoadingSignal")
	myReady = true
	emit_signal("myReadySignal")
	
func onLoad_2() -> void :
	myReady = false
	updateCat()
	myReady = true
	emit_signal("myReadySignal")
	doneLoading = true
	emit_signal("myReadySignal")

#signal playerClassRequested
#func _on_player_class_requested(emitter) -> void:
	#emit_signal("playerClassRequested", emitter)
	
var playerModsCache : Array[NumberClass] = []
#var routineSpeedCache : Array[float] = []
func setPlayerMods(newMods : Array[NumberClass]) :
	playerModsCache = newMods
	#routineSpeedCache = routineSpeed
	#$TrainingPanel.cacheRoutineSpeed(routineSpeed)
func initialiseNumberRefs(val) :
	$TrainingPanel.initialiseNumberRefs(val)

func unlockRoutine(routine) :
	$TrainingPanel.unlockRoutine(routine)
func upgradeRoutine(routine) :
	$TrainingPanel.upgradeRoutine(routine)
func onRespec() :
	for key in Definitions.attributeDictionary.keys() :
		var oldVal = $AttributeLevels.getLevel(key)
		$AttributeLevels.setLevel(key, round(oldVal/2.0))

signal playerPortraitRequested
signal playerPortraitReceived
var waitingForplayerPortrait : bool = false
var playerPortrait_comm = {}
func getplayerPortrait() -> Dictionary :
	waitingForplayerPortrait = true
	emit_signal("playerPortraitRequested", self)
	if (waitingForplayerPortrait) :
		await playerPortraitReceived
	return playerPortrait_comm
func provideplayerPortrait(val) :
	playerPortrait_comm = val
	waitingForplayerPortrait = false
	emit_signal("playerPortraitReceived")

func updateCat() :
	var portrait = await getplayerPortrait()
	var cat = portrait["textures"]["Cat"]
	$AttributeBonuses.getRightCol().get_node("CatPic").texture = MegaFile.getPlayerTexture_felids(cat)
