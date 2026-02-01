extends RichTextLabel

var myText : String
func setText(val) :
	myText = val
	text = " " + myText + " "
	
func getText() :
	return myText
