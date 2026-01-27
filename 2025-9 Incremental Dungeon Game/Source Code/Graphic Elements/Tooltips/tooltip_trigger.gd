extends Control

@export var tooltipTitle : String = ""
@export_multiline var tooltipText : String = ""

var spawned : bool = false
var extended : bool = false
var onTooltip : bool = false
var onTrigger : bool = false

var myTooltip : Node = null
var myLayer : CanvasLayer = null

func initialise(key : String) :
	tooltipTitle = key
	tooltipText = Encyclopedia.descriptions[key]
	
func setTitle(val) :
	tooltipTitle = val
	
func setDesc(val) :
	tooltipText = val
	if (spawned) :
		myTooltip.setDesc(tooltipText)

func _on_mouse_entered() -> void:
	if (!spawned) :
		$SpawnTimer.start()
	onTrigger = true

func _on_mouse_exited() -> void:
	if (!spawned) :
		$SpawnTimer.stop()
	onTrigger = false

func _process(_delta) :
	if (!spawned) :
		return
		
	##Deleting tooltip logic
	##Trigger hovered
	if (onTrigger) :
		if (!$ExpirationTimer.is_stopped()) :
			$ExpirationTimer.stop()
	##Mouse off while not extended -> destroy
	elif (!extended && !onTooltip) :
		destroyTooltip()
		return
	elif (extended) :
		##Extended and on tooltip -> stop draining wheel
		if (onTooltip || myTooltip.isOnNestedTooltip()) :
			if (!$ExpirationTimer.is_stopped()) :
				$ExpirationTimer.stop()
		##Extended and off tooltip -> start draining wheel
		elif ($ExpirationTimer.is_stopped()) :
			$ExpirationTimer.start()
		
	##Draining the wheel
	if (!$ExpirationTimer.is_stopped()) :
		myTooltip.updateExtendTimer($ExpirationTimer.time_left/$ExpirationTimer.wait_time)
	##Filling the wheel
	else :
		myTooltip.updateExtendTimer(1-$ExtendTimer.time_left/$ExtendTimer.wait_time)

var currentLayer = 0
func setCurrentLayer(val) :
	currentLayer = val
	if (spawned) :
		myTooltip.setCurrentLayer(currentLayer + 1)
func _on_spawn_timer_timeout() -> void:
	myLayer = CanvasLayer.new()
	add_child(myLayer)
	myLayer.layer = currentLayer + 1
	var tooltipLoader = load("res://Graphic Elements/Tooltips/tooltip.tscn")
	myTooltip = tooltipLoader.instantiate()
	myLayer.add_child(myTooltip)
	myTooltip.setWidth(tooltipWidth)
	myTooltip.setCurrentLayer(currentLayer + 1)
	myTooltip.setTitle(tooltipTitle)
	myTooltip.setDesc(tooltipText)
	myTooltip.set_z_index(get_z_index() + 1)
	myTooltip.connect("mouse_entered", _on_tooltip_mouse_entered)
	myTooltip.connect("mouse_exited", _on_tooltip_mouse_exited)
	spawned = true
	$ExtendTimer.start()
	
func setPos(upperLeft : Vector2, lowerRight : Vector2) :
	position = upperLeft
	size = Vector2(lowerRight.x-upperLeft.x, lowerRight.y-upperLeft.y)
	
func _on_tooltip_mouse_entered() :
	onTooltip = true
func _on_tooltip_mouse_exited() :
	onTooltip = false
func _on_extend_timer_timeout() -> void:
	extended = true
	myTooltip.extend()
func _on_expiration_timer_timeout() -> void:
	extended = false
	destroyTooltip()
	
func destroyTooltip() :
	if (!$ExtendTimer.is_stopped()) :
		$ExtendTimer.stop()
	if (!$ExpirationTimer.is_stopped()) :
		$ExpirationTimer.stop()
	myLayer.queue_free()
	myLayer = null
	myTooltip = null
	spawned = false

func isOnNestedTooltip() -> bool :
	if (onTrigger || onTooltip) :
		return true
	if (myTooltip != null && myTooltip.isOnNestedTooltip()) :
		return true
	return false
	
var tooltipWidth = 400
func setTooltipWidth(val) :
	tooltipWidth = val
