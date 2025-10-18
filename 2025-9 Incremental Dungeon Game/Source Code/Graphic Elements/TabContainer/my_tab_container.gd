@tool
extends Control

func _ready() :
	if (!Engine.is_editor_hint()):
		await get_tree().process_frame
		setTab($InnerContainer.get_child(0))

const tabButtonLoader = preload("res://Graphic Elements/TabContainer/tab_button.tscn")
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
			if (buttonList[index].text != children[index].name) :
				hardRefresh = true
	if (hardRefresh) :
		hardRefreshTabs()
	else :
		softRefreshTabs()

func hardRefreshTabs() :
	if ($InnerContainer == null) :
		return
	for tabButton in $TabButtons/HBoxContainer.get_children() :
		tabButton.queue_free()
	var children = $InnerContainer.get_children()
	if (!children.is_empty()) :
		for child in children :
			addButton(child)
	$TabButtons/HBoxContainer.queue_sort()
	await(get_tree().process_frame)
	softRefreshTabs()

func softRefreshTabs() :
	$InnerContainer.offset_top = panelSeparation + $TabButtons.size.y + 10
	$InnerContainer.offset_bottom = 0
	$Background.offset_top = panelSeparation + $TabButtons.size.y + 10
	$Background.offset_bottom = 0

func addButton(tab) :
	var newButton = tabButtonLoader.instantiate()
	$TabButtons/HBoxContainer.add_child(newButton)
	newButton.setText(tab.name)
	newButton.name = tab.name
	newButton.theme = theme
	newButton.connect("myToggled", _on_button_toggled)
	newButton.initialise()
	
signal tabChanged

func _on_button_toggled(toggledOn : bool, emitter : Node) :
	if ($InnerContainer == null || !toggledOn) :
		return
	var children = $InnerContainer.get_children()
	if (children.is_empty()) :
		return
	setTab(emitter.text)
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
			$TabButtons/HBoxContainer.get_child(index).detoggle()
