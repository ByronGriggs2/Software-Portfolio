@tool
extends Container
		
func _notification(what) :
	if (what == NOTIFICATION_SORT_CHILDREN) :
		var children = get_children()
		if (children.is_empty()) :
			return
		for child in children :
			fit_child_in_rect(child,Rect2(Vector2(), size))

func _on_child_order_changed() -> void:
	queue_sort()

func _ready() :
	if (!Engine.is_editor_hint()) :
		var children = get_children()
		if (children.is_empty()) :
			return
		for child in children :
			child.visible = false
		children[0].visible = true
