extends Timer

func _on_timeout() -> void:
	SaveManager.saveGame(Definitions.saveSlots.current)
