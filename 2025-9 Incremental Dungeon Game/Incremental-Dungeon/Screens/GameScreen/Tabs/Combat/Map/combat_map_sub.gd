extends Panel

const originalSize : Vector2 = Vector2(3840,2160)
func scaleMap() :
	var forcedSize = size
	var targetRatio = originalSize.x/originalSize.y
	var forcedRatio = forcedSize.x/forcedSize.y
	
	var scaleFactor : float
	if (forcedRatio > targetRatio) :
		scaleFactor = forcedSize.y/originalSize.y
	else :
		scaleFactor = forcedSize.x/originalSize.x
	for child in get_children() :
		child.scale = Vector2(scaleFactor, scaleFactor)
		child.position = (forcedSize-originalSize*scaleFactor)/2
	#recursiveScale(get_children(), scaleFactor)
	
#func recursiveScale(nodeArray, scaleFactor) :
	#for child in nodeArray :
		#if (child.get_child_count() != 0) :
			#

func _on_resized() -> void:
	scaleMap()
