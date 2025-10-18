extends Control

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
	initPartyPositions($FriendlyParty.get_children())
	initPartyPositions($EnemyParty.get_children())
	resumeCombat()
	
func restartCombat() :
	var friendlyCores : Array[ActorPreset] = []
	for child in $FriendlyParty.get_children() :
		friendlyCores.append(child.core)
		friendlyCores.back().HP = friendlyCores.back().MAXHP
	var enemyCores : Array[ActorPreset] = []
	for child in $EnemyParty.get_children() :
		enemyCores.append(child.core)
		enemyCores.back().HP = enemyCores.back().MAXHP
	resetCombat(friendlyCores, enemyCores)
	
func initPartyPositions(party : Array) :
	if (party.size() == 1) :
		party[0].combatPosition = 2
	elif (party.size() == 2 || party.size() == 3) :
		party[0].combatPosition = 1
		party[1].combatPosition = 2
		if (party.size() == 3) :
			party[2].combatPosition = 3
	elif (party.size() == 4 || party.size() == 5) :
		for index in range(4) :
			party[index].combatPosition = 	index
		if (party.size() == 5) :
			party[4].combatPosition = 4 
	
func cleanup() :
	for child in $FriendlyParty.get_children() :
		child.visible = false
		child.free()
	for child in $EnemyParty.get_children() :
		child.visible = false
		child.free()
	
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
	var target = getTarget(emitter, $EnemyParty.get_children(), action)
	executeAction(emitter, action, target)
	
func _on_enemy_action_taken(emitter, action : Action) :
	var target = getTarget(emitter, $FriendlyParty.get_children(), action)
	executeAction(emitter, action, target)

func getTarget(emitter, otherParty, action) :
	if (action.mode == Action.targetingMode.RAND) :
		return getRandomTarget(otherParty)
	elif (action.mode == Action.targetingMode.LOW) :
		return getLowestTarget(otherParty)
	elif (action.mode == Action.targetingMode.STANDARD && action is AttackAction) :
		return getStandardTarget(emitter, otherParty)
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

func getStandardTarget(emitter, otherParty) :
	var myPos = emitter.combatPosition
	var target
	for index in range(5) :
		target = searchPartyAlive(otherParty, myPos+index)
		if (target != null) :
			return target
		target = searchPartyAlive(otherParty, myPos-index)
		if (target != null) :
			return target
	return null

func searchPartyAlive(party, pos:int) :
	for unit in party :
		if (unit.combatPosition == pos && !unit.core.dead) :
			return unit
	return null

func executeAction(emitter, action, target) :
	if (target is int && target == -1) :
		return
	var attack = emitter.core.AR
	var defense
	if (action.type == AttackAction.damageType.PHYS) :
		defense = target.core.PHYSDEF
	elif (action.type == AttackAction.damageType.MAG) :
		defense = target.core.MAGDEF
	else :
		return
	var damage = (attack/defense)*action.power
	if (damage > target.core.HP) :
		target.core.HP = 0
	else :
		target.core.HP -= damage
