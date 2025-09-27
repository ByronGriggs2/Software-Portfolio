extends CanvasLayer

func _ready() :
	$Window.grab_focus()

func setTitle(newTitle) :
	$Window/VBoxContainer/Title.text = newTitle
	
func setText(newText) :
	$Window/VBoxContainer/Text.text = newText
