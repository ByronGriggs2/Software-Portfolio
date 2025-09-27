@tool
extends "res://Graphic Elements/BigTextPanel/big_text_panel.gd"

signal continueSignal

func _on_my_button_pressed() -> void:
	emit_signal("continueSignal")
