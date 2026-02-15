extends "res://Graphic Elements/popups/my_popup_button.gd"

const bigEntryLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/enemy_entry_big.tscn")
const bigEntryLoaderNew = preload("res://Screens/GameScreen/Tabs/Combat/Map/Map_Runtime/enemy_entry_big_runtime.tscn")

var enemyDict : Array = []

const forcedInclude = [
	"goblin",
	"hobgoblin",
	"orc",
	"rat",
	"zombie",
	"athena",
	"apophis"
]

func _ready() :
	Helpers.highVisScroll($Panel/CenterContainer/Window/VBoxContainer/ScrollContainer)
	enemyDict = EnemyDatabase.getAllEnemies().duplicate()
	enemyDict.sort_custom(func(a,b):return a.getName()<b.getName())
	var index = 0
	while (index < enemyDict.size()) :
		var enemy : ActorPreset = enemyDict[index]
		if (!enemy.enemyGroups.isEligible && forcedInclude.find(enemy.getResourceName()) == -1) :
			enemyDict.remove_at(index)
		else :
			index += 1
	$Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/RichTextLabel2.text = str(EnemyDatabase.getSoulCount())
	$Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/RichTextLabel/TooltipTrigger.currentLayer = layer+1
	var copy = IGOptions.getIGOptionsCopy()
	var global = SaveManager.getGlobalSettings()
	for enemy in enemyDict :
		var newEntry = getContainer().get_node("Sample").duplicate()
		getContainer().add_child(newEntry)
		newEntry.visible = true
		var isDiscovered : bool
		if (copy[IGOptions.options.globalEncyclopedia]) :
			var val = global["globalEncyclopedia"]["beastiary"].get(enemy.getResourceName())
			isDiscovered = val != null && val != 0
		else :
			isDiscovered = EnemyDatabase.getKillCount(enemy.getResourceName()) != 0
		if (!isDiscovered) :
			newEntry.setText("Undiscovered")
			newEntry.set_disabled(true)
		else :
			newEntry.setText(enemy.getName())
			#enemyDict[newEntry.getText()] = enemy
		newEntry.connect("myPressed", _on_my_pressed)
		$Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer.move_child($Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer/Sample, $Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer.get_child_count()-1)
		
func getContainer() :
	return  $Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer
	
func findEntryIndex(emitter) :
	for index in range(0, enemyDict.size()) :
		if ($Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer.get_child(index) == emitter) :
			return index
	return -1
	
func _on_my_pressed(emitter) :
	var index = findEntryIndex(emitter)
	if (index == -1) :
		return
	var enemy : ActorPreset = EnemyDatabase.getEnemy(enemyDict[index].getResourceName())
	var bigEntry
	if (enemy.enemyGroups != null && (enemy.enemyGroups.isEligible || enemy.getResourceName() == "apophis") || enemy.getResourceName() == "athena") :
		bigEntry = bigEntryLoaderNew.instantiate()
	else :
		bigEntry = bigEntryLoader.instantiate()
	add_child(bigEntry)
	bigEntry.nestedPopupInit(self)
	bigEntry.setEnemy(enemy)


func _on_line_edit_text_changed(new_text: String) -> void:
	if (new_text == "") :
		for child in getContainer().get_children() :
			if (child == getContainer().get_node("Sample")) :
				continue
			child.visible = true
		return
	for child in getContainer().get_children() :
		if (child == getContainer().get_node("Sample")) :
			continue
		if (child.getText().to_upper().find(new_text.to_upper()) == -1 || child.getText() == "Undiscovered") :
			child.visible = false
		else :
			child.visible = true


func _on_my_check_box_2_pressed() -> void:
	if ($Panel/CenterContainer/Window/VBoxContainer/HBoxContainer2/myCheckBox.isPressed()) :
		for child in getContainer().get_children() :
			if (child == getContainer().get_node("Sample")) :
				continue
			child.visible = true
		$Panel/CenterContainer/Window/VBoxContainer/HBoxContainer2/LineEdit.clear()
	else :
		for child in getContainer().get_children() :
			if (child == getContainer().get_node("Sample")) :
				continue
			if (child.getText() == "Undiscovered") :
				child.visible = false
				
func _on_grid_container_filter_changed(val : Dictionary) -> void:
	var listContainer = $Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer
	var allSelected : bool = true
	for key in val.keys() :
		for subval in val[key] :
			if (!subval) :
				allSelected = false
				break
	for index in range(0,listContainer.get_child_count()-1) :
		var enemy : ActorPreset = enemyDict[index]
		if (!allSelected && (enemy.enemyGroups == null || (!enemy.enemyGroups.isEligible && enemy.getResourceName() != "athena" && enemy.getResourceName() != "apophis"))) :
			listContainer.get_child(index).visible = false
		elif (!val["technology"][enemy.enemyGroups.equipmentLevel] || !val["offense"][enemy.enemyGroups.enemyRange] || !val["defense"][enemy.enemyGroups.enemyArmor]) :
			listContainer.get_child(index).visible = false
		else :
			listContainer.get_child(index).visible = true
