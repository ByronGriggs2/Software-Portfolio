extends "res://Graphic Elements/popups/my_popup_button.gd"

func nestedPopupInit(val) :
	super(val)
	$Panel/CenterContainer/Window/VBoxContainer/DiscardFilter.setCurrentLayer(layer)
	
var myReady : bool = false
signal myReadySignal
func _ready() :
	super()
	if (!$Panel/CenterContainer/Window/VBoxContainer/DiscardFilter.myReady) :
		await $Panel/CenterContainer/Window/VBoxContainer/DiscardFilter.myReadySignal
	myReady = true
	emit_signal("myReadySignal")

func initalise(loadDict : Dictionary) :
	$Panel/CenterContainer/Window/VBoxContainer/DiscardFilter.initialise(loadDict)
func getData() -> Dictionary :
	return $Panel/CenterContainer/Window/VBoxContainer/DiscardFilter.getData()

signal savePressed
func _on_save_button_pressed() -> void:
	emit_signal("savePressed")
