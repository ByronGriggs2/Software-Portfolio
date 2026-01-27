extends Control

var is32 : bool = false
var is48 : bool = false

func _ready() :
	resize_32()
	updateSize()
	
func resize_32() :
	if ($Icon.region_rect.size.x == 32) :
		is32 = true
		var oldScale = $Icon.scale
		var newScale = Vector2(oldScale.x/2.0,oldScale.y/2.0)
		$Icon.scale = newScale
	elif ($Icon.region_rect.size.x == 48) :
		is48 = true
		var oldScale = $Icon.scale
		var newScale = Vector2(oldScale.x/3.0,oldScale.y/3.0)
		$Icon.scale = newScale
	
func updateSize() :
	var oldX = $Icon.region_rect.position.x
	var oldY = $Icon.region_rect.position.y
	var factor : int = 0
	if (is48) :
		factor = 48
	elif (is32) :
		factor = 32
	else :
		factor = 16
	var newX = factor*floor(oldX/factor)
	var newY = factor*floor(oldY/factor)
	$Icon.region_rect = Rect2(newX, newY, factor, factor)
	custom_minimum_size = factor*$Icon.scale

func getTexture() :
	return $Icon.texture
func setTexture(val) :
	$Icon.texture = val
func getRegionRect() :
	return $Icon.region_rect
func setRegionRect(val) :
	$Icon.region_rect = val
func getScale() :
	if (is32) :
		return $Icon.scale.x*2
	elif (is48) :
		return $Icon.scale.x*3
	else :
		return $Icon.scale.x
func setScale(val) :
	if (is32) :
		$Icon.scale = Vector2(val/2.0,val/2.0)
	elif (is48) :
		$Icon.scale = Vector2(val/3.0,val/3.0)
	else :
		$Icon.scale = Vector2(val,val)
	custom_minimum_size = Vector2(16*val,16*val)
	size = Vector2(0,0)
func isIs32() :
	return is32
func setIs32(val) :
	is32 = val
func getFlipped() -> bool :
	return $Icon.flip_h
func setFlipped(val : bool) :
	$Icon.flip_h = val
