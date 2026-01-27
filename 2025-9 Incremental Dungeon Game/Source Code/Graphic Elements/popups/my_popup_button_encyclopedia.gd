extends "res://Graphic Elements/popups/my_popup_button.gd"

func _ready() : 
	getEncyclopediaLabel().useLightMode()
	getEncyclopediaLabel().currentLayer = self.layer

func setKey(val : String) :
	setTitle(val)
	getEncyclopediaLabel().setTextExceptKey(Encyclopedia.descriptions[val], val)
	
func getEncyclopediaLabel() :
	return $Panel/CenterContainer/Window/VBoxContainer/EncyclopediaTextLabel

func nestedPopupInit(parent : Node) :
	super(parent)
	getEncyclopediaLabel().currentLayer = self.layer
