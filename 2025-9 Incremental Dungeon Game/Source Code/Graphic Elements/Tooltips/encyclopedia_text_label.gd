extends RichTextLabel

@export var wrappingEnabled : bool = true

var updateRunning : bool = false
var coroutineSemaphore : int = 0
signal coroutinesDone
signal doneRunning

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
var colourString = str1
const str1 = "[color=#baa1e6]"
const str1_b = "[color=#000000]"
const str1_d = "[color=#8A50A1]"
const str2 = "[/color]"
var badKey : String = ""

func getText() :
	return get_parsed_text()

func makeAllTextBlack() :
	text = text.replace(colourString, str1_b)
	colourString = str1_b
	updateHyperlinksExceptBadKey()
	add_theme_color_override("default_color", Color(0,0,0))
	
func useLightMode() :
	text = text.replace(colourString, str1_d)
	text = text.replace("[color=green]", "[color=#006d00]")
	colourString = str1_d
	updateHyperlinksExceptBadKey()
	add_theme_color_override("default_color", Color(0,0,0))

func _ready() :
	if (!wrappingEnabled) :
		disableWrapping()
	#add_theme_font_size_override("bold_font_size", get_theme_font_size("normal_font_size"))
	if (text != "Sample text [color=#003e85][b]hyperlink text[/b][/color] sample text") :
		updateHyperlinksExceptBadKey()
	
func disableWrapping() :
	wrappingEnabled = false
	autowrap_mode = TextServer.AUTOWRAP_OFF
	
func setTextExceptKey(val, key) :
	if (isOnNestedTooltip()) :
		return
	if (get_black_text() == val.replace("`","")) :
		var index = text.find(key)
		if (index == -1) :
			return
		var nextBBCode = text.find("[/", index+key.length())
		if (nextBBCode == -1 || nextBBCode > index+key.length()) :
			return
	text = val
	badKey = key
	await updateHyperlinksExceptBadKey()
	
func setText(val : String) :
	if (isOnNestedTooltip()) :
		return
	if (get_black_text() == val.replace("`","")) :
		return
	#print("TEXT:" + text + ": ", text.to_utf8_buffer())
	#print("VAL:" + val + ": ", val.to_utf8_buffer())
	text = val
	await updateHyperlinksExceptBadKey()
	
func get_black_text() :
	var retVal = text
	retVal = retVal.replace(str1, "")
	retVal = retVal.replace(str1_b, "")
	retVal = retVal.replace(str1_d, "")
	retVal = retVal.replace(str2, "")
	return retVal
	
func updateHyperlinksExceptBadKey() :
	if (isOnNestedTooltip()) :
		return
	if (self.updateRunning) :
		await self.doneRunning
	updateRunning = true
	for child in get_children() :
		child.queue_free()
	await get_tree().process_frame
	#text = get_black_text()
	##Search through the text to bold/color change keywords
	for key in Encyclopedia.descriptions.keys() :
		if (isBadKey(key)) :
			continue
		var currentIndex = 0
		var foundIndex = text.find(key, currentIndex)
		while (foundIndex != -1) :
			## Yield to longer keywords
			if (Encyclopedia.problemDictionary.get(key) != null) :
				var outerContinue : bool = false
				for otherKey in Encyclopedia.problemDictionary[key] :
					var nestedPos = otherKey.find(key)
					var otherIndex = foundIndex - nestedPos
					if (otherIndex != -1 && text.find(otherKey, otherIndex) == otherIndex) :
						currentIndex = otherIndex + otherKey.length()
						foundIndex = text.find(key, currentIndex)
						outerContinue = true
						break
				if (outerContinue) :
					continue
			addHyperlinkAtPos(foundIndex, key)
			currentIndex = foundIndex + colourString.length() + str2.length() + key.length()
			foundIndex = text.find(key, currentIndex)
	fixLinebreaks()
	##Search through the text again to add tooltips
	for key in Encyclopedia.descriptions.keys() :
		if (isBadKey(key)) :
			continue
		var currentIndex = 0
		var foundIndex = get_parsed_text().find(key, currentIndex)
		while (foundIndex != -1) :
			## Yield to longer keywords
			if (Encyclopedia.problemDictionary.get(key) != null) :
				var outerContinue : bool = false
				for otherKey in Encyclopedia.problemDictionary[key] :
					var otherIndex = foundIndex + key.length() - otherKey.length()
					if (otherIndex != -1 && get_parsed_text().find(otherKey, otherIndex) == otherIndex) :
						currentIndex = otherIndex + otherKey.length()
						foundIndex = get_parsed_text().find(key, currentIndex)
						outerContinue = true
						break
				if (outerContinue) :
					continue
			addTooltipAtPos(foundIndex, key)
			currentIndex = foundIndex + key.length()
			foundIndex = get_parsed_text().find(key, currentIndex)
	text = text.replace("`","")
	if (coroutineSemaphore != 0) :
		await coroutinesDone
	updateRunning = false
	emit_signal("doneRunning")
	
func isBadKey(key) -> bool :
	if (key == badKey) :
		return true
	var alternates = Encyclopedia.keyword_alternates.get(badKey)
	if (alternates == null) :
		alternates = Encyclopedia.keyword_alternates.get(key)
	if (alternates == null) :
		return false
	if (alternates is Array) :
		for item in alternates :
			if (item == key || item == badKey) :
				return true
	else :
		if (alternates == key || alternates == badKey) :
			return true
	return false
			
func addHyperlinkAtPos(index, key) :
	if (index >= colourString.length() && text.find(colourString, index-colourString.length()) == index-colourString.length()) :
		return
	var extendedKey = getExtendedKey(index, key,false)
	text = text.insert(index, colourString)
	text = text.insert(index + colourString.length() + extendedKey.length(), str2)
	
func getExtendedKey(index : int, key : String, parsed : bool) -> String :
	var myText : String
	if (parsed) :
		myText = get_parsed_text()
	else :
		myText = text
	if (myText.find("`", index+key.length()) == index+key.length()) :
		return key
	var distanceToDelimRes = distanceToDelim(myText, index+key.length())
	var distanceToEnd = (myText.length()) - (index + key.length()-1)
	var delimExists = distanceToDelimRes != -1
	
	var extensionAmount
	if (delimExists) :
		extensionAmount = distanceToDelimRes
	else :
		extensionAmount = distanceToEnd
	return myText.substr(index, key.length() + extensionAmount)
	
func distanceToDelim(paramText : String, index : int) -> int :
	const delim = [" ", "\n", ".", ",", "[", "]", ":", "\"", "(", ")"]
	var currentDist = 999
	for character in delim :
		var dist = paramText.find(character, index) - index
		if (dist != -1 - index && dist < currentDist) :
			currentDist = dist
	if (currentDist == 999) :
		return -1
	return currentDist
			
var currentLayer = 0
func addTooltipAtPos(index, key) :
	coroutineSemaphore += 1
	var newTooltip = tooltipLoader.instantiate()
	add_child(newTooltip)
	newTooltip.initialise(key)
	newTooltip.currentLayer = currentLayer
	var lineNumber = get_character_line(index)
	if (lineNumber == -1) :
		coroutineSemaphore -= 1
		if (coroutineSemaphore == 0) :
			emit_signal("coroutinesDone")
		return
	var temp = get_line_range(lineNumber)
	var firstCharInLine = temp.x
	var lastCharInLine = temp.y
	var textArray : Array[String] = []
	var extendedKey : String = getExtendedKey(index, key,true)

	textArray.append(extendedKey)##width of key
	textArray.append(get_parsed_text().substr(firstCharInLine, index - firstCharInLine))## width of line up to key
	textArray.append(get_parsed_text().substr(firstCharInLine, lastCharInLine-firstCharInLine))## width of line
	var textArrayWidth = await Helpers.getTextWidthWaitFrameArray(get_theme_font_size("normal_font_size"), textArray)
	if (newTooltip == null) :
		coroutineSemaphore -= 1
		if (coroutineSemaphore == 0) :
			emit_signal("coroutinesDone")
		return
	var fontSize = get_theme_font_size("normal_font_size")
	var lineHeight = get_theme_font("normal_font").get_height(fontSize)
		
	var yCoord1 = lineNumber * lineHeight
	var xCoord1
	if (get_horizontal_alignment() == HORIZONTAL_ALIGNMENT_CENTER) :
		xCoord1 = (0.5 * size.x) - 0.5 * textArrayWidth[2] + textArrayWidth[1]
	else :
		xCoord1 = textArrayWidth[1]
	var yCoord2 = yCoord1 + lineHeight
	var xCoord2 = xCoord1 + textArrayWidth[0]
	newTooltip.setPos(Vector2(xCoord1, yCoord1), Vector2(xCoord2, yCoord2))
	coroutineSemaphore -= 1
	if (coroutineSemaphore == 0) :
		emit_signal("coroutinesDone")

func _on_minimum_size_changed() -> void:
	if (!self.updateRunning):
		updateHyperlinksExceptBadKey()

func isOnNestedTooltip() -> bool :
	if (get_child_count() == 0) :
		return false
	for child in get_children() :
		if (child.isOnNestedTooltip()) :
			return true
	return false

func fixLinebreaks() :
	if (!wrappingEnabled) :
		return
	##Position of key in parsed text
	var parseIndex = 0
	##Position of key in text
	var foundIndex = text.find(colourString)
	while(foundIndex != -1) :
		var parsedText = get_parsed_text()
		var keyLength = text.find(str2, foundIndex+colourString.length())-(foundIndex+colourString.length())
		var key = text.substr(foundIndex+colourString.length(),keyLength)

		parseIndex = parsedText.find(key, parseIndex)
		if (get_character_line(parseIndex) != get_character_line(parseIndex + keyLength-1)) :
			text = text.insert(foundIndex, "\n")
		foundIndex = text.find(colourString, foundIndex + colourString.length() + keyLength + str2.length()+1)
		
