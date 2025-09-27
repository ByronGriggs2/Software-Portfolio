extends Control

const binaryPopup = preload("res://Graphic Elements/Popups/binary_decision.tscn")

var tutorialsEnabled : bool = true

func _ready() :
	for child in $TabContainer.get_children() :
		var newButton : Button = Button.new()
		$TabButtonContainer.add_child(newButton)
		newButton.text = child.name
		newButton.name = child.name
		var c = Callable(self, "_on_tab_pressed").bind(newButton)
		newButton.pressed.connect(c)
	await get_tree().process_frame
	var maxSize = 180
	for child : Button in $TabButtonContainer.get_children() :
		if (child.size.x > maxSize) :
			maxSize = child.size.x
	for child : Button in $TabButtonContainer.get_children() :
		child.custom_minimum_size.x = maxSize
		$TabContainer/Combat.addPlayerReference($PlayerStatDisplay/Player)
	####
	var tutorialDialog = binaryPopup.instantiate()
	add_child(tutorialDialog)
	tutorialDialog.setTitle("Enable Tutorials?")
	tutorialDialog.setText("Would you like to enable tutorial popups? (Recommended)")
	tutorialDialog.connect("binaryChosen", _on_tutorial_dialog_choice)
	
func _on_tutorial_dialog_choice(choice : int) :
	if (choice == 0) :
		tutorialsEnabled = true
	elif (choice == 1) :
		tutorialsEnabled = false

func _process(_delta) :
	updatePlayerStats()
	
func updatePlayerStats() :
	var HP = calculateMAXHP()
	$PlayerStatDisplay.setStat("MAXHP",HP)
	$PlayerStatDisplay.setStat("HP",HP)
	$PlayerStatDisplay.setStat("PHYSATK",calculatePHYSATK())
	$PlayerStatDisplay.setStat("PHYSDEF",calculatePHYSDEF())
	$PlayerStatDisplay.setStat("MAGATK",calculateMAGATK())
	$PlayerStatDisplay.setStat("MAGDEF",calculateMAGDEF())
	
func calculateMAXHP() -> int :
	const baseHP : int = 0
	const HPGrowth : int = 1
	return baseHP + floor(HPGrowth * getAttribute(Definitions.attributeEnum.DUR))
	
func calculatePHYSATK() -> float :
	const baseAttack : float = 0
	const strAttackGrowth : float = 1
	var strAttack = strAttackGrowth * getAttribute(Definitions.attributeEnum.STR)
	return baseAttack + strAttack

func calculatePHYSDEF() -> float :
	const baseDefense : float = 10
	const strDefenseGrowth : float = 0.05
	const durDefenseGrowth : float = 0.2
	var strDefense = strDefenseGrowth * getAttribute(Definitions.attributeEnum.STR)
	var durDefense = durDefenseGrowth * getAttribute(Definitions.attributeEnum.DUR)
	return baseDefense + strDefense + durDefense
	
func calculateMAGATK() -> float :
	const baseAttack : float = 0
	const intAttackGrowth : float = 1
	var intAttack = intAttackGrowth * getAttribute(Definitions.attributeEnum.INT)
	return baseAttack + intAttack
	
func calculateMAGDEF() -> float :
	const baseDefense : float = 10
	const intDefenseGrowth : float = 0.05
	const durDefenseGrowth : float = 0.2
	var intDefense = intDefenseGrowth * getAttribute(Definitions.attributeEnum.INT)
	var durDefense = durDefenseGrowth * getAttribute(Definitions.attributeEnum.DUR)
	return baseDefense + intDefense + durDefense
	
func getAttribute(type : Definitions.attributeEnum) -> int :
	var baseValue = $PlayerStatDisplay/Player.characterClass.getBaseAttribute(type)
	var scalingMultiplier = $PlayerStatDisplay/Player.characterClass.getAttributeScaling(type)
	var scalingAmount =  $TabContainer/Attributes.getAttributeLevel(type)
	return baseValue + scalingAmount * scalingMultiplier

func _on_tab_pressed(emitter : Button) :
	for child in $TabContainer.get_children() :
		if (child.name == emitter.name) :
			child.visible = true
		else :
			child.visible = false
			
func initialStartup(character : CharacterClass) :
	$PlayerStatDisplay/Player.characterClass = character
