extends RichTextLabel

var value : float = 0

func initialiseAndRun(val : float, minX, maxX, minY, maxY) :
	setValue(val)
	global_position.x = randf_range(minX,maxX)
	global_position.y = randf_range(minY,maxY)
	var myTween : Tween = create_tween()
	myTween.tween_property(self, "position:y",position.y-100,2)
	myTween.parallel().tween_property(self, "modulate",Color(1,0.5,0.5,0),2)
	myTween.tween_callback(queue_free)
func setValue(val) :
	value = val
	if (val == 99999999) :
		text = " " + str(int(val)) + " "
	else :
		text = " " + Helpers.engineeringRound(val,5) + " "
