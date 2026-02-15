extends Panel

const labelledBar = preload("res://Graphic Elements/ProgressBars/labelled_progress_bar.tscn")
enum AlignmentMode {
	ALIGNMENT_BEGIN,
	ALIGNMENT_CENTER,
	ALIGNMENT_END
}
var growthMultipliers : Array[float]

func _ready() :
	if (Definitions.DEVMODE) :
		$Panel/HBoxContainer/DevOptions.visible = true
	for key in Definitions.attributeDictionary.keys() :
		var newBar = labelledBar.instantiate()
		newBar.name = Definitions.attributeDictionary[key]
		$Panel/HBoxContainer/Con.add_child(newBar)
		newBar.size_flags_vertical = Control.SIZE_EXPAND_FILL
		newBar.setLabel(Definitions.attributeDictionary[key])
		#newBar.alignment = AlignmentMode.ALIGNMENT_CENTER
	var widestLabel = 0
	for key in Definitions.attributeDictionary.keys() :
		var width = $Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[key]).getLabelWidth()
		if (width > widestLabel) :
			widestLabel = width
	for key in Definitions.attributeDictionary.keys() :
		$Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[key]).setLabelWidth(widestLabel)
		
	for key in Definitions.attributeDictionary.keys() :
		growthMultipliers.append(0)
	
var multiplicityRef : NumberClass = null
func initialiseMultiplicityRef(val : NumberClass) :
	multiplicityRef = val
		
func setMultipliers(newTraining : AttributeTraining) :
	if (newTraining == null) :
		for key in Definitions.attributeDictionary.keys() :
			$Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[key]).setGrowthMultiplier(0)
	else :
		for key in Definitions.attributeDictionary.keys() :
			$Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[key]).setGrowthMultiplier(newTraining.getScaling(key))

var vettedCache : bool = false
func _process(_delta) :
	if (!vettedCache) :
		if ($Panel/HBoxContainer/Con.get_child_count() == Definitions.attributeDictionary.keys().size()) :
			vettedCache = true
		else :
			return
	for key in Definitions.attributeDictionary.keys() :
		$Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[key]).setMultiplicity(multiplicityRef.getFinal())
			
func getLevel(type : Definitions.attributeEnum) :
	return $Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[type]).getLevel()
func setLevel(type : Definitions.attributeEnum, val : float) :
	$Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[type]).setLevel(val)
func getProgress(type : Definitions.attributeEnum) :
	return $Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[type]).getProgress()
func setProgress(type : Definitions.attributeEnum, val) :
	$Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[type]).setProgress(val)

#func updateRoutineSpeed(val : Array[float]) :
	#for key in Definitions.attributeDictionary.keys() :
		#$Panel/HBoxContainer/Con.get_node(Definitions.attributeDictionary[key]).setSecondaryGrowthMultiplier(val[key])

func _on_dex_text_submitted(new_text: String) -> void:
	setLevel(Definitions.attributeEnum.DEX, int(new_text))


func _on_dur_text_submitted(new_text: String) -> void:
	setLevel(Definitions.attributeEnum.DUR, int(new_text))


func _on_int_text_submitted(new_text: String) -> void:
	setLevel(Definitions.attributeEnum.INT, int(new_text))


func _on_ski_text_submitted(new_text: String) -> void:
	setLevel(Definitions.attributeEnum.SKI, int(new_text))


func _on_line_edit_5_text_submitted(new_text: String) -> void:
	setLevel(Definitions.attributeEnum.STR, int(new_text))
