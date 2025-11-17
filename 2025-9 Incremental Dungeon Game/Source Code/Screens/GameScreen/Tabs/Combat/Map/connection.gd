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
	visible = false
	setVisibility(visibilityOnStartup as int)
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
	#Once something is revealed from the fog of war it can never be covered
	if (getVisibility() != myVisibilityEnum.fullVisible as int) :
		visible = true
		default_color = Color(0.31,0.31,0.31,1)
	
func getVisibility() :
	if (!visible) :
		return myVisibilityEnum.invisible as int
	elif (is_equal_approx(default_color.r,0.31)) :
		return myVisibilityEnum.halfVisible as int
	elif (is_equal_approx(default_color.r, 0)) :
		return myVisibilityEnum.fullVisible as int
		
func setVisibility(val) :
	if (val as myVisibilityEnum == myVisibilityEnum.invisible) :
		visible = false
	elif (val as myVisibilityEnum == myVisibilityEnum.halfVisible) :
		halfReveal()
	elif(val as myVisibilityEnum == myVisibilityEnum.fullVisible) :
		fullReveal()
