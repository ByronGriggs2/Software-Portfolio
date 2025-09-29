extends Timer

func _on_timeout() -> void:
	SaveManager.saveGame()
