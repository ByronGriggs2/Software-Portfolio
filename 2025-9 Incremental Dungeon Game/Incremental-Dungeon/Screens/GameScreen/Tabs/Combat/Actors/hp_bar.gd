extends ProgressBar

@export var maxHP : float = 1
var currentHP : float = 1

func setMaxHP(val) :
	if (strSet) :
		return
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
var strSet : bool = false
func setCurrentHP(val) :
	if (val is String) :
		$HBoxContainer/Number.text = val
		value = 100
		$HBoxContainer/Percent.text = "(100%)"
		strSet = true
		return
	elif (strSet) :
		return
	currentHP = val
	$HBoxContainer/Number.text = Helpers.engineeringRound(val,3)
	setPercent(currentHP/maxHP)
func setFontSize(val) :
	$HBoxContainer/Number.add_theme_font_size_override("normal_font_size", val)
	$HBoxContainer/Percent.add_theme_font_size_override("normal_font_size", val)
