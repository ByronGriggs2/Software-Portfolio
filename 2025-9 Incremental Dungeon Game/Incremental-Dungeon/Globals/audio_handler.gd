extends Node

const originalTheme = preload("res://Audio/OST/theme_01.wav")
const mainMenuTheme = originalTheme

enum sfx {rangedPhysical,rangedMagic,meleeSlice}
const combatSounds = {
	sfx.rangedPhysical : [
		preload("res://Audio/SFX/Combat/bow_01.wav"),
		preload("res://Audio/SFX/Combat/bow_02.wav")
	],
	sfx.rangedMagic : [
		preload("res://Audio/SFX/Combat/magic_01.wav"),
		preload("res://Audio/SFX/Combat/magic_02.wav")
	],
	sfx.meleeSlice : [
		preload("res://Audio/SFX/Combat/sword_01.wav"),
		preload("res://Audio/SFX/Combat/sword_02.wav"),
		preload("res://Audio/SFX/Combat/sword_03.wav")
	]
}
enum menuSfx {save,select,cancel,warning}
const menuSounds = {
	menuSfx.save : preload("res://Audio/SFX/Menu/save.wav"),
	menuSfx.select : preload("res://Audio/SFX/Menu/select.wav"),
	menuSfx.cancel : preload("res://Audio/SFX/Menu/cancel.wav"),
	menuSfx.warning : preload("res://Audio/SFX/Menu/alert.wav")
}

enum bus {master,music,sfx}
const busDict = {
	bus.master : "Master",
	bus.music : "Music",
	bus.sfx : "Sfx"
}
var musicStream : AudioStreamPlayer = null
var sfxStream : AudioStreamPlayer = null
var masterBus : int = -1
var musicBus : int = -1
var sfxBus : int = -1

enum musicMode {silent,playlist,mainMenu}
var musicModeMutex = Mutex.new()
var currentMusicMode : musicMode = musicMode.silent
var nextMusicMode : musicMode = musicMode.silent

var myTimer : Timer = null
var timerPing : bool = false
var musicFinishedPing : bool = false
var myReady : bool = false
signal myReadySignal
func _ready() :
	await waitForDependencies()
	musicBus = AudioServer.get_bus_index("Music")
	masterBus = AudioServer.get_bus_index("Master")
	sfxBus = AudioServer.get_bus_index("Sfx")
	
	#setVolume(bus.music, -6.012)
	#AudioServer.set_bus_volume_db(musicBus, -6.012)
	loadMainOptions()
	
	musicStream = AudioStreamPlayer.new()
	add_child(musicStream)
	musicStream.bus = busDict[bus.music]
	musicStream.connect("finished", _on_music_finished)
	sfxStream = AudioStreamPlayer.new()
	add_child(sfxStream)
	sfxStream.bus = busDict[bus.sfx]
	sfxStream.max_polyphony = 10
	
	myTimer = Timer.new()
	add_child(myTimer)
	myTimer.connect("timeout", _on_timeout)
	makeConnections()
	_on_main_menu_loaded()
	myReady = true
	emit_signal("myReadySignal")

func _on_timeout() :
	timerPing = true
func _on_music_finished() :
	musicFinishedPing = true

func _process(_delta) :
	musicModeMutex.lock()
	if (currentMusicMode != nextMusicMode) :
		musicModeMutex.unlock()
		transitionMusicMode()
		return
	musicModeMutex.unlock
	if (currentMusicMode == musicMode.silent) :
		return
	elif (currentMusicMode == musicMode.playlist) :
		process_playlist()
	elif (currentMusicMode == musicMode.mainMenu) :
		process_mainMenu()
	timerPing = false
	musicFinishedPing = false

var firstProcess : bool = true
var transitioning : bool = false
var transitionMutex = Mutex.new()
func transitionMusicMode() :
	transitionMutex.lock()
	if (transitioning) :
		transitionMutex.unlock()
		return
	else :
		transitioning = true
		transitionMutex.lock()
	if (musicStream.stream != null) :
		await fadeoutMusic()
	currentMusicMode = nextMusicMode
	firstProcess = true
	transitionMutex.lock()
	transitioning = false
	transitionMutex.unlock()
	
func fadeoutMusic() :
	var originalVolume = musicStream.volume_db
	const fadeoutDuration = 2
	const fadeoutGranularity = 20.0
	var fadeoutTimer = Timer.new()
	add_child(fadeoutTimer)
	fadeoutTimer.wait_time = fadeoutDuration/fadeoutGranularity
	fadeoutTimer.start()
	for index in range(0,fadeoutGranularity) :
		musicStream.volume_db -= 60.0/fadeoutGranularity
		if (index < fadeoutGranularity-1) :
			await fadeoutTimer.timeout
	musicStream.stop()
	fadeoutTimer.stop()
	fadeoutTimer.queue_free()
	fadeoutTimer = null
	musicStream.stream = null
	musicStream.volume_db = originalVolume
	
var process_playlist_state = 0
func process_playlist() :
	if (firstProcess) :
		musicStream.stream = originalTheme
		playSilentMusic(60)
		firstProcess = false
		process_playlist_state = 1
	elif (process_playlist_state == 1) :
		if (timerPing) :
			myTimer.stop()
			musicStream.play()
			process_playlist_state = 2
	elif (process_playlist_state == 2) :
		if (musicFinishedPing) :
			playSilentMusic(60)
			process_playlist_state = 1

var process_mainMenu_state = 0
func process_mainMenu() :
	if (firstProcess) :
		musicStream.stream = mainMenuTheme
		musicStream.play()
		firstProcess = false
		process_mainMenu_state = 1
	elif (process_mainMenu_state == 1) :
		if (musicFinishedPing) :
			playSilentMusic(10)
			process_mainMenu_state = 2
	elif (process_mainMenu_state == 2) :
		if (timerPing) :
			myTimer.stop()
			musicStream.play()
			process_mainMenu_state = 1

func playSilentMusic(duration : int) :
	musicStream.stop()
	myTimer.wait_time = duration
	myTimer.start()

func _on_main_menu_loaded() :
	musicModeMutex.lock()
	nextMusicMode = musicMode.mainMenu
	musicModeMutex.unlock()

func _on_game_loaded() :
	musicModeMutex.lock()
	nextMusicMode = musicMode.playlist
	musicModeMutex.unlock()

func makeConnections() :
	pass
	
func getIdx(type : bus) :
	if (type == bus.master) :
		return masterBus
	elif (type == bus.music) :
		return musicBus
	elif (type == bus.sfx) :
		return sfxBus
	else :
		return -1
func setVolume(type : bus, val : float) :
	var idx = getIdx(type)
	if (idx != -1) :
		if (is_equal_approx(val, -999)) :
			AudioServer.set_bus_mute(idx, true)
		else :
			AudioServer.set_bus_mute(idx, false)
			AudioServer.set_bus_volume_db(idx, val)
func setMasterVolume(val) :
	setVolume(bus.master, val)
func setMasterMute(val) :
	setVolume(bus.master, -999)

func getVolume(type : bus) -> float :
	var idx = getIdx(type)
	if (idx == -1) :
		return -999
	if (AudioServer.is_bus_mute(idx)) :
		return -999
	return AudioServer.get_bus_volume_db(idx)
func getMasterVolume() :
	return getVolume(bus.master)
func getMasterMute() -> bool :
	return is_equal_approx(getVolume(bus.master),-999)

func loadMainOptions() :
	var allSettings = SaveManager.getGlobalSettings()
	var settings = allSettings.get("audio")
	if (settings == null) :
		return
	if (settings.get("masterVolume") != null) :
		setMasterVolume(settings["masterVolume"])
	if (settings.get("musicVolume") != null) :
		setVolume(bus.music, settings["musicVolume"])
	if (settings.get("sfxVolume") != null) :
		setVolume(bus.sfx, settings["sfxVolume"])
	
func saveMainOptions() :
	var currentSettings = SaveManager.getGlobalSettings()
	currentSettings["audio"] = getMainOptionsDictionary()
	SaveManager.queueSaveGlobalSettings_immediate(currentSettings)
	
func getMainOptionsDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["masterVolume"] = getVolume(bus.master)
	retVal["musicVolume"] = getVolume(bus.music)
	retVal["sfxVolume"] = getVolume(bus.sfx)
	return retVal
	
func getDefaultMainOptionsDictionary() -> Dictionary :
	var retVal : Dictionary = {}
	retVal["masterVolume"] = -6.012
	retVal["musicVolume"] = -6.012
	retVal["sfxVolume"] = -6.012
	return retVal
	
func waitForDependencies() :
	if (SaveManager.myReady == false) :
		await SaveManager.myReadySignal
		
## Only allows 1 sfx at a time. Ask ChatGPT how to cleanly implement multiple sfx?
#var sfxMutex = Mutex.new()
#var sfxPlaying : bool = false
var lastPlayed = null
func playSfx(type : sfx, lowerVolume : bool) :
	playSfx_internal("combat", type as int, lowerVolume)
func playMenuSfx(type : menuSfx) :
	playSfx_internal("menu", type as int, false)
	
func createSfxAudioStreamPlayer(stream : AudioStream, lowerVolume : bool) -> AudioStreamPlayer :
	var ret : AudioStreamPlayer = sfxStream.duplicate()
	ret.stream = stream
	if (lowerVolume) :
		ret.volume_db -= 6
	else :
		ret.volume_db -= 0
	return ret
func playSfx_internal(sfxType : String, myEnum : int, lowerVolume : bool) :
	var type
	var sounds
	if (sfxType == "combat") :
		type = myEnum as sfx
		sounds = combatSounds
	elif (sfxType == "menu") :
		type = myEnum as menuSfx
		sounds = menuSounds
	if (AudioServer.is_bus_mute(getIdx(bus.sfx))) :
		return
	if (sounds.get(type) == null) :
		return
	var tempStream
	if (sounds[type] is Array) :
		var roll
		if (sounds[type].size() > 1) :
			roll = randi_range(0, sounds[type].size()-1)
			while (sounds[type][roll] == lastPlayed) :
				roll = randi_range(0, sounds[type].size()-1)
		else :
			roll = 0
		tempStream = sounds[type][roll]
		lastPlayed = sounds[type][roll]
	else :
		tempStream = sounds[type]
		lastPlayed = sounds[type]
	var tempPlayer = createSfxAudioStreamPlayer(tempStream, lowerVolume)
	add_child(tempPlayer)
	tempPlayer.play()
	await tempPlayer.finished
	tempPlayer.queue_free()
