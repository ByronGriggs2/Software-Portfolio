extends PanelContainer

var originalPos = Vector2(0,0)

func _ready() :
	recursiveConnect(get_children())
	originalPos = get_global_mouse_position()
	
func updatePos() :
	var toolPos = Vector2(originalPos.x+30,originalPos.y-30-size.y)
	var rect = get_viewport().get_visible_rect()
	var screenSize = rect.position + rect.size
	toolPos.x = clamp(toolPos.x, 0, screenSize.x - size.x)
	toolPos.y = clamp(toolPos.y, 0, screenSize.y - size.y)
	global_position = toolPos
	
func recursiveConnect(children : Array[Node]) :
	for child in children :
		if (child is Control) :
			child.connect("resized", _on_child_resized)
		if (child.get_child_count() != 0) :
			recursiveConnect(child.get_children())

func _on_child_resized() :
	size = Vector2(0,0)
	updatePos()

func setCurrentLayer(val) :
	$HBoxContainer/VBoxContainer/EncyclopediaTextLabel.currentLayer = val

func setTitle(val) :
	$HBoxContainer/VBoxContainer/Title.text = val
	
func setDesc(val) :
	if ($HBoxContainer/VBoxContainer/Title.text != "") :
		await $HBoxContainer/VBoxContainer/EncyclopediaTextLabel.setTextExceptKey(val, $HBoxContainer/VBoxContainer/Title.text)
	else :
		await $HBoxContainer/VBoxContainer/EncyclopediaTextLabel.setText(val)
	if (val == "") :
		$HBoxContainer/VBoxContainer/EncyclopediaTextLabel.visible = false
	
func updateExtendTimer(time) :
	$HBoxContainer/TextureProgressBar.value = time*100
	
func isOnNestedTooltip() -> bool :
	return $HBoxContainer/VBoxContainer/EncyclopediaTextLabel.isOnNestedTooltip()
	
func extend() :
	#mouse_filter = Control.MOUSE_FILTER_STOP
	#$HBoxContainer/VBoxContainer/EncyclopediaTextLabel.mouse_filter = Control.MOUSE_FILTER_PASS
	pass

func setWidth(val) :
	$HBoxContainer/VBoxContainer/Title.custom_minimum_size = Vector2(val,0)
	$HBoxContainer/VBoxContainer/EncyclopediaTextLabel.custom_minimum_size = Vector2(val,0)
