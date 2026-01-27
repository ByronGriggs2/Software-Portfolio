extends Control

@export var initSprite : Texture2D
@export var initStr : String = ""

func _ready() :
	$Sprite.texture = initSprite
	$TooltipTrigger.setTitle(initStr)
	var topLayer = Helpers.getTopLayer()
	$TooltipTrigger.currentLayer = topLayer + 1
	_on_skull_resized()

func _on_skull_resized() -> void:
	self.custom_minimum_size = Vector2($Sprite.size.x*$Sprite.scale.x,$Sprite.size.y*$Sprite.scale.y)
	$TooltipTrigger.custom_minimum_size = self.custom_minimum_size
	self.size = Vector2(0,0)
	$TooltipTrigger.size = Vector2(0,0)
