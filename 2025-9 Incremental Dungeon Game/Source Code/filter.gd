extends GridContainer

func option14(toggled_on : bool) -> void:
	$Option15/CheckBox.set_pressed_no_signal(!toggled_on)
func option15(toggled_on : bool) -> void:
	$Option14/CheckBox.set_pressed_no_signal(!toggled_on)
func option12(toggled_on : bool) -> void:
	if (toggled_on) :
		$Option13/CheckBox.set_pressed_no_signal(false)
		var startIndex = Helpers.findIndexInContainer(self, $Option)
		var children = get_children()
		for index in range(startIndex, children.size()) :
			if (children[index] is HBoxContainer) :
				children[index].get_node("CheckBox").set_pressed_no_signal(true)
func option13(toggled_on : bool) -> void:
	if (toggled_on) :
		$Option12/CheckBox.set_pressed_no_signal(false)
		var startIndex = Helpers.findIndexInContainer(self, $Option)
		var children = get_children()
		for index in range(startIndex, children.size()) :
			if (children[index] is HBoxContainer) :
				children[index].get_node("CheckBox").set_pressed_no_signal(false)
