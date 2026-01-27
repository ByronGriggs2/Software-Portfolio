extends TextureButton

signal myPressed


func _on_pressed() -> void:
	emit_signal("myPressed", self)
