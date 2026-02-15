extends PanelContainer

func getVolumeRef(type : AudioHandler.bus) -> Node :
	if (type == AudioHandler.bus.master) :
		return $VBoxContainer/Master/HSlider
	elif (type == AudioHandler.bus.music) :
		return $VBoxContainer/Music/HSlider
	if (type == AudioHandler.bus.sfx) :
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

func loadVolume() :
	for key in AudioHandler.busDict.keys() :
		getVolumeRef(key).value = convertDB(AudioHandler.getVolume(key))

func convertDB(val) :
	var linear = db_to_linear(val)
	var proportion = linear/(1-0.001)
	return round(proportion*100)
	
var oldValue = [0,0,0]
func _process(_delta) :
	for key in AudioHandler.busDict.keys() :
		var value = getValueDB(key)
		if (value != oldValue[key]) :
			AudioHandler.setVolume(key, value)
			getTextRef(key).text = str(int(getVolumeRef(key).value)) + "%"
			oldValue[key] = value
	
func getValueDB(type : AudioHandler.bus) :
	var value = round(getVolumeRef(type).value)
	if (value == 0) :
		return -999
	var linear = ((value)/100.0)*(1-0.001)+0.001
	return linear_to_db(linear)
	
func onSave() :
	await AudioHandler.saveMainOptions()
func onExit() :
	AudioHandler.loadMainOptions()
	
func _ready() :
	if (!AudioHandler.myReady) :
		await AudioHandler.myReadySignal
	for key in AudioHandler.busDict.keys() :
		var volume = AudioHandler.getVolume(key)
		if (is_equal_approx(volume, -999)) :
			getVolumeRef(key).value = 0
		else :
			getVolumeRef(key).value = convertDB(volume)
		getTextRef(key).text = str(int(getVolumeRef(key).value)) + "%"
