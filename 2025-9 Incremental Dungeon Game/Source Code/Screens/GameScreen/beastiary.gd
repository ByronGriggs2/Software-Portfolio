extends "res://Graphic Elements/popups/my_popup_button.gd"

const bigEntryLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/enemy_entry_big.tscn")

func _ready() :
	var enemyList : Array = EnemyDatabase.getEnemyList()
	for enemy in enemyList :
		var newEntry = getContainer().get_node("Sample").duplicate()
		getContainer().add_child(newEntry)
		newEntry.visible = true
		if (EnemyDatabase.getKillCount(enemy) == 0) :
			newEntry.setText("Undiscovered")
			newEntry.set_disabled(true)
		else :
			newEntry.setText(enemy)
		newEntry.connect("myPressed", _on_my_pressed)
		
func getContainer() :
	return  $Panel/CenterContainer/Window/VBoxContainer/ScrollContainer/VBoxContainer
	
func _on_my_pressed(emitter) :
	var bigEntry = bigEntryLoader.instantiate()
	add_child(bigEntry)
	bigEntry.nestedPopupInit(self)
	bigEntry.setEnemy(EnemyDatabase.getEnemy(emitter.getText()))
