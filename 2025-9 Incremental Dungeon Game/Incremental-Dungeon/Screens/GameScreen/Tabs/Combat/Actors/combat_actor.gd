extends Panel

signal actionTaken
var core : ActorPreset
var HP : float = 1
var dead : bool = false
var combatPosition : int = -1
var isEnemy : bool = false

func HPBar() :
	return $ResourceCard/VBoxContainer/HPBar
	
const damageNumberLoader = preload("res://Graphic Elements/damage_number.tscn")
const estimatedSize : Vector2 = Vector2(92,40)
func setHP(val) :
	if (!$ResourceCard/VBoxContainer/ActionProgressBar.paused && val < HP) :
		var damage = HP - val
		var screenSize : Vector2i = Engine.get_singleton("DisplayServer").screen_get_size()
		var X0 = clamp(global_position.x-estimatedSize.x-50, 0, screenSize.x-estimatedSize.x)
		var X1 = clamp(global_position.x+size.x+50,0,screenSize.x-estimatedSize.x)
		var Y0
		var Y1
		if (isEnemy) :
			Y0 = clamp(global_position.y-50,get_parent().global_position.y+100,screenSize.y-estimatedSize.y)
			Y1 = global_position.y+size.y-estimatedSize.y
		else :
			Y0 = global_position.y+100
			Y1 = screenSize.y-estimatedSize.y
		var damageNumber = damageNumberLoader.instantiate()
		add_child(damageNumber)
		damageNumber.initialiseAndRun(damage, X0,X1,Y0,Y1)
	HP = val

func setHPText(val) :
	$ResourceCard/VBoxContainer/HPBar.setCurrentHP(val)

#Add more sophisticated AI later
func _ready() :
	if (Definitions.hasDLC && core.resourceName == "athena") :
		var style : StyleBoxFlat = get_theme_stylebox("panel").duplicate()
		style.bg_color = Color("ff7160")
		add_theme_stylebox_override("panel", style)
	$TitleCard/Title.text = core.text
	$Graphic.texture = core.portrait
	var width = await Helpers.getTextWidthWaitFrame($TitleCard/Title,null,core.text)
	originalSize.x = max(originalSize.x, width+20)
	#$TitleCard/Title.custom_minimum_size.x = width
	#$TitleCard.custom_minimum_size.x = width+20
	#custom_minimum_size.x = width+20
	setHP(core.MAXHP)
	#var chosenAction = chooseAction()
	takeAction(chooseAction())
	var screenSize : Vector2i = Engine.get_singleton("DisplayServer").screen_get_size()
	isEnemy = global_position.y < screenSize.y/2
	if (!isEnemy) :
		if (await getPlayerSubclass() == Definitions.subclass.barb) :
			$ResourceCard/VBoxContainer/ActionProgressBar.modifyActionWarmup(0.75)
		elif (await getPlayerSubclass() == Definitions.subclass.whirl) :
			var weapon = await getWeaponResource()
			if (weapon != null && weapon.equipmentGroups.weaponClass != EquipmentGroups.weaponClassEnum.ranged) :
				$ResourceCard/VBoxContainer/ActionProgressBar.modifyActionWarmup(0.84)
	
signal playerSubclassRequested
signal playerSubclassReceived
var waitingForPlayerSubclass : bool = false
var playerSubclass_comm = -1
func getPlayerSubclass() -> Definitions.subclass :
	if (playerSubclass_comm != -1) :
		return playerSubclass_comm as Definitions.subclass
	waitingForPlayerSubclass = true
	emit_signal("playerSubclassRequested", self)
	if (waitingForPlayerSubclass) :
		await playerSubclassReceived
	return playerSubclass_comm
func providePlayerSubclass(val) :
	playerSubclass_comm = val
	waitingForPlayerSubclass = false
	emit_signal("playerSubclassReceived")
	
signal weaponResourceRequested
signal weaponResourceReceived
var waitingForweaponResource : bool = false
var weaponResource_comm
func getWeaponResource() -> Weapon :
	waitingForweaponResource = true
	emit_signal("weaponResourceRequested", self)
	if (waitingForweaponResource) :
		await weaponResourceReceived
	return weaponResource_comm
func provideWeaponResource(val) :
	weaponResource_comm = val
	waitingForweaponResource = false
	emit_signal("weaponResourceReceived")
	
var originalSize = Vector2(227,367.5)
const titleFont = 18/367.5
const actionFont = 16/367.5
const progressFont = 15/367.5
const HPFont = 15/367.5

var oldSize
var firstProcess : bool = true
func _process(_delta) :
	if (firstProcess) :
		firstProcess = false
		updateFontSizes()
		oldSize = size
	else :
		if (!(is_equal_approx(size.x,oldSize.x)&&(is_equal_approx(size.y,oldSize.y)))) : 
			updateFontSizes()
			oldSize = size
	var windowSize = Engine.get_singleton("DisplayServer").window_get_size()
	#if (windowSize.x < windowSize.y) :
	var actorCount = get_parent().get_child_count()
	var myMaxWidth = get_parent().size.x/float(actorCount)
	var newHeight = get_parent().size.y*0.95
	var newWidth = newHeight/1.618
	if (newWidth > myMaxWidth*0.95) :
		newWidth = myMaxWidth*0.95
		newHeight = myMaxWidth*1.618
	if (newWidth < originalSize.x || newHeight < originalSize.y) :
		custom_minimum_size = originalSize
	else :
		custom_minimum_size = Vector2(newWidth,newHeight)

	if (checkDeath()) :
		return
	HPBar().setMaxHP(core.MAXHP)
	HPBar().setCurrentHP(HP)
	
func updateFontSizes() :
	$TitleCard/Title.add_theme_font_size_override("normal_font_size", titleFont*size.y)
	$ResourceCard/ActionLabel.add_theme_font_size_override("normal_font_size", actionFont*size.y)
	$ResourceCard/VBoxContainer/ActionProgressBar.add_theme_font_size_override("font_size",progressFont*size.y)
	$ResourceCard/VBoxContainer/HPBar.setFontSize(HPFont*size.y)

func pause() :
	$ResourceCard/VBoxContainer/ActionProgressBar.paused = true
	
func resume() :
	$ResourceCard/VBoxContainer/ActionProgressBar.paused = false

func takeAction(action) :
	$ResourceCard/ActionLabel.text = action.text
	$ResourceCard/VBoxContainer/ActionProgressBar.setAction(action)
	$ResourceCard/VBoxContainer/ActionProgressBar.value = 0
	
func checkDeath() :
	if (dead || HP <= 0) :
		dead = true
		$ResourceCard/ActionLabel.text = "Dead"
		HPBar().setCurrentHP(0)
		$ResourceCard/VBoxContainer/ActionProgressBar.value = 0
		$ResourceCard/VBoxContainer/ActionProgressBar.paused = true
		$ResourceCard/VBoxContainer/ActionProgressBar.show_percentage = false
		return true
	return false

func _on_action_progress_bar_action_ready(action) -> void:
	if (checkDeath()) :
		return
	emit_signal("actionTaken", self, action)
	#add more sophisticated AI later
	takeAction(chooseAction())
	
var flurryCounter : int = 0
func chooseAction() -> Action :
	var chosenAction = core.actions[randi_range(0,core.actions.size()-1)]
	if (Definitions.hasDLC && core.getResourceName() == "athena") :
		if (flurryCounter != 0) :
			flurryCounter -= 1
			return MegaFile.getNewAction("flurry")
		if (chosenAction == MegaFile.getNewAction("flurry")) :
			flurryCounter = 4
			return MegaFile.getNewAction("flurry")
	return chosenAction
	
