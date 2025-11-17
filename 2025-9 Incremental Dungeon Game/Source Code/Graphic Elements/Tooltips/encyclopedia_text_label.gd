extends RichTextLabel

var updateRunning : bool = false
var coroutineSemaphore : int
signal coroutinesDone
signal doneRunning

const tooltipLoader = preload("res://Graphic Elements/Tooltips/tooltip_trigger.tscn")
const str1 = "[color=#baa1e6]"
const str2 = "[/color]"
var badKey : String = ""
func _ready() :
	#add_theme_font_size_override("bold_font_size", get_theme_font_size("normal_font_size"))
	if (text != "Sample text [color=#003e85][b]hyperlink text[/b][/color] sample text") :
		updateHyperlinksExceptBadKey()
	
func setTextExceptKey(val, key) :
	text = val
	badKey = key
	updateHyperlinksExceptBadKey()
	
func setText(val) :
	text = val
	updateHyperlinksExceptBadKey()
	
func updateHyperlinksExceptBadKey() :
	if (self.updateRunning) :
		await self.doneRunning
	updateRunning = true
	for child in get_children() :
		child.queue_free()
	await get_tree().process_frame
	##Search through the text to bold/color change keywords
	for key in Encyclopedia.descriptions.keys() :
		if (isBadKey(key)) :
			continue
		var currentIndex = 0
		var foundIndex = text.find(key, currentIndex)
		while (foundIndex != -1) :
			addHyperlinkAtPos(foundIndex, key)
			currentIndex = foundIndex + str1.length() + str2.length() + key.length()
			foundIndex = text.find(key, currentIndex)
	fixLinebreaks()
	##Search through the text again to add tooltips
	for key in Encyclopedia.descriptions.keys() :
		if (isBadKey(key)) :
			continue
		var currentIndex = 0
		var foundIndex = get_parsed_text().find(key, currentIndex)
		while (foundIndex != -1) :
			addTooltipAtPos(foundIndex, key)
			currentIndex = foundIndex + key.length()
			foundIndex = get_parsed_text().find(key, currentIndex)
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
	if (text.find(str1, index-str1.length()) == index-str1.length()) :
		return
	var extendedKey = getExtendedKey(index, key,false)
	text = text.insert(index, str1)
	text = text.insert(index + str1.length() + extendedKey.length(), str2)
	
func getExtendedKey(index : int, key : String, parsed : bool) -> String :
	var myText
	if (parsed) :
		myText = get_parsed_text()
	else :
		myText = text
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
	const delim = [" ", "\n", ".", ",", "[", ":"]
	var currentDist = 999
	for character in delim :
		var dist = paramText.find(character, index) - index
		if (dist != -1 - index && dist < currentDist) :
			currentDist = dist
	if (currentDist == 999) :
		return -1
	return currentDist
			
func addTooltipAtPos(index, key) :
	coroutineSemaphore += 1
	var newTooltip = tooltipLoader.instantiate()
	add_child(newTooltip)
	newTooltip.initialise(key)
	var lineNumber = get_character_line(index)
	var temp = get_line_range(lineNumber)
	var firstCharInLine = temp.x
	var lastCharInLine = temp.y
	var textArray : Array[String] = []
	var extendedKey : String = getExtendedKey(index, key,true)

	textArray.append(extendedKey)##width of key
	textArray.append(get_parsed_text().substr(firstCharInLine, index - firstCharInLine))## width of line up to key
	textArray.append(get_parsed_text().substr(firstCharInLine, lastCharInLine-firstCharInLine))## width of line
	var textArrayWidth = await Helpers.getTextWidthWaitFrameArray(get_theme_font_size("normal_font_size"), textArray)
	var lineHeight = get_theme_font("normal_font").get_height(get_theme_font_size("normal_font_size"))
		
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
	updateHyperlinksExceptBadKey()

func isOnNestedTooltip() -> bool :
	if (get_child_count() == 0) :
		return false
	for child in get_children() :
		if (child.isOnNestedTooltip()) :
			return true
	return false

func fixLinebreaks() :
	##Position of key in parsed text
	var parseIndex = 0
	##Position of key in text
	var foundIndex = text.find(str1)
	while(foundIndex != -1) :
		var parsedText = get_parsed_text()
		var keyLength = text.find(str2, foundIndex+str1.length())-(foundIndex+str1.length())
		var key = text.substr(foundIndex+str1.length(),keyLength)

		parseIndex = parsedText.find(key, parseIndex)
		if (get_character_line(parseIndex) != get_character_line(parseIndex + keyLength-1)) :
			text = text.insert(foundIndex, "\n")
		foundIndex = text.find(str1, foundIndex + str1.length() + keyLength + str2.length()+1)
		
