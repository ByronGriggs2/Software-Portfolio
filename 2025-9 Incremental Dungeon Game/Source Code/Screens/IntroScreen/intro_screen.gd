extends Control

signal characterDone

func _ready() :
	for child in get_children() :
		child.visible = false
	$IntroText.visible = true
	$IntroText.connect("continueSignal", _on_intro_text_end)
	
func _on_intro_text_end() :
	$IntroText.visible = false
	$ReasonCarousel.visible = true
	$ReasonCarousel.initialise(options, details)
	$ReasonCarousel.connect("continueSignal", _on_reason_carousel_end)
	
func _on_reason_carousel_end(chosenOption) :
	$ReasonCarousel.visible = false
	$CharacterCreator.visible = true
	var index = 0
	for tempInt in range (options.size()) :
		if (options[tempInt] == chosenOption) :
			index = tempInt
	$CharacterCreator.initialise(options, details, index)
	$CharacterCreator.connect("characterDone", _on_character_creator_end)
	
func _on_character_creator_end(character, characterName) :
	emit_signal("characterDone", character, characterName)

var options : Array[String] = [
	"I want to slay the demon king",
	"I want to prove I'm the strongest",
	"I want to study the dungeon",
	"I am on a diplomatic mission",
	"I want to get rich",
	"I want revenge"
]
var details : Array[String] = [
	"The demon king is a scourge of humanity! For the prosperity of mankind he must fall.",
	"The dungeon has a myriad of terrible monsters to test my mettle against. I will only return once I have proven myself!",
	"The dungeon is flooded with ancient magic and secrets which hold inconceivable power. I will be the one to decipher its mysteries",
	"The demon race is like any humanoid race we cooperate with. Elves, dwarves, demons, it makes no difference. I will discard the prejudices of my people and work towards a brighter future for both civilisations.",
	"The dungeon is filled with magical artifacts and precious gems, even one of which worth a years' salary. I won't stop until I have a lot more than one.",
	"A demon killed my father! I will not rest until I've found the one responsible and brought him to justice!"
]
