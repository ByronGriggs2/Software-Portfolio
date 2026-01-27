extends Control

var derivedStatsReference : Node = null
var attackSpeedRef : Node = null
var damageRef : Node = null

func initialise(VBox : Node, derivedStats : Node, attackSpeedNumber : Node, damageNumber : Node) :
	connectRecursive(VBox)
	derivedStatsReference = derivedStats
	attackSpeedRef = attackSpeedNumber
	damageRef = damageNumber
	
func connectRecursive(item : Node) :
	if (item.has_signal("resized")) :
		item.connect("resized", _on_resized)
	for child in item.get_children() :
		connectRecursive(child)
		
func _on_resized() :
	var children = get_children()
	for index in range(0, children.size()) :
		var ref
		if (index == 0) :
			ref = derivedStatsReference
		elif (index == 1) :
			ref = attackSpeedRef
		elif (index == 2) :
			ref = damageRef
		remove_child(children[index])
		ref.add_child(children[index])
		#children[index].size = ref.size
		#var child : Control = children[index]
		#child.layout_mode = 
		#children[index].anchors_preset = 
	#$DerivedStatsTooltip.global_position = derivedStatsReference.global_position
	#$DerivedStatsTooltip.size = derivedStatsReference.size
	#$AttackSpeedTooltip.global_position = attackSpeedRef.global_position
	#$AttackSpeedTooltip.size = attackSpeedRef.size
	#$AttackDamageTooltip.global_position = damageRef.global_position
	#$AttackDamageTooltip.size = damageRef.size
