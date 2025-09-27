extends VBoxContainer

func _ready() :
	var playerBox = RichTextLabel.new()
	add_child(playerBox) #I should probably just make a custom theme
	playerBox.name = "PlayerBox"
	playerBox.text = "Player Statistics"
	playerBox.fit_content = true
	playerBox.autowrap_mode = TextServer.AUTOWRAP_OFF
	playerBox.add_theme_color_override("default_color", Color.BLACK)
	playerBox.add_theme_font_size_override("normal_font_size", 50)
	for key in Definitions.baseStatDictionary :
		var textBox = playerBox.duplicate()
		add_child(textBox) #I should probably just make a custom theme
		textBox.add_theme_font_size_override("normal_font_size", 35)
		textBox.name = key
	
func setStat(stat : String, val) :
	$Player.core.setStat(stat, val)
	get_node(stat).text = Definitions.baseStatDictionary[stat] + ": " + str(val)
