extends "res://Screens/IntroScreen/portrait.gd"

const textureWidth = 32
var myReady : bool = false
func _ready() :
	applyScale()
	#_on_base_item_rect_changed()
	myReady = true

func _process(_delta) :
	fixPos()
	
func fixPos() :
	for child in get_children() :
		child.centered = true
		child.position = size/2.0

func setScale(val) :
	myScale = val
	for child in get_children() :
		child.scale = Vector2(myScale, myScale)
#var skipNext : bool = false
#func _on_base_item_rect_changed() -> void:
	#if (!myReady) :
		#return
	#if (skipNext) :
		#skipNext = false
		#return
	#fixPos()
	#skipNext = true
