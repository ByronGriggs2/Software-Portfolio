extends "res://Graphic Elements/popups/my_popup.gd"
const primaryTheme = preload("res://Graphic Elements/Themes/myTheme.tres")
const secondaryTheme = preload("res://Graphic Elements/Themes/redTheme.tres")

signal optionChosen

func _ready() :
	refreshButtons()
	
func refreshButtons() :
	if (!exists(Definitions.saveSlots.slot0)) :
		$Window/VBoxContainer/Slot0Button.set_disabled(true)
	if (!exists(Definitions.saveSlots.slot1)) :
		$Window/VBoxContainer/Slot1Button.set_disabled(true)
	if (!exists(Definitions.saveSlots.slot2)) :
		$Window/VBoxContainer/Slot2Button.set_disabled(true)
	if (!exists(Definitions.saveSlots.slot3)) :
		$Window/VBoxContainer/Slot3Button.set_disabled(true)
	
func exists(val : Definitions.saveSlots) -> bool :
	return FileAccess.file_exists(Definitions.slotPaths[val])

func _on_slot_0_button_pressed() -> void:
	checkDelete(Definitions.saveSlots.slot0)

func _on_slot_1_button_pressed() -> void:
	checkDelete(Definitions.saveSlots.slot1)

func _on_slot_2_button_pressed() -> void:
	checkDelete(Definitions.saveSlots.slot2)

func _on_slot_3_button_pressed() -> void:
	checkDelete(Definitions.saveSlots.slot3)
	
const confirmLoad = preload("res://Graphic Elements/popups/binary_decision.tscn")
var checkDelete_tempSlot
func checkDelete(slot : Definitions.saveSlots) :
	if (!currentlyDeleting) :
		emit_signal("optionChosen", slot)
		queue_free()
		return
	var confirm = confirmLoad.instantiate()
	add_child(confirm)
	confirm.layer = layer + 1
	confirm.setTitle("Delete a save?")
	confirm.setText("Are you sure you want to delete " + Definitions.slotNames[slot] + "?")
	confirm.connect("binaryChosen", _on_confirm_chosen)
	checkDelete_tempSlot = slot
func _on_confirm_chosen(val : int) :
	if (val == 0) :
		DirAccess.remove_absolute(Definitions.slotPaths[checkDelete_tempSlot])
		refreshButtons()
		currentlyDeleting = false
		stopDelete()

func _on_return_button_pressed() -> void:
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
	for child in $Window/VBoxContainer.get_children() :
		if child is Button :
			child.theme = secondaryTheme
	for child in $Window/VBoxContainer2.get_children() :
		child.theme = secondaryTheme
	$Window/VBoxContainer2/DeleteButton.text = "Cancel delete"
	
func stopDelete() :
	for child in $Window/VBoxContainer.get_children() :
		if child is Button :
			child.theme = primaryTheme
	for child in $Window/VBoxContainer2.get_children() :
		child.theme = primaryTheme
	$Window/VBoxContainer2/DeleteButton.text = "Delete a save"
