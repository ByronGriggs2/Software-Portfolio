@tool
extends "res://Graphic Elements/BigTextPanel/big_text_panel.gd"

signal continueSignal

func _on_my_button_pressed() -> void:
	emit_signal("continueSignal")
func setText(val) :
	$VBoxContainer/VBoxContainer/PanelContainer/RichTextLabel.setText(val)
func setButtonText(val) :
	$VBoxContainer/MyButton.text = " " + val + " "
func setTitle(val) :
	$VBoxContainer/VBoxContainer/Title.text = val
