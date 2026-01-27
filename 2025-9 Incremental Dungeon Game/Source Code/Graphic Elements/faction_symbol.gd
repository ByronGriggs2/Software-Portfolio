extends HBoxContainer

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
func _ready() :
	var currentLayer = Helpers.getTopLayer()
	var children = get_children()
	for index in range(0,children.size()) :
		var newTooltip = tooltipLoader.instantiate()
		children[index].add_child(newTooltip)
		newTooltip.initialise(EnemyGroups.factionDictionary[index as EnemyGroups.factionEnum])
		newTooltip.currentLayer = currentLayer
		var upperLeft = Vector2(0,0)
		var bottomRight = Vector2(32,32)*children[index].getScale() + Vector2(10,10)
		newTooltip.setPos(upperLeft, bottomRight)
	setSpriteSizeRecursive(get_children(), 5)	
		
func setSpriteSizeRecursive(children : Array[Node], val) :
	for child in children :
		if (child.has_method("setScale")) :
			child.setScale(val)
			child.visible = true
		setSpriteSizeRecursive(child.get_children(), val)

func setSymbolMutex(val : EnemyGroups.factionEnum) :
	clearAllSymbols()
	get_child(val).visible = true
	
func setSymbol(val : EnemyGroups.factionEnum) :
	get_child(val).visible = true

func clearAllSymbols() :
	for child in get_children() :
		child.visible = false
		
func setEnvironment(val : MyEnvironment) :
	clearAllSymbols()
	for key in EnemyGroups.factionDictionary.keys() :
		if (val.permittedFactions.find(key) != -1) :
			setSymbol(key)
