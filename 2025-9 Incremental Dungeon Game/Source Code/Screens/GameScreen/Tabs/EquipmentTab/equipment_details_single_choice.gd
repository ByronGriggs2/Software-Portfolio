extends "res://Screens/GameScreen/Tabs/EquipmentTab/equipment_details.gd"

signal pressed
func _on_button_pressed() -> void:
	emit_signal("pressed")
