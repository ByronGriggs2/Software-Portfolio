extends "res://Screens/GameScreen/Tabs/EquipmentTab/equipment_details.gd"

signal optionPressed
func _on_option_0_pressed() -> void:
	emit_signal("optionPressed", currentItemSceneRef, 0)

func _on_option_1_pressed() -> void:
	emit_signal("optionPressed", currentItemSceneRef, 1)

func setOption0(val : String) :
	$VBoxContainer/HBoxContainer2/CenterContainer0/Option0.text = val
func setOption1(val : String) :
	$VBoxContainer/HBoxContainer2/CenterContainer1/Option1.text = val
func setItemSceneRef(val) :
	setItemSceneRefBase(val)
