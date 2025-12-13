extends ProgressBar

@export var maxHP : float = 1
var currentHP : float = 1

func setMaxHP(val) :
	maxHP = val
	setPercent(currentHP/maxHP)
func setPercent(val) :
	if (val < 0.01 && val > 0) :
		value = 1
	elif (val > 0.99 && val < 1.00) :
		value = 99
	else :
		value = val*100
	$HBoxContainer/Percent.text = "(" + str(Helpers.myRound(value,2)) + "%)"
func setCurrentHP(val) :
	currentHP = val
	$HBoxContainer/Number.text = str(Helpers.myRound(val,3))
	setPercent(currentHP/maxHP)
