extends HBoxContainer

const bigLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/enemy_entry_big.tscn")

var myEnemy : ActorPreset

func setEnemy(enemy) :
	if (enemy is String) :
		myEnemy = EnemyDatabase.getEnemy(enemy)
	elif (enemy is ActorPreset) :
		myEnemy = enemy
	else :
		return
	$Name.text = " " + myEnemy.getName() + " "
	EnemyDatabase.connect("enemyDataChanged", _on_enemy_data_changed)
	
func getEnemy() :
	return myEnemy
	
func _on_enemy_data_changed(enemyName : String) :
	if (enemyName != myEnemy.getResourceName() && enemyName != "ALL") :
		return
	$Skull.visible = EnemyDatabase.getEnemyKilled(myEnemy.getResourceName())
	$Star.visible = EnemyDatabase.getAllEnemyDropsCollected(myEnemy.getResourceName())

func createBigEntry() :
	var bigEntry = bigLoader.instantiate()
	add_child(bigEntry)
	if (!bigEntry.myReady) :
		await bigEntry.myReadySignal
	bigEntry.setEnemy(myEnemy)
