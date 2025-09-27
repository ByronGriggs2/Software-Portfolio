extends ProgressBar

signal actionReady

var warmup : float = 0.01
var paused : bool = false
var currentAction : Action = null

func _process(delta) :
	if (paused) :
		return
	var oldProgress = value
	var newProgress = oldProgress + delta/warmup
	if (newProgress >= 100) :
		value = 0
		emit_signal("actionReady", currentAction)
	else :
		value = newProgress

func setAction(val : Action) :
	warmup = 0.01*val.warmup
	currentAction = val
