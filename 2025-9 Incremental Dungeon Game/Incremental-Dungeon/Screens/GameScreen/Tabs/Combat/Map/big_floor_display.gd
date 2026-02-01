extends VBoxContainer

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
var biomeTooltip
func _ready() :
	biomeTooltip = tooltipLoader.instantiate()
	$BiomeLabel.add_child(biomeTooltip)
	biomeTooltip.initialise("Biome")
	biomeTooltip.currentLayer = Helpers.getTopLayer()

signal floorDown
func _on_floor_display_floor_down() -> void:
	emit_signal("floorDown")

signal floorUp
func _on_floor_display_floor_up() -> void:
	emit_signal("floorUp")

func setFloor(val) :
	$FloorDisplay.setFloor(val)
	
func setMaxFloor(val) :
	$FloorDisplay.setMaxFloor(val)
	
func setEnvironment(val : MyEnvironment) :
	$BiomeLabel.text = " " + val.getName().to_upper() + " "
	$FactionSymbol.clearAllSymbols()
	for faction : EnemyGroups.factionEnum in val.permittedFactions :
		$FactionSymbol.setSymbol(faction)
	$Elements.clearAllSymbols()
	if (val.firePermitted) :
		$Elements.setSymbol("Fire")
	if (val.earthPermitted) :
		$Elements.setSymbol("Earth")
	if (val.waterPermitted) :
		$Elements.setSymbol("Water")
	if (val.icePermitted) :
		$Elements.setSymbol("Ice")
	updateBiomeTooltip()

func updateBiomeTooltip() :
	while (!self.is_visible_in_tree()) :
		await self.visibility_changed
	await get_tree().process_frame
	await get_tree().process_frame
	var upperLeft = $BiomeLabel.size - Vector2($BiomeLabel.get_content_width(),$BiomeLabel.get_content_height())
	var bottomRight = $BiomeLabel.size
	biomeTooltip.setPos(upperLeft, bottomRight)

func setTutorialBiome() :
	$BiomeLabel.text = " TUTORIAL "
	$FactionSymbol.clearAllSymbols()
	$Elements.clearAllSymbols()
	updateBiomeTooltip()
