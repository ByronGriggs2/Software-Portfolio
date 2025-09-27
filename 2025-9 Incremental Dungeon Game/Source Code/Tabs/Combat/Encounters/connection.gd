extends Line2D

@export var Room1 : Button
@export var Room2 : Button
enum myVisibilityEnum {invisible, halfVisible, fullVisible}
@export var visibilityOnStartup : myVisibilityEnum = myVisibilityEnum.invisible

func _ready() :
	if (visibilityOnStartup == myVisibilityEnum.invisible) :
		visible = false
		default_color = Color(0,0,0,0.5)
	elif (visibilityOnStartup == myVisibilityEnum.halfVisible) :
		visible = true
		default_color = Color(0,0,0,0.5)
	elif (visibilityOnStartup == myVisibilityEnum.fullVisible) :
		visible = true
		default_color = Color(0,0,0,1)
	await get_tree().process_frame
	points = [Room1.position, Room2.position]
	points[0].x += 0.5*(Room1.size.x)
	points[0].y += 0.5*(Room1.size.y)
	points[1].x += 0.5*(Room2.size.x)
	points[1].y += 0.5*(Room2.size.y)

func myEnable() :
	default_color = Color(0,0,0,1)
func myDisable() :
	default_color = Color(0,0,0,0.5)
