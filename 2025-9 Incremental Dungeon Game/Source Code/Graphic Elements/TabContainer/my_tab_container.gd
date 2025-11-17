extends Panel

var myReady : bool = false
signal actuallyReady
func _ready() :
	await hardRefreshTabs()
	setTab($InnerContainer.get_child(0))
	myReady = true
	emit_signal("actuallyReady")

const tabButtonLoader = preload("res://Graphic Elements/Buttons/super_button.tscn")
@export var panelSeparation : float :
	set (val) :
		panelSeparation = val
		$InnerContainer.queue_sort()

var currentTabIndex : int = 0

func _on_inner_container_pre_sort_children() -> void:
	var hardRefresh : bool = false
	var children = $InnerContainer.get_children()
	if (children.is_empty()) :
		return
	var buttonList = $TabButtons/HBoxContainer.get_children()
	if (buttonList.size() != children.size()) :
		hardRefresh = true
	else :
		for index in range(0,children.size()) :
			if (buttonList[index].get_node("HBoxContainer").get_child(0).getText() != children[index].name) :
				hardRefresh = true
	if (hardRefresh) :
		hardRefreshTabs()
	else :
		softRefreshTabs()

var hardRefreshLock : bool = false
signal hardRefreshUnlocked
func hardRefreshTabs() :
	while (hardRefreshLock) :
		await hardRefreshUnlocked
	hardRefreshLock = true
	var deletedButton : bool = false
	for button in $TabButtons/HBoxContainer.get_children() :
		if (findChild(button) == null) :
			deletedButton = true
			button.queue_free()
	if (deletedButton) :
		await get_tree().process_frame
		
	if ($InnerContainer == null) :
		return
		
	var children = $InnerContainer.get_children()
	if (!children.is_empty()) :
		for child in children :
			if (findButton(child) == null) :
				addButton(child)
		$TabButtons/HBoxContainer.queue_sort()
		await(get_tree().process_frame)
		softRefreshTabs()
	hardRefreshLock = false
	emit_signal("hardRefreshUnlocked")

func softRefreshTabs() :
	$InnerContainer.offset_top = panelSeparation + $TabButtons.size.y + 10
	$InnerContainer.offset_bottom = 0

func addButton(tab) :
	var newButton = tabButtonLoader.instantiate()
	$TabButtons/HBoxContainer.add_child(newButton)
	newButton.name = tab.name
	newButton.theme = theme
	newButton.setAlignment("Center")
	newButton.setSticky(true)
	newButton.connect("wasSelected", _on_button_toggled)
	var newLabel = $SampleTextLabel.duplicate()
	newButton.addToContainer(newLabel)
	newLabel.setText(tab.name)
	newLabel.visible = true
	
signal tabChanged
func _on_button_toggled(emitter : Node) :
	if ($InnerContainer == null || !emitter.isSelected()) :
		return
	var children = $InnerContainer.get_children()
	if (children.is_empty()) :
		return
	setTab(emitter.get_node("HBoxContainer").get_child(0).getText())
	emit_signal("tabChanged", currentTabIndex)

func setTab(newTab) :
	var children = $InnerContainer.get_children()
	if (children.is_empty()) :
		return
	for index in range(0,children.size()) :
		if ((newTab is Control && children[index] == newTab) || (newTab is String && children[index].name == newTab)) :
			children[index].visible = true
			$TabButtons/HBoxContainer.get_child(index).select()
			currentTabIndex = index
		else :
			children[index].visible = false
			$TabButtons/HBoxContainer.get_child(index).deselect()
			
func findButton(child : Control) :
	for button in $TabButtons/HBoxContainer.get_children() :
		if (button.get_node("HBoxContainer").get_child(0).getText() == child.name) :
			return button
	return null

func findChild(button) :
	for child in $InnerContainer.get_children() :
		if (button.get_node("HBoxContainer").get_child(0).getText() == child.name) :
			return child
	return null
			
func hideChild(child : Control) :
	if (hardRefreshLock) :
		await hardRefreshUnlocked
	findButton(child).visible = false
	
func revealChild(child : Control) :
	if (hardRefreshLock) :
		await hardRefreshUnlocked
	findButton(child).visible = true
