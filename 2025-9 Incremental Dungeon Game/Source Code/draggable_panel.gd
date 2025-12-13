extends PanelContainer

var isDragging : bool = false
var isHovered : bool = false
signal dragStart
signal dragStop
var isClicking : bool = false
var clickStartPos : Vector2 = Vector2(-1,-1)

func _input(event: InputEvent) -> void:
	if (isHovered && event is InputEventMouseButton && event.get_button_index() == 1) :
		if (event.is_pressed()) :
			isClicking = true
			clickStartPos = get_global_mouse_position()
		else :
			if (isClicking) :
				isClicking = false
				clickStartPos = Vector2(-1,-1)
			elif (isDragging) :
				accept_event()
				isDragging = false
				emit_signal("dragStop", self)
			# should be null set
			else :
				pass

func _process(_delta) :
	if (isDragging) :
		set_global_position(get_global_mouse_position()-size/2)
	elif (isClicking && clickStartPos != Vector2(-1,-1)) :
		var currentMousePos = get_global_mouse_position()
		if (sqrt(pow(currentMousePos.x-clickStartPos.x,2.0)+(pow(currentMousePos.y-clickStartPos.y,2.0))) >= 50) :
			isClicking = false
			isDragging = true
			emit_signal("dragStart", self)

func _on_mouse_entered() -> void:
	isHovered = true
	
func _on_mouse_exited() -> void:
	if (!isDragging) :
		isHovered = false
