extends Panel

signal actionTaken
var core : ActorPreset
var combatPosition : int = -1

#Add more sophisticated AI later
func _ready() :
	$TitleCard/Title.text = core.text
	core.HP = core.MAXHP
	takeAction(core.actions[0])
	
func _process(_delta) :
	if (checkDeath()) :
		return
	$ResourceCard/VBoxContainer/HPBar.value = (core.HP/(1.0*core.MAXHP))*100
	if ($ResourceCard/VBoxContainer/HPBar.value <= 1) :
		$ResourceCard/VBoxContainer/HPBar.value = 1

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
		$ResourceCard/VBoxContainer/HPBar.value = 0
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
