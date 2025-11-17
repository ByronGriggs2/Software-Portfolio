@tool
extends "res://Graphic Elements/BigTextPanel/big_text_panel.gd"

signal continueSignal

func initialise(options, details) :
	$VBoxContainer/Carousel.setOptions(options)
	$VBoxContainer/Carousel.details = details
	$VBoxContainer/Carousel.refresh()

func _on_carousel_continue_move(details) -> void:
	$VBoxContainer/VBoxContainer/PanelContainer/RichTextLabel.text = details

func _on_carousel_choice_made(option) -> void:
	emit_signal("continueSignal", option)
