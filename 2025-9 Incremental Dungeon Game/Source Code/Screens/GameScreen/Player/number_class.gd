extends Resource

class_name NumberClass
var prebonuses : Dictionary = {}
var postbonuses : Dictionary = {}
var premultipliers : Dictionary = {}
var postmultipliers : Dictionary = {}

func getFinal() -> float :
	var prebon : Array[float] = []
	for key in prebonuses.keys() :
		prebon.append(prebonuses[key])
	var premul : Array[float] = []
	for key in premultipliers.keys() :
		premul.append(premultipliers[key])
	var postbon : Array[float] = []
	for key in postbonuses.keys() :
		postbon.append(postbonuses[key])
	var postmul : Array[float] = []
	for key in postmultipliers.keys() :
		postmul.append(postmultipliers[key])
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
	removeZeroEntry(key)
func setPostbonus(key, val : float) :
	postbonuses[key] = val
	removeZeroEntry(key)
func setPremultiplier(key, val : float) :
	premultipliers[key] = val
	removeZeroEntry(key)
func setPostmultiplier(key, val : float) :
	postmultipliers[key] = val
	removeZeroEntry(key)
	
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
