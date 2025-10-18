extends Control

func _ready() :
	resize_32()
	updateSize()
	
func resize_32() :
	if ($Icon.region_rect.size.x == 32) :
		var oldScale = $Icon.scale
		var newScale = Vector2(oldScale.x/2.0,oldScale.y/2.0)
		$Icon.scale = newScale
	
func updateSize() :
	var oldX = $Icon.region_rect.position.x
	var oldY = $Icon.region_rect.position.y
	#32x32 image magnified by default to 80x80 (not 1 to 1, look into that later)
	if ($Icon.region_rect.size.x == 32) :
		var newX = 32*floor(oldX/32)
		var newY = 32*floor(oldY/32)
		$Icon.region_rect = Rect2(newX, newY, 32, 32)
		custom_minimum_size = 32*$Icon.scale
	#16x16 image magnified by default to 80x80 (this IS 1 to 1)
	else :
		var newX = 16*floor(oldX/16)
		var newY = 16*floor(oldY/16)
		$Icon.region_rect = Rect2(newX, newY, 16, 16)
		custom_minimum_size = 16*$Icon.scale

func getTexture() :
	return $Icon.texture
func setTexture(val) :
	$Icon.texture = val
func getRegionRect() :
	return $Icon.region_rect
func setRegionRect(val) :
	$Icon.region_rect = val
func getScale() :
	return $Icon.scale
func setScale(val) :
	$Icon.scale = val
