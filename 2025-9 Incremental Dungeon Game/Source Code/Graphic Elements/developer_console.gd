extends CanvasLayer

signal consoleOpened
signal setPlayerCore
signal consoleClosed
signal getAllItems

func _ready() :
	if (Definitions.currentVersion != "V0.3 development") :
		queue_free()

func _input(event) :
	if (event.is_action_released("Open Console")) :
		if (!visible) :
			visible = true
			emit_signal("consoleOpened")
		else :
			visible = false
			emit_signal("consoleClosed")

func _on_line_edit_text_submitted(new_text: String) -> void:
	addLine(new_text)
	$Container/LineEdit.text = ""
	var parts = new_text.strip_edges().split(" ")
	var cmd = parts[0]
	var args = parts.slice(1, parts.size())
	if commands.has(cmd):
		commands[cmd].call(args)
	
func addLine(newText : String) :
	$Container/Label.text = $Container/Label.text + "\n" + newText
	
var commands = {
	"echo" : func(args): addLine(" ".join(args)),
	"setPlayerCore" : func(args) : emit_signal("setPlayerCore", args),
	"getAllItems" : func(_args) : emit_signal("getAllItems")
}
