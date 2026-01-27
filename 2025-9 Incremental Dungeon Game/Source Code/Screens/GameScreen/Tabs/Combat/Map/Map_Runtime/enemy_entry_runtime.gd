extends HBoxContainer

var myEnemy : ActorPreset = null
func setEnemy(enemy : ActorPreset) :
	myEnemy = enemy
	$Name.text = " " + myEnemy.getName() + " "

func incrementKilled() :
	EnemyDatabase.incrementKills(myEnemy.getResourceName())
	
func getEnemy() :
	return myEnemy

signal addChildRequested
signal childAdded
var waitingForAddChild : bool = false
func unlockAddChild() :
	waitingForAddChild = false
	emit_signal("childAdded")
func requestAddChild(node : Node) :
	waitingForAddChild = true
	emit_signal("addChildRequested", self, node)
	if (waitingForAddChild) :
		await childAdded
const bigLoader = preload("res://Screens/GameScreen/Tabs/Combat/Map/Map_Runtime/enemy_entry_big_runtime.tscn")
func createBigEntry() :
	var bigEntry = bigLoader.instantiate()
	await requestAddChild(bigEntry)
	if (!bigEntry.myReady) :
		await bigEntry.myReadySignal
	bigEntry.initialise(self)
