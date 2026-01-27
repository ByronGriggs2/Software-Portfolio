extends Resource

class_name NumberClass
@export var referenceMode : bool = false
var prebonuses : Dictionary = {}
var postbonuses : Dictionary = {}
var premultipliers : Dictionary = {}
var postmultipliers : Dictionary = {}

var trimmedPrebonuses : Dictionary = {}
var trimmedPostbonuses : Dictionary = {}
var trimmedPremultipliers : Dictionary = {}
var trimmedPostmultipliers : Dictionary = {}

func getFinal() -> float :
	var prebon : Array = prebonuses.values()
	var postbon : Array = postbonuses.values()
	var premul : Array = premultipliers.values()
	var postmul : Array = postmultipliers.values()
	var base : float = Helpers.calculateBase(prebon, premul)
	return Helpers.calculateFinal(base, postbon, postmul)
func getPrebonuses() -> Dictionary:
	return trimDict(prebonuses, false)
func getPostbonuses() -> Dictionary:
	return trimDict(postbonuses, false)
func getPremultipliers() -> Dictionary:
	return trimDict(premultipliers, true)
func getPostmultipliers() -> Dictionary:
	return trimDict(postmultipliers, false)
func getPrebonusesReference() -> Dictionary :
	return trimmedPrebonuses
func getPostbonusesReference() -> Dictionary :
	return trimmedPostbonuses
func getPremultipliersReference() -> Dictionary :
	return trimmedPremultipliers
func getPostmultipliersReference() -> Dictionary : 
	return trimmedPostmultipliers
func getPrebonusesRaw() :
	return prebonuses.duplicate()
func getPostbonusesRaw() :
	return postbonuses.duplicate()
func getPremultipliersRaw() :
	return premultipliers.duplicate()
func getPostmultipliersRaw() :
	return postmultipliers.duplicate()
	
	
func setPrebonus(key, val : float) :
	prebonuses[key] = val
	if (referenceMode) :
		if (!is_equal_approx(val,0)) :
			trimmedPrebonuses[key] = val
		elif (trimmedPrebonuses.get(key) != null) :
			trimmedPrebonuses.erase(key)
	removeZeroEntry(key)
func setPostbonus(key, val : float) :
	postbonuses[key] = val
	if (referenceMode) :
		if (!is_equal_approx(val,0)) :
			trimmedPostbonuses[key] = val
		elif (trimmedPrebonuses.get(key) != null) :
			trimmedPostbonuses.erase(key)
	removeZeroEntry(key)
func setPremultiplier(key, val : float) :
	premultipliers[key] = val
	if (referenceMode) :
		if (!is_equal_approx(val,1)) :
			trimmedPremultipliers[key] = val
		elif (trimmedPremultipliers.get(key) != null) :
			trimmedPremultipliers.erase(key)
	removeZeroEntry(key)
func setPostmultiplier(key, val : float) :
	postmultipliers[key] = val
	if (referenceMode) :
		if (!is_equal_approx(val,0)) :
			trimmedPostmultipliers[key] = val
		elif (trimmedPostmultipliers.get(key) != null) :
			trimmedPostmultipliers.erase(key)
	removeZeroEntry(key)
	
func enableReferenceMode() :
	referenceMode = true
	
func removeZeroEntry(key : String) :
	var prebonus = prebonuses.get(key)
	var postbonus = postbonuses.get(key)
	var premultiplier = premultipliers.get(key)
	var postmultiplier = postmultipliers.get(key)
	if ((prebonus == null || prebonus == 0)&&(postbonus==null||postbonus==0)&&(premultiplier==null||premultiplier==1)&&(postmultiplier==null||postmultiplier==0)) :
		prebonuses.erase(key)
		postbonuses.erase(key)
		premultipliers.erase(key)
		postmultipliers.erase(key)
	
func trimDict(dict : Dictionary, trim1 : bool) -> Dictionary :
	var temp = dict.duplicate()
	for key in temp.keys() :
		if (temp[key] == 0 && !trim1) :
			temp.erase(key)
		elif (temp[key] == 1 && trim1) :
			temp.erase(key)
	return temp 
