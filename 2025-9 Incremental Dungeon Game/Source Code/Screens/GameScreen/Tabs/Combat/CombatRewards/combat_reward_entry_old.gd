extends "res://Graphic Elements/Buttons/super_button.gd"

var myReward : Equipment = null
const iconPath : String = "res://Screens/GameScreen/Tabs/EquipmentTab/EquipmentIcons/"

func setReward(reward : Equipment) :
	myReward = reward
	$HBoxContainer/ItemName.text = reward.getTitle()
	var itemName = reward.resource_path.get_file().get_basename()
	$HBoxContainer/Icon/Panel/TextureRect.texture = load(iconPath + itemName + ".png")
	
func getRewardRef() :
	return myReward
