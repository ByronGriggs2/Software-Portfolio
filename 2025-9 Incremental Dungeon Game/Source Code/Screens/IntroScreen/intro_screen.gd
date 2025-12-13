extends Control

signal characterDone
	
func _on_reason_carousel_end(chosenOption) :
	$ReasonCarousel.visible = false
	$CharacterCreator.visible = true
	var index = 0
	for tempInt in range (options.size()) :
		if (options[tempInt] == chosenOption) :
			index = tempInt
	$CharacterCreator.initialise(options, details, index)
	currentState = state.Character
	
func _on_character_creator_end(character, characterName) :
	emit_signal("characterDone", character, characterName)

var options : Array[String] = [
	"I want to protect my village",
	"I want to prove I'm the strongest",
	"I want to study the dungeon",
	"I am on a diplomatic mission",
	"I want to get rich",
	"I want revenge"
]
var details : Array[String] = [
	"We may be at peace now, but we're in no shape for future conflict. I will master the art of combat and return home armed with the equipment and knowledge I need to protect my community.",
	"The dungeon has a myriad of terrible monsters to test my mettle against. I will only return once I have proven myself!",
	"The dungeon is flooded with ancient magic and secrets which hold inconceivable power. I will be the one to decipher its mysteries",
	"The demon race is like any humanoid race we cooperate with. Elves, dwarves, demons, it makes no difference. I will discard the prejudices of my people and work towards a brighter future for both civilisations.",
	"The dungeon is filled with magical artifacts and precious gems, even one of which worth a years' salary. I won't stop until I have a lot more than one.",
	"A demon killed my father! I will not rest until I've found the one responsible and brought him to justice!"
]
func _on_continue_signal(emitter) -> void:
	emitter.visible = false
	var children = $NarrativeContainer.get_children()
	for index in range(0,children.size()) :
		if (children[index] == emitter) :
			if (index == children.size()-1) :
				$ReasonCarousel.initialise(options, details)
				$ReasonCarousel.visible = true
				currentState = state.Reason
			else :
				children[index+1].visible = true
				currentState = (index+1) as state

enum state {P1,P2,P3,P4,P5,Reason,Character}
var currentState = state.P1
signal cancel
func _on_back_button_pressed() -> void:
	if (currentState == state.P1) :
		emit_signal("cancel")
	elif (currentState != state.Reason && currentState != state.Character) :
		narrativeGoBack()
	elif (currentState == state.Reason) :
		currentState = state.P5
		$NarrativeContainer.get_child(state.P5).visible = true
		$ReasonCarousel.visible = false
	elif (currentState == state.Character) :
		currentState = state.Reason
		$ReasonCarousel.visible = true
		$CharacterCreator.visible = false
		
func narrativeGoBack() :
	currentState = (currentState as int - 1) as state
	$NarrativeContainer.get_child(currentState).visible = true
	$NarrativeContainer.get_child(currentState + 1).visible = false
