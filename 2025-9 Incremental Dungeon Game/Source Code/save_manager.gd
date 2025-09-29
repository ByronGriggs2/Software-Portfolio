extends Node

func saveGame() :
	var centralisedGameState = {}
	for node in get_tree().get_nodes_in_group("Saveable") :
		centralisedGameState[node.get_path()] = node.getSaveDictionary()
	var saveFile = FileAccess.open("res://save.json", FileAccess.WRITE)
	saveFile.store_string(JSON.stringify(centralisedGameState))
	saveFile.close()
	
func loadGame() :
	if (!FileAccess.file_exists("res://save.json")) : return false
	var text = FileAccess.open("res://save.json", FileAccess.READ).get_as_text()
	var centralisedGameState = JSON.parse_string(text)
	for nodePath in centralisedGameState.keys() :
		var node = get_node_or_null(nodePath)
		if (node) :
			node.setFromSaveDictionary(centralisedGameState[nodePath])
	return true
