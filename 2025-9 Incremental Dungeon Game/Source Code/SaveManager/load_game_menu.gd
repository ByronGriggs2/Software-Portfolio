extends "res://Graphic Elements/popups/my_popup.gd"
const primaryTheme = preload("res://Graphic Elements/Themes/subTab.tres")
const secondaryTheme = preload("res://Graphic Elements/Themes/redTab.tres")

signal loadRequested
signal finished

func _ready() :
	for slot in range(1,5) :
		var currentButton = $Panel/CenterContainer/Window/VBoxContainer.get_child(slot+1)
		currentButton.setSlot(slot as Definitions.saveSlots)
	refreshSaveInformation()
	
func exists(val : Definitions.saveSlots) -> bool :
	return FileAccess.file_exists(Definitions.slotPaths[val])

func _on_slot_0_button_pressed(_throw) -> void:
	checkDelete(Definitions.saveSlots.slot0)

func _on_slot_1_button_pressed(_throw) -> void:
	checkDelete(Definitions.saveSlots.slot1)

func _on_slot_2_button_pressed(_throw) -> void:
	checkDelete(Definitions.saveSlots.slot2)

func _on_slot_3_button_pressed(_throw) -> void:
	checkDelete(Definitions.saveSlots.slot3)
	
const confirmLoad = preload("res://Graphic Elements/popups/binary_decision.tscn")
var checkDelete_tempSlot
func checkDelete(slot : Definitions.saveSlots) :
	if (!currentlyDeleting) :
		emit_signal("loadRequested", slot)
		emit_signal("finished")
		queue_free()
		return
	var confirm = confirmLoad.instantiate()
	add_child(confirm)
	confirm.setTheme(secondaryTheme)
	confirm.layer = layer + 1
	confirm.setTitle("Delete a save?")
	confirm.setText("Are you sure you want to delete " + Definitions.slotNames[slot] + "?")
	confirm.connect("binaryChosen", _on_confirm_chosen)
	confirm.nestedPopupInit(self)
	checkDelete_tempSlot = slot
	
func _on_confirm_chosen(val : int) :
	if (val == 0) :
		DirAccess.remove_absolute(Definitions.slotPaths[checkDelete_tempSlot])
		refreshSaveInformation()
		currentlyDeleting = false
		stopDelete()

func _on_return_button_pressed() -> void:
	emit_signal("finished")
	queue_free()

var currentlyDeleting : bool = false
func _on_delete_button_pressed() -> void:
	if (currentlyDeleting) :
		currentlyDeleting = false
		stopDelete()
	else :
		currentlyDeleting = true
		startDelete()

func startDelete() :
	for slot in range(1,5) :
		$Panel/CenterContainer/Window/VBoxContainer.get_child(slot+1).setTheme(secondaryTheme)
	for child in $Panel/CenterContainer/Window/VBoxContainer.get_children() :
		child.theme = secondaryTheme
	$Panel/CenterContainer/Window/VBoxContainer/VBoxContainer2/DeleteButton.text = "Cancel delete"
	
func stopDelete() :
	for slot in range(1,5) :
		$Panel/CenterContainer/Window/VBoxContainer.get_child(slot+1).setTheme(primaryTheme)
	for child in $Panel/CenterContainer/Window/VBoxContainer.get_children() :
		child.theme = primaryTheme
	$Panel/CenterContainer/Window/VBoxContainer/VBoxContainer2/DeleteButton.text = "Delete a save"
	
func refreshSaveInformation() :
	for slot in range(1,5) :
		var loadDict = SaveManager.loadSaveDict(slot as Definitions.saveSlots)
		var currentButton = $Panel/CenterContainer/Window/VBoxContainer.get_child(slot + 1)
		if (loadDict == null) :
			currentButton.hideExtra()
			currentButton.set_disabled(true)
		else :
			currentButton.setName((loadDict["/root/Main/GameScreen/Player"])["playerName"])
			currentButton.setTime((loadDict[SaveManager.get_path() as String])["playtime"])
			currentButton.showExtra()
			currentButton.set_disabled(false)
