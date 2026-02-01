extends "res://Screens/GameScreen/Tabs/Combat/Actors/combat_actor.gd"

func setPortrait(val : Dictionary) :
	$Portrait.onLoad(val)

var oldSize2 = Vector2(0,0)
const portraitOriginalSize = Vector2(227,238.875)
func _process(_delta) :
	super(_delta)
	if (is_equal_approx(oldSize2.x,$Portrait.size.x) && is_equal_approx(oldSize2.y,$Portrait.size.y)) :
		return
	for child in $Portrait.get_children() :
		child.centered = true
		child.position = $Portrait.size/2.0
	var ratio = min($Portrait.size.x/portraitOriginalSize.x,$Portrait.size.y/portraitOriginalSize.y)
	$Portrait.setScale(6.438*ratio)
	oldSize2 = $Portrait.size
