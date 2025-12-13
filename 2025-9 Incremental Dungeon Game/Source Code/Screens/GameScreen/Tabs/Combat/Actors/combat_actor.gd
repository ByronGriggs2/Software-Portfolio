extends Panel

signal actionTaken
var core : ActorPreset
var combatPosition : int = -1
var isEnemy : bool = false

func HPBar() :
	return $ResourceCard/VBoxContainer/HPBar
	
const damageNumberLoader = preload("res://Graphic Elements/damage_number.tscn")
const estimatedSize : Vector2 = Vector2(92,40)
func setHP(val) :
	if (!$ResourceCard/VBoxContainer/ActionProgressBar.paused && val < core.HP) :
		var damage = core.HP - val
		var screenSize : Vector2i = Engine.get_singleton("DisplayServer").screen_get_size()
		var X0 = clamp(global_position.x-estimatedSize.x-50, 0, screenSize.x-estimatedSize.x)
		var X1 = clamp(global_position.x+size.x+50,0,screenSize.x-estimatedSize.x)
		var Y0
		var Y1
		if (isEnemy) :
			Y0 = clamp(global_position.y-50,100,screenSize.y-estimatedSize.y)
			Y1 = global_position.y+size.y-estimatedSize.y
		else :
			Y0 = global_position.y+100
			Y1 = screenSize.y-estimatedSize.y
		var damageNumber = damageNumberLoader.instantiate()
		add_child(damageNumber)
		damageNumber.initialiseAndRun(damage, X0,X1,Y0,Y1)
	core.HP = val

#Add more sophisticated AI later
func _ready() :
	$TitleCard/Title.text = core.text
	$Graphic.texture = core.portrait
	setHP(core.MAXHP)
	takeAction(core.actions[0])
	var screenSize : Vector2i = Engine.get_singleton("DisplayServer").screen_get_size()
	isEnemy = global_position.y < screenSize.y/2
	
func _process(_delta) :
	if (checkDeath()) :
		return
	HPBar().setMaxHP(core.MAXHP)
	HPBar().setCurrentHP(core.HP)

func pause() :
	$ResourceCard/VBoxContainer/ActionProgressBar.paused = true
	
func resume() :
	$ResourceCard/VBoxContainer/ActionProgressBar.paused = false

func takeAction(action) :
	$ResourceCard/ActionLabel.text = action.text
	$ResourceCard/VBoxContainer/ActionProgressBar.setAction(action)
	$ResourceCard/VBoxContainer/ActionProgressBar.value = 0
	
func checkDeath() :
	if (core.dead || core.HP <= 0) :
		core.dead = true
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
	takeAction(core.actions[0])
