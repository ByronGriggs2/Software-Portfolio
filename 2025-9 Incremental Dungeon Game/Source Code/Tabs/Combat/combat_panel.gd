extends Panel

signal victory
signal defeat
signal retreat

func resetCombat(friendlyCores : Array[ActorPreset], enemyCores : Array[ActorPreset]) :
	cleanup()
	var actorLoader = preload("Actors/combat_actor.tscn")
	for friend in friendlyCores :
		var newActor = actorLoader.instantiate()
		newActor.core = friend.duplicate(true)
		$FriendlyParty.add_child(newActor)
		newActor.connect("actionTaken", _on_friend_action_taken)
		newActor.size_flags_horizontal = 0
		newActor.size_flags_vertical = Control.SIZE_SHRINK_END
	for enemy in enemyCores :
		var newActor = actorLoader.instantiate()
		newActor.core = enemy.duplicate(true)
		$EnemyParty.add_child(newActor)
		newActor.connect("actionTaken", _on_enemy_action_taken)
		newActor.size_flags_horizontal = 0
		newActor.size_flags_vertical = 0
	resumeCombat()
	
func cleanup() :
	for child in $FriendlyParty.get_children() :
		child.visible = false
		child.queue_free()
	for child in $EnemyParty.get_children() :
		child.visible = false
		child.queue_free()
	
enum combatStatus {running, victory, defeat}		
func _process(_delta: float) -> void:
	if (paused) :
		return
	var status = getStatus()
	if (status == combatStatus.running) :
		doCombatStep()
		return
	elif (status == combatStatus.victory) :
		pauseCombat()
		emit_signal("victory")
	elif (status == combatStatus.defeat) :
		pauseCombat()
		emit_signal("defeat")
		
func _on_my_button_pressed() -> void:
	pauseCombat()
	emit_signal("retreat")

#Damage over time etc.
func doCombatStep() :
	pass
		
func getStatus() :
	var victoryBool : bool = true
	for child in $EnemyParty.get_children() :
		if (!child.core.dead) :
			victoryBool = false
	if (victoryBool) : return combatStatus.victory
	var defeatBool : bool = true
	for child in $FriendlyParty.get_children() :
		if (!child.core.dead) :
			defeatBool = false
	if (defeatBool) : return combatStatus.defeat
	return combatStatus.running
#######################
var paused : bool = true
func pauseCombat() :
	paused = true
	for child in $EnemyParty.get_children() :
		child.pause()
	for child in $FriendlyParty.get_children() :
		child.pause()
		
func resumeCombat() :
	paused = false
	for child in $EnemyParty.get_children() :
		child.resume()
	for child in $FriendlyParty.get_children() :
		child.resume()
########################
func _on_friend_action_taken(emitter, action : Action) :
	var target = getTarget($EnemyParty.get_children(), action)
	executeAction(emitter, action, target)
	
func _on_enemy_action_taken(emitter, action : Action) :
	var target = getTarget($FriendlyParty.get_children(), action)
	executeAction(emitter, action, target)

func getTarget(otherParty, action) :
	if (action.mode == Action.targetingMode.RAND) :
		return getRandomTarget(otherParty)
	elif (action.mode == Action.targetingMode.LOW) :
		return getLowestTarget(otherParty)
	else :
		return -1
		
func getRandomTarget(otherParty) :
	var validTargets : Array
	for unit in otherParty :
		if (!unit.core.dead) :
			validTargets.append(unit)
	if (validTargets.size() == 0) :
		return -1
	var randIndex = randi()%validTargets.size()
	return validTargets[randIndex]

func getLowestTarget(otherParty) :
	var lowestHP = otherParty[0].core.MAXHP
	var lowestTarget
	for unit in otherParty :
		if (unit.core.HP <= lowestHP && unit.core.HP > 0) :
			lowestHP = unit.core.HP
			lowestTarget = unit
	return lowestTarget

func executeAction(emitter, action, target) :
	if (target is int && target == -1) :
		return
	var attack
	var defense
	if (action.type == AttackAction.damageType.PHYS) :
		attack = emitter.core.PHYSATK
		defense = target.core.PHYSDEF
	elif (action.type == AttackAction.damageType.MAG) :
		attack = emitter.core.MAGATK
		defense = target.core.MAGDEF
	else :
		return
	var damage = ceil((attack/defense)*action.power)
	if (damage > target.core.HP) :
		target.core.HP = 0
	else :
		target.core.HP -= damage
