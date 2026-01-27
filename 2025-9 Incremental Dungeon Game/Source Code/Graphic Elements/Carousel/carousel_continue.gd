extends "res://Graphic Elements/Carousel/carousel.gd"

signal choiceMade

func _on_continue_pressed() -> void:
	emit_signal("choiceMade", options[currentPos])
