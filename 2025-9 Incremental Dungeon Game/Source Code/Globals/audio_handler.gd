extends Node

const originalTheme = preload("res://Audio/OST/theme_01.wav")
const mainMenuTheme = originalTheme
var musicStream : AudioStreamPlayer = null

enum musicMode {silent,playlist,mainMenu}
var musicModeMutex = Mutex.new()
var currentMusicMode : musicMode = musicMode.silent
var nextMusicMode : musicMode = musicMode.silent

var myTimer : Timer = null
var timerPing : bool = false
var musicFinishedPing : bool = false
func _ready() :
	musicStream = AudioStreamPlayer.new()
	add_child(musicStream)
	musicStream.bus = "Music"
	musicStream.connect("finished", _on_music_finished)
	
	myTimer = Timer.new()
	add_child(myTimer)
	myTimer.connect("timeout", _on_timeout)
	
	makeConnections()
	_on_main_menu_loaded()

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
