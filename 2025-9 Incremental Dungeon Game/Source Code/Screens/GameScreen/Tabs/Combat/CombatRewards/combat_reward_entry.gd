extends VBoxContainer

@export var myItemScene : Node = null
var myText : RichTextLabel = null

#const theme1 = preload("res://Graphic Elements/Themes/mainTab.tres")
const theme2 = preload("res://Graphic Elements/Themes/subTab.tres")

func initialise(equipmentName : String) :
	var newItem = SceneLoader.createEquipmentScene(equipmentName)
	add_child(newItem)
	myItemScene = newItem
	newItem.name = "equip"
	var newText : RichTextLabel = $SampleTextLabel.duplicate()
	newItem.addToContainer(newText)
	myText = newText
	newText.visible = true
	if (newItem.getType() == Definitions.equipmentTypeEnum.currency) :
		setCount(0)
	else :
		newText.text = newItem.getTitle()
	newItem.setTheme(theme2)
	newItem.setContentMargin(15)
	newItem.setSeparation(30)
	newItem.setContainerExpandHorizontal()
	newItem.use48x48()
	newItem.connect("wasSelected", _on_button_selected)
	
func getItemSceneRef() :
	return myItemScene

signal wasSelected
func _on_button_selected(itemSceneRef) :
	emit_signal("wasSelected", itemSceneRef)
func deselect() :
	$equip.deselect()
func setCount(val) :
	myItemScene.setCount(val)
	myText.text = myItemScene.getTitle() + " (" + str(val) + ")"
	
func getCount() :
	return myItemScene.getCount()
