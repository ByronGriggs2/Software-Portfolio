extends VBoxContainer

@export var myItemScene : Node = null
var myText : RichTextLabel = null

#const theme1 = preload("res://Graphic Elements/Themes/mainTab.tres")
const theme2 = preload("res://Graphic Elements/Themes/subTab.tres")

func initialise_internal(item : Equipment) :
	add_child(myItemScene)
	myItemScene.core = item
	myItemScene.name = "equip"
	var newText : RichTextLabel = $SampleTextLabel.duplicate()
	myItemScene.addToContainer(newText)
	myText = newText
	newText.visible = true
	if (myItemScene.getType() == Definitions.equipmentTypeEnum.currency) :
		setCount(0)
	else :
		newText.text = myItemScene.getTitle()
	myItemScene.setTheme(theme2)
	myItemScene.setContentMargin(15)
	myItemScene.setSeparation(30)
	myItemScene.setContainerExpandHorizontal()
	myItemScene.use48x48()
	myItemScene.connect("wasSelected", _on_button_selected)
	
func initialise(item : Equipment) :
	myItemScene = SceneLoader.createEquipmentScene(item.getItemName())
	initialise_internal(item)
	
const currencyNames = ["gold_coin", "ore", "soul"]
func initialise_currency(index, val) :
	myItemScene = SceneLoader.createEquipmentScene(currencyNames[index])
	initialise_internal(EquipmentDatabase.getEquipment(currencyNames[index]))
	setCount(val)
	
func getItemSceneRef() :
	return myItemScene

signal wasSelected
func _on_button_selected(itemSceneRef) :
	emit_signal("wasSelected", itemSceneRef)
func deselect() :
	myItemScene.deselect()
func setCount(val) :
	myItemScene.setCount(val)
	myText.text = myItemScene.getTitle() + " (" + str(val) + ")"#
func select():
	myItemScene.select()
func isSelected() :
	return myItemScene.isSelected()
	
func getCount() :
	return myItemScene.getCount()
	
func getText() -> String :
	return myItemScene.getName()
