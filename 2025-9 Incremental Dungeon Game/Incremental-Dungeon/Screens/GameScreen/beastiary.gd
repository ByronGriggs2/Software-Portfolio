extends "res://Graphic Elements/popups/my_popup_button.gd"

const bigEntryLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/enemy_entry_big.tscn")
const bigEntryLoaderNew = preload("res://Screens/GameScreen/Tabs/Combat/Map/Map_Runtime/enemy_entry_big_runtime.tscn")

var enemyDict : Dictionary = {}

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
	var enemyList : Array = EnemyDatabase.getAllEnemies().duplicate()
	enemyList.sort_custom(func(a,b):return a.getName()<b.getName())
	var index = 0
	while (index < enemyList.size()) :
		var enemy : ActorPreset = enemyList[index]
		if (!enemy.enemyGroups.isEligible && forcedInclude.find(enemy.getResourceName()) == -1) :
			enemyList.remove_at(index)
		else :
			index += 1
	$Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/RichTextLabel2.text = str(EnemyDatabase.getSoulCount())
	$Panel/CenterContainer/Window/VBoxContainer/HBoxContainer/RichTextLabel/TooltipTrigger.currentLayer = layer+1
	for enemy in enemyList :
		var newEntry = getContainer().get_node("Sample").duplicate()
		getContainer().add_child(newEntry)
		newEntry.visible = true
		var isDiscovered : bool
		if (IGOptions.getIGOptionsCopy()[IGOptions.options.globalEncyclopedia]) :
			var val = SaveManager.getGlobalSettings()["globalEncyclopedia"]["beastiary"].get(enemy.getResourceName())
			isDiscovered = val != null && val != 0
		else :
			isDiscovered = EnemyDatabase.getKillCount(enemy.getResourceName()) != 0
		if (!isDiscovered) :
			newEntry.setText("Undiscovered")
			newEntry.set_disabled(true)
		else :
			newEntry.setText(enemy.getName())
			enemyDict[newEntry.getText()] = enemy
		newEntry.connect("myPressed", _on_my_pressed)
		
func getContainer() :
	return  $Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer
	
func _on_my_pressed(emitter) :
	var enemy : ActorPreset = EnemyDatabase.getEnemy(enemyDict[emitter.getText()].getResourceName())
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
