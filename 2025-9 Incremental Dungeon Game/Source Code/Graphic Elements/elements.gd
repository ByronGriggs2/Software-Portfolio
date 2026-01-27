extends HBoxContainer

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
const elementStrings = ["Earth", "Fire", "Ice", "Water"]
func _ready() :
	var children = get_children()
	for index in range(0,children.size()) :
		var newTooltip = tooltipLoader.instantiate()
		children[index].add_child(newTooltip)
		newTooltip.initialise(elementStrings[index])
		#newTooltip.currentLayer = Helpers.getTopLayer()
		var upperLeft = Vector2(0,0)
		var bottomRight = Vector2(16,16)*children[index].getScale()
		newTooltip.setPos(upperLeft, bottomRight)
		
func setLayer(val) :
	for child in get_children() :
		child.get_child(1).currentLayer = val

func setSymbolMutex(val : String) :
	clearAllSymbols()
	setSymbol(val)
	
func setSymbol(val : String) :
	for index in range(0,elementStrings.size()) :
		if (val == elementStrings[index]) :
			get_child(index).visible = true
			return

func clearAllSymbols() :
	for child in get_children() :
		child.visible = false
		
func setEnvironment(val : MyEnvironment) :
	clearAllSymbols()
	if (val.firePermitted) :
		setSymbol("Fire")
	if (val.earthPermitted) :
		setSymbol("Earth")
	if (val.waterPermitted) :
		setSymbol("Water")
	if (val.icePermitted) :
		setSymbol("Ice")
