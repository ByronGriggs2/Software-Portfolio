extends Button

signal levelChosen
@export var hiddenName : String
@export var encounter : Encounter = null
enum myVisibilityEnum {invisible, halfVisible, fullVisible}
@export var visibilityOnStartup : myVisibilityEnum = myVisibilityEnum.invisible
var firstEntry : bool = true

var encounterOverride : Encounter = null
var overrideOnce : bool = false

func isFirstEntry() :
	return firstEntry
	
func enter() :
	firstEntry = false

func _ready() :
	visible = false
	setVisibility(visibilityOnStartup as int)

func _on_pressed() -> void:
	emit_signal("levelChosen", self)

func fullReveal() :
	visible = true
	disabled = false
	text = hiddenName
	
func halfReveal() :
	if (getVisibility() != myVisibilityEnum.fullVisible as int) :
		visible = true
		disabled = true
		text = ""

func getVisibility() :
	if (!visible) :
		return myVisibilityEnum.invisible as int
	elif (disabled) :
		return myVisibilityEnum.halfVisible as int
	elif (!disabled) :
		return myVisibilityEnum.fullVisible as int
	else :
		return 3
	
func setVisibility(val) :
	if (val as myVisibilityEnum == myVisibilityEnum.invisible) :
		visible = false
	elif (val as myVisibilityEnum == myVisibilityEnum.halfVisible) :
		halfReveal()
	elif (val as myVisibilityEnum == myVisibilityEnum.fullVisible) :
		fullReveal()

func getEncounterRef() :
	if (encounterOverride != null) :
		return encounterOverride
	return encounter

func overrideEncounter(newEncounter : Encounter, once : bool) :
	encounterOverride = newEncounter
	overrideOnce = once
		
func getOverridePath() :
	if (encounterOverride == null) :
		return "null"
	return encounterOverride.resource_path
	
func getOverrideOnce() :
	return overrideOnce
	
func onCombatComplete() :
	if (overrideOnce) :
		encounterOverride = null
		overrideOnce = false
