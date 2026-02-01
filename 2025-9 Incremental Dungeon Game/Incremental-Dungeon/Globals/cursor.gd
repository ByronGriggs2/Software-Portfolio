extends Node

const sensitivity = 1.0
const cursorLoader = preload("res://Globals/cursorIcon.tscn")

var cursor

var mousePos = Vector2(0,0)
var initialised : bool = false
func _ready() :
	cursor = cursorLoader.instantiate()
	add_child(cursor)
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	await get_tree().process_frame
	calculatedPos = get_viewport().get_mouse_position()
	initialised = true
	
var myRelative = Vector2(0,0)
var calculatedPos = Vector2(0,0)
var skipFrame : bool = false
func _process(_delta) :
	if (!initialised) :
		return
	var vp = get_viewport()
	if (skipFrame) :
		calculatedPos = vp.get_mouse_position()
		clampPos()
		skipFrame = false
	else :
		calculatedPos += myRelative
		clampPos()
		vp.warp_mouse(calculatedPos)
	
	var event := InputEventMouseMotion.new()
	event.position = calculatedPos
	event.relative = Vector2(0,0)
	event.global_position = calculatedPos
	if (pressed) :
		event.button_mask = MOUSE_BUTTON_MASK_LEFT
	else :
		event.button_mask = 0
	Input.parse_input_event(event)  # manually forward the event
	
	cursor.get_child(0).global_position = calculatedPos
	myRelative = Vector2(0,0)

	#myRelative = Vector2(0,0)
	#if (pythag(calculatedPos-vp.get_mouse_position()) > 5) :
		#vp.warp_mouse(calculatedPos)
		#skipFrame = true
func pythag(val : Vector2) :
	return sqrt(pow(val.x,2)+pow(val.y,2))
var pressed : bool = false
func _input(event) :
	if (!initialised) :
		return
	if (event is InputEventMouseMotion) :
		myRelative += event.relative * sensitivity
		#var nevent := InputEventMouseMotion.new()
		#nevent.position = calculatedPos
		#nevent.relative = Vector2(0,0)
		#nevent.global_position = calculatedPos
		#nevent.button_mask = 0
		#Input.parse_input_event(nevent)
	if (event is InputEventMouseButton && event.button_mask == MOUSE_BUTTON_MASK_LEFT) :
		pressed = event.is_pressed()
func clampPos() :
	var screensize = get_viewport().get_visible_rect().size
	calculatedPos.x = clamp(calculatedPos.x,0,screensize.x)
	calculatedPos.y = clamp(calculatedPos.y,0,screensize.y)
			
	#if (pythag(myRelative) < 5) :
		#myRelative = Vector2(0,0)
		#calculatedPos = vp.get_mouse_position()
		#return
	
	
	
	#var mp = oldPos + myRelative
	#mp.x = round(mp.x)
	#mp.y = round(mp.y)
	#var distance = sqrt(pow(mp.x-oldPos.x,2)+pow(mp.y-oldPos.y,2))
	#if (distance > 50) :
		#vp.warp_mouse(mp)
	#oldPos = mp
	#myRelative = Vector2(0,0)
	
	

			
	#get_viewport().warp_mouse(get_viewport().get_mouse_position())
	
	#if (sqrt(pow(myRelative.x-lastWarp.x,2)+pow(myRelative.y-lastWarp.y,2)) < 5) :
		#lastWarp = Vector2(0,0)
		#return
	#myRelative -= lastWarp
	#lastWarp = myRelative
	#if (moveFrame) :
		#myWarp(myRelative)
		#myRelative = Vector2(0,0)
	#moveFrame = !moveFrame

#func myWarp(val : Vector2) :
	#var screensize = get_viewport().get_visible_rect().size
	#var newPos = Vector2(0,0)
	#mousePos.x = clamp(mousePos.x+val.x,0,screensize.x)
	#mousePos.y = clamp(mousePos.y+val.y,0,screensize.y)
	#get_viewport().warp_mouse(mousePos)
