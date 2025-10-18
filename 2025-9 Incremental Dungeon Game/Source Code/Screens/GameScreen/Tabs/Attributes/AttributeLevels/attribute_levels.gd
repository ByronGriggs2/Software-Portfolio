extends Panel

const labelledBar = preload("res://Graphic Elements/ProgressBars/labelled_progress_bar.tscn")
enum AlignmentMode {
	ALIGNMENT_BEGIN,
	ALIGNMENT_CENTER,
	ALIGNMENT_END
}
var growthMultipliers : Array[float]

func _ready() :
	for key in Definitions.attributeDictionary.keys() :
		var newBar = labelledBar.instantiate()
		newBar.name = Definitions.attributeDictionary[key]
		$Con.add_child(newBar)
		newBar.setLabel(Definitions.attributeDictionary[key])
		#newBar.alignment = AlignmentMode.ALIGNMENT_CENTER
	var widestLabel = 0
	for key in Definitions.attributeDictionary.keys() :
		var width = $Con.get_node(Definitions.attributeDictionary[key]).getLabelWidth()
		if (width > widestLabel) :
			widestLabel = width
	for key in Definitions.attributeDictionary.keys() :
		$Con.get_node(Definitions.attributeDictionary[key]).setLabelWidth(widestLabel)
		
	for key in Definitions.attributeDictionary.keys() :
		growthMultipliers.append(0)
		
func setMultipliers(newTraining : AttributeTraining) :
	if (newTraining == null) :
		for key in Definitions.attributeDictionary.keys() :
			$Con.get_node(Definitions.attributeDictionary[key]).setGrowthMultiplier(0)
	else :
		for key in Definitions.attributeDictionary.keys() :
			$Con.get_node(Definitions.attributeDictionary[key]).setGrowthMultiplier(newTraining.getScaling(key))
			
func getLevel(type : Definitions.attributeEnum) :
	return $Con.get_node(Definitions.attributeDictionary[type]).getLevel()
func setLevel(type : Definitions.attributeEnum, val : int) :
	$Con.get_node(Definitions.attributeDictionary[type]).setLevel(val)
func getProgress(type : Definitions.attributeEnum) :
	return $Con.get_node(Definitions.attributeDictionary[type]).getProgress()
func setProgress(type : Definitions.attributeEnum, val) :
	$Con.get_node(Definitions.attributeDictionary[type]).setProgress(val)
