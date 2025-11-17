extends RichTextLabel

var myNumberRef : NumberClass = null
const templateText = "[b]Prebonuses[/b]<PREBONUSES>\n[b]Premultipliers[/b]<PREMULTIPLIERS>\n[b]Postbonuses[/b]<POSTBONUSES>\n[b]Postmultipliers[/b]<POSTMULTIPLIERS>\n\n[<PREBONUSFORMULA><PREMULTIPLIERFORMULA>]<POSTMULTIPLIERFORMULA><POSTBONUSFORMULA> = "

var myText : String
func setText(val) :
	myText = val
	text = " " + val + " "
func getText() :
	return myText

func setNumberReference(val : NumberClass) :
	myNumberRef = val
	
func _process(_delta) :
	if (myNumberRef == null) :
		return
	setText(str(myNumberRef.getFinal()))
	if (!$TooltipTrigger.isOnNestedTooltip()) :
		return
	var prebonuses = myNumberRef.getPrebonuses()
	var premultipliers = myNumberRef.getPremultipliers()
	var postbonuses = myNumberRef.getPostbonuses()
	var postmultipliers = myNumberRef.getPostmultipliers()
	var tooltipText = templateText
	
	var prebonusText : String = ""
	for key in prebonuses.keys() :
		prebonusText += "\n"
		prebonusText += key + ": " + str(prebonuses[key])
	var premultiplierText : String = ""
	for key in premultipliers.keys() :
		premultiplierText += "\n"
		premultiplierText += key + ": " + str(premultipliers[key])
	var postbonusText : String = ""
	for key in postbonuses.keys() :
		postbonusText += "\n"
		postbonusText += key + ": " + str(postbonuses[key])
	var postmultiplierText : String = ""
	for key in postmultipliers.keys() :
		postmultiplierText += "\n"
		postmultiplierText += key + ": " + str(postmultipliers[key])
	
	tooltipText = tooltipText.replace("<PREBONUSES>", prebonusText)
	tooltipText = tooltipText.replace("<PREMULTIPLIERS>", premultiplierText)
	tooltipText = tooltipText.replace("<POSTBONUSES>", postbonusText)
	tooltipText = tooltipText.replace("<POSTMULTIPLIERS>", postmultiplierText)
	
	var prebonusFormula : String = ""
	if (!(prebonuses.is_empty())) :
		if (prebonuses.size() > 1) :
			prebonusFormula += "("
		var myKeys = prebonuses.keys()
		for index in range(0,myKeys.size()) :
			if (index != 0) :
				prebonusFormula += "+"
			prebonusFormula += str(prebonuses[myKeys[index]])
		if (prebonuses.size() > 1) :
			prebonusFormula += ")"
			
	var premultiplierFormula : String = ""
	if (!(premultipliers.is_empty())) :
		premultiplierFormula += " * "
		if (premultipliers.size() > 1) :
			premultiplierFormula += "("
		var myKeys = premultipliers.keys()
		for index in range(0,myKeys.size()) :
			if (index != 0) :
				premultiplierFormula += "*"
			premultiplierFormula += str(premultipliers[myKeys[index]])
		if (premultipliers.size() > 1) :
			premultiplierFormula += ")"
			
	var postbonusFormula : String = ""
	if (!(postbonuses.is_empty())) :
		postbonusFormula += " + "
		if (postbonuses.size() > 1) :
			postbonusFormula += "("
		var myKeys = postbonuses.keys()
		for index in range(0,myKeys.size()) :
			if (index != 0) :
				postbonusFormula += "+"
			postbonusFormula += str(postbonuses[myKeys[index]])
		if (postbonuses.size() > 1) :
			postbonusFormula += ")"
	var postmultiplierFormula : String = ""
	if (!(postmultipliers.is_empty())) :
		postmultiplierFormula += " * ([b]1[/b] + "
		var myKeys = postmultipliers.keys()
		for index in range(0, myKeys.size()) :
			if (index != 0) :
				postmultiplierFormula += "+"
			postmultiplierFormula += str(postmultipliers[myKeys[index]])
		postmultiplierFormula += ")"
	
	tooltipText = tooltipText.replace("<PREBONUSFORMULA>", prebonusFormula)
	tooltipText = tooltipText.replace("<PREMULTIPLIERFORMULA>", premultiplierFormula)
	tooltipText = tooltipText.replace("<POSTBONUSFORMULA>", postbonusFormula)
	tooltipText = tooltipText.replace("<POSTMULTIPLIERFORMULA>", postmultiplierFormula)
	tooltipText += getText()
	
	$TooltipTrigger.tooltipText = tooltipText
