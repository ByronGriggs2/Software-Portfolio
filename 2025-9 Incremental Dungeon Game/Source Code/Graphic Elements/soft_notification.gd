extends RichTextLabel

var value : float = 0

func initialiseAndRun(val : String, minX, maxX, minY, maxY) :
	text = " [b]" + val + "[/b] "
	if (minX != null && maxX != null && minY != null && maxY != null) :
		global_position.x = randf_range(minX,maxX)
		global_position.y = randf_range(minY,maxY)
	var myTween : Tween = create_tween()
	myTween.tween_property(self, "position:y",position.y-150,3)
	myTween.parallel().tween_property(self, "modulate",Color(1,1,1,0),3)
	myTween.tween_callback(queue_free)
