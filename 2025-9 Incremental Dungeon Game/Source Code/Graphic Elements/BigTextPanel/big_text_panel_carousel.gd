@tool
extends "res://Graphic Elements/BigTextPanel/big_text_panel.gd"

signal continueSignal

func initialise(options, details) :
	$ElementSlot/Carousel.options = options
	$ElementSlot/Carousel.details = details
	$ElementSlot/Carousel.refresh()

func _on_carousel_continue_move(details) -> void:
	$RichTextLabel.text = details

func _on_carousel_choice_made(option) -> void:
	emit_signal("continueSignal", option)
