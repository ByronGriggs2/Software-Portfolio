extends "res://Graphic Elements/popups/my_popup.gd"

signal optionChosen

func _ready() :
	for slot in range(1,5) :
		$Panel/CenterContainer/Window/VBoxContainer.get_child(slot+1).setSlot(slot as Definitions.saveSlots)
	addSaveInformation()

func _on_slot_0_button_pressed(_throw) -> void:
	saveSlotCheck(Definitions.saveSlots.slot0)

func _on_slot_1_button_pressed(_throw) -> void:
	saveSlotCheck(Definitions.saveSlots.slot1)

func _on_slot_2_button_pressed(_throw) -> void:
	saveSlotCheck(Definitions.saveSlots.slot2)

func _on_slot_3_button_pressed(_throw) -> void:
	saveSlotCheck(Definitions.saveSlots.slot3)

func _on_return_button_pressed() -> void:
	queue_free()

const menuLoader = preload("res://Graphic Elements/popups/binary_decision.tscn")
var saveSlotCheck_tempSlot
func saveSlotCheck(slot : Definitions.saveSlots) :
	if (!FileAccess.file_exists(Definitions.slotPaths[slot])) :
		emit_signal("optionChosen", slot)
		queue_free()
		return
	var menu = menuLoader.instantiate()
	add_child(menu)
	menu.layer = layer + 1
	menu.setTitle("Save Game")
	menu.setText("Are you sure you want to overwrite " + Definitions.slotNames[slot] + "?")
	menu.connect("binaryChosen", _on_confirmation_return)
	menu.nestedPopupInit(self)
	saveSlotCheck_tempSlot = slot
func _on_confirmation_return(val : int) :
	if (val == 0) :
		emit_signal("optionChosen", saveSlotCheck_tempSlot)
		queue_free()

func addSaveInformation() :
	for slot in range(1,5) :
		var loadDict = SaveManager.loadSaveDict(slot as Definitions.saveSlots)
		var currentButton = $Panel/CenterContainer/Window/VBoxContainer.get_child(slot + 1)
		if (loadDict == null) :
			currentButton.hideExtra()
		else :
			currentButton.setName((loadDict["/root/Main/GameScreen/Player"])["playerName"])
			currentButton.setTime((loadDict[SaveManager.get_path() as String])["playtime"])
			currentButton.showExtra()
