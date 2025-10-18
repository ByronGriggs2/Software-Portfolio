@tool
extends Line2D

@export var Room1 : Button
@export var Room2 : Button
enum myVisibilityEnum {invisible, halfVisible, fullVisible}
@export var visibilityOnStartup : myVisibilityEnum = myVisibilityEnum.invisible

func _ready() :
	if (Room1 == null || Room2 == null) :
		return
	if (Engine.is_editor_hint()) :
		fullReveal()
	elif (visibilityOnStartup == myVisibilityEnum.invisible) :
		visible = false
	elif (visibilityOnStartup == myVisibilityEnum.halfVisible) :
		halfReveal()
	elif (visibilityOnStartup == myVisibilityEnum.fullVisible) :
		fullReveal()
	await get_tree().process_frame
	points = [Room1.position, Room2.position]
	points[0].x += 0.5*(Room1.size.x)
	points[0].y += 0.5*(Room1.size.y)
	points[1].x += 0.5*(Room2.size.x)
	points[1].y += 0.5*(Room2.size.y)

func fullReveal() :
	visible = true
	default_color = Color(0,0,0,1)
func halfReveal() :
	if (getVisibility() != 1) :
		visible = true
		default_color = Color(0,0,0,0.5)
	
func getVisibility() :
	if (!visible) :
		return 0
	elif (is_equal_approx(default_color.a, 1)) :
		return 1
	elif (is_equal_approx(default_color.a,0.5)) :
		return 2
	else :
		return 3
