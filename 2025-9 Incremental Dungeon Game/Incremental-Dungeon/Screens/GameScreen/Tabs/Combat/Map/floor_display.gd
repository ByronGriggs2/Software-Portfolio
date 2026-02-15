extends HBoxContainer

signal floorUp
func _on_left_button_pressed() -> void:
	if (currentFloor == 0) :
		return
	emit_signal("floorUp")

signal floorDown
func _on_right_button_pressed() -> void:
	if (currentFloor == maxFloor) :
		return
	emit_signal("floorDown")

func setFloor(val : int, showMaxFloor : bool) :
	currentFloor = val
	if (val == 0 || maxFloor == 0) :
		$LeftButton.set_disabled(true)
	else :
		$LeftButton.set_disabled(false)
	if (val == maxFloor) :
		$RightButton.set_disabled(true)
	else :
		$RightButton.set_disabled(false)
	if (showMaxFloor) :
		$FloorNumber2.text = " " + str(val) + "/190" + " "
	else :
		$FloorNumber2.text = " " + str(val) + " "
	
func setMaxFloor(val : int) :
	maxFloor = val
	if (currentFloor == maxFloor - 1) :
		$RightButton.set_disabled(false)

var currentFloor : int = 0
var maxFloor : int = 0
