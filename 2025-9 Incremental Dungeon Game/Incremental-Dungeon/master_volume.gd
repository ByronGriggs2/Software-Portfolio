extends PanelContainer

func getVolumeRef(type : AudioHandler.bus) -> Node :
	if (type == AudioHandler.bus.master) :
		return $VBoxContainer/Master/HSlider
	elif (type == AudioHandler.bus.music) :
		return $VBoxContainer/Music/HSlider
	elif (type == AudioHandler.bus.sfx) :
		return $VBoxContainer/Sfx/HSlider
	else :
		return null
	
func getTextRef(type : AudioHandler.bus) -> Node :
	if (type == AudioHandler.bus.master) :
		return $VBoxContainer/Master/Percent
	elif (type == AudioHandler.bus.music) :
		return $VBoxContainer/Music/Percent
	if (type == AudioHandler.bus.sfx) :
		return $VBoxContainer/Sfx/Percent
	else :
		return null

#func loadVolume() :
	#for key in AudioHandler.busDict.keys() :
		#getVolumeRef(key).value = convertDB(AudioHandler.getVolume(key))

#func convertDB(val) :
	#var proportion = val/rangeMultiplier
	#proportion = db_to_linear(proportion)
	#proportion /= 2.0
	#return round(proportion*100)
	
var oldValue = [0,0,0]
func _process(_delta) :
	for key in AudioHandler.busDict.keys() :
		#var value = getValueDB(key)
		var value = getVolumeRef(key).value
		if (value != oldValue[key]) :
			AudioHandler.setVolume(key, value)
			getTextRef(key).text = dBToStr(value, key)
			oldValue[key] = value
			
func dBToStr(val : float, key : AudioHandler.bus) -> String :
	var min = AudioHandler.defaultVolume[key]-AudioHandler.volumeRange[key]/2.0
	var max = AudioHandler.defaultVolume[key]+AudioHandler.volumeRange[key]/2.0
	if (val <= min) :
		return "0%"
	elif (val >= max) :
		return "100%"
	else :
		var proportion = (val-min)/(AudioHandler.volumeRange[key])
		return str(int(round(100*proportion))) + "%"
	
#func getValueDB(type : AudioHandler.bus) :
	#var value = round(getVolumeRef(type).value)
	#if (value == 0) :
		#return -999
	#var linear = value/100.0
	#linear *= 2
	#return rangeMultiplier*(linear_to_db(linear))
	
func onSave() :
	await AudioHandler.saveMainOptions()
func onExit() :
	AudioHandler.loadMainOptions()
	
func _ready() :
	if (!AudioHandler.myReady) :
		await AudioHandler.myReadySignal
	for key in AudioHandler.busDict.keys() :
		getVolumeRef(key).min_value = AudioHandler.defaultVolume[key] - AudioHandler.volumeRange[key]/2.0
		getVolumeRef(key).max_value = AudioHandler.defaultVolume[key] + AudioHandler.volumeRange[key]/2.0
		var volume = AudioHandler.getVolume(key)
		if (is_equal_approx(volume, -999)) :
			getVolumeRef(key).value = getVolumeRef(key).min_value
		else :
			getVolumeRef(key).value = volume
		getTextRef(key).text = dBToStr(getVolumeRef(key).value, key)
