extends PanelContainer

func getVolumeRef() -> Node :
	return $VBoxContainer/HBoxContainer/HSlider
	
func getTextRef() -> Node :
	return $VBoxContainer/Percent

func loadVolume() :
	getVolumeRef().value = convertDB(AudioHandler.getMasterVolume())
	
func convertDB(val) :
	var linear = db_to_linear(val)
	var proportion = linear/(1-0.001)
	return round(proportion*100)
	
var oldValue = 0
func _process(_delta) :
	var value = getValueDB()
	if (value != oldValue) :
		if (value == -999) :
			AudioHandler.setMasterMute(true)
		else :
			AudioHandler.setMasterMute(false)
			AudioHandler.setMasterVolume(value)
		getTextRef().text = str(int(getVolumeRef().value)) + "%"
		oldValue = value
	
func getValueDB() :
	var value = round(getVolumeRef().value)
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
	var masterVolume = AudioHandler.getMasterVolume()
	var masterMute = AudioHandler.getMasterMute()
	if (masterMute) :
		getVolumeRef().value = 0
	else :
		getVolumeRef().value = convertDB(masterVolume)
	getTextRef().text = str(int(getVolumeRef().value)) + "%"
