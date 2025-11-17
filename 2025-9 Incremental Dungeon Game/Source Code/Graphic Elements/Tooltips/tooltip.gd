extends PanelContainer

func setTitle(val) :
	$HBoxContainer/VBoxContainer/Title.text = val
func setDesc(val) :
	$HBoxContainer/VBoxContainer/EncyclopediaTextLabel.setTextExceptKey(val, $HBoxContainer/VBoxContainer/Title.text)
func updateExtendTimer(time) :
	$HBoxContainer/TextureProgressBar.value = time*100
func isOnNestedTooltip() -> bool :
	return $HBoxContainer/VBoxContainer/EncyclopediaTextLabel.isOnNestedTooltip()
func extend() :
	mouse_filter = Control.MOUSE_FILTER_STOP
	$HBoxContainer/VBoxContainer/EncyclopediaTextLabel.mouse_filter = Control.MOUSE_FILTER_PASS
