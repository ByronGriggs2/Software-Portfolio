extends RichTextLabel

var myNumberRef : NumberClass = null
const templateText = "[b]Prebonuses[/b]\n<PREBONUSES>\n[b]Premultipliers[/b]\n<PREMULTIPLIERS>\n[b]Postbonuses[/b]\n<POSTBONUSES>\n[b]Postmultipliers[/b]\n<POSTMULTIPLIERS>\n\n[<PREBONUSFORMULA><PREMULTIPLIERFORMULA>]<POSTMULTIPLIERFORMULA><POSTBONUSFORMULA> = "

var myText : String
func setText(val) :
	myText = val
	text = " " + val + " "
func getText() :
	return myText

func setNumberReference(val : NumberClass) :
	myNumberRef = val
	
func createNumberList(items : Dictionary) -> String :
	var retVal : String = ""
	var keys = items.keys()
	for index in range(0,keys.size()) :
		if (index != 0) :
			retVal += "\n"
		retVal += keys[index] + ": " + str(Helpers.myRound(items[keys[index]], 3))
	return retVal
	
enum number_createFormulaString_type{prebonus,postbonus,premultiplier,postmultiplier}
func createFormulaString(items : Dictionary, type : number_createFormulaString_type) :
	var retVal : String = ""
	if (!(items.is_empty())) :
		retVal += getPrefix(type, items.size())
		var myKeys = items.keys()
		for index in range(0,myKeys.size()) :
			if (index != 0) :
				retVal += getSymbol(type)
			retVal += str(Helpers.myRound(items[myKeys[index]],3))
		retVal += getAffix(type, items.size())
	return retVal

func getPrefix(type : number_createFormulaString_type, itemsSize) :
	var retVal : String = ""
	if (type == number_createFormulaString_type.prebonus) :
		pass
	elif (type == number_createFormulaString_type.postbonus) :
		retVal += " + "
	elif (type == number_createFormulaString_type.premultiplier) :
		retVal += " * "
	elif (type == number_createFormulaString_type.postmultiplier) :
		retVal += " * ([b]1[/b] + "
	else :
		return ""
	if (itemsSize > 1) :
		if (type == number_createFormulaString_type.postmultiplier) :
			pass
		elif (type == number_createFormulaString_type.prebonus || type == number_createFormulaString_type.postbonus || type == number_createFormulaString_type.premultiplier) :
			retVal += "("
		else :
			return ""
	return retVal
	
func getAffix(type : number_createFormulaString_type, itemsSize) :
	if (type == number_createFormulaString_type.prebonus || type == number_createFormulaString_type.postbonus || type == number_createFormulaString_type.premultiplier) :
		if (itemsSize > 1) :
			return ")"
		else :
			return ""
	elif (type == number_createFormulaString_type.postmultiplier) :
		return ")"
	else :
		return ""
	
func getSymbol(type : number_createFormulaString_type) :
	if (type == number_createFormulaString_type.prebonus || type == number_createFormulaString_type.postbonus || type == number_createFormulaString_type.postmultiplier) :
		return "+"
	elif (type == number_createFormulaString_type.premultiplier) :
		return "*"
	else :
		return ""
	
func _process(_delta) :
	if (myNumberRef == null) :
		return
	setText(str(Helpers.myRound(myNumberRef.getFinal(),3)))
	if (!$TooltipTrigger.isOnNestedTooltip()) :
		return
	var prebonuses = myNumberRef.getPrebonuses()
	var premultipliers = myNumberRef.getPremultipliers()
	var postbonuses = myNumberRef.getPostbonuses()
	var postmultipliers = myNumberRef.getPostmultipliers()
	
	var prebonusText = createNumberList(prebonuses)
	var premultiplierText = createNumberList(premultipliers)
	var postbonusText = createNumberList(postbonuses)
	var postmultiplierText = createNumberList(postmultipliers)
	
	var tooltipText = templateText
	tooltipText = tooltipText.replace("<PREBONUSES>", prebonusText)
	tooltipText = tooltipText.replace("<PREMULTIPLIERS>", premultiplierText)
	tooltipText = tooltipText.replace("<POSTBONUSES>", postbonusText)
	tooltipText = tooltipText.replace("<POSTMULTIPLIERS>", postmultiplierText)
	
	var prebonusFormula : String = createFormulaString(prebonuses, number_createFormulaString_type.prebonus)
	var premultiplierFormula : String = createFormulaString(premultipliers, number_createFormulaString_type.premultiplier)
	var postbonusFormula : String = createFormulaString(postbonuses, number_createFormulaString_type.postbonus)
	var postmultiplierFormula : String = createFormulaString(postmultipliers, number_createFormulaString_type.postmultiplier)
	
	tooltipText = tooltipText.replace("<PREBONUSFORMULA>", prebonusFormula)
	tooltipText = tooltipText.replace("<PREMULTIPLIERFORMULA>", premultiplierFormula)
	tooltipText = tooltipText.replace("<POSTBONUSFORMULA>", postbonusFormula)
	tooltipText = tooltipText.replace("<POSTMULTIPLIERFORMULA>", postmultiplierFormula)
	## This is the final value
	tooltipText += getText()
	$TooltipTrigger.setDesc(tooltipText)
