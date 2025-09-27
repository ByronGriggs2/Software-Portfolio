using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.IO;
using UnityEngine;
using System.Collections;

namespace AwariaInterface
{
    [DefaultExecutionOrder(-1000)]
    public class MainInterface : MonoBehaviour, IInputProvider
    {
        ///////////////////////////////// TESTING
        // Desync test- forces the client to alternate reading twice in a row or write twice in a row once every 2 seconds
        readonly bool _testDesync = false;
        bool _testDesync_skipRead = false;
        bool _testDesync_skipWrite = false;
        bool _testDesync_nextSkipIsRead = true;
        IEnumerator testDesyncCoroutine()
        {
            if (!_testDesync)
            {
                Debug.Log("_testDesync not enabled");
                yield break;
            }
            yield return new WaitForSeconds(2);
            while (true)
            {
                yield return new WaitForSeconds(2);
                if (_testDesync_nextSkipIsRead)
                    _testDesync_skipRead = true;
                else
                    _testDesync_skipWrite = true;
            }
        }
        ////////////////////////////////
        // Special background thread: its only goal is to keep Windows from locking if the main thread blocks/freezes
        readonly bool _runSpinThread = false;
        internal class KeepAliveThread
        {
            private Thread thread;
            private bool running;
            public void Start()
            {
                if (thread != null) return;
                running = true;
                thread = new Thread(Run);
                thread.IsBackground = true; // so it won't block process exit
                thread.Start();
            }
            public void Stop(){ running = false; }

            private void Run()
            {
                while (running)
                {
                    try
                    {
                        // Do something that prevents idle lock
                        PreventSleep();

                        Thread.Sleep(30000); // check every 30s
                    }
                    catch { /* swallow errors */ }
                }
            }

            private void PreventSleep()
            {
                // Option A: Windows API call
                NativeMethods.SetThreadExecutionState(
                    NativeMethods.ES_CONTINUOUS | NativeMethods.ES_SYSTEM_REQUIRED | NativeMethods.ES_DISPLAY_REQUIRED);
            }
        }
        internal static class NativeMethods
        {
            [System.Runtime.InteropServices.DllImport("kernel32.dll")]
            public static extern uint SetThreadExecutionState(uint esFlags);

            public const uint ES_CONTINUOUS = 0x80000000;
            public const uint ES_SYSTEM_REQUIRED = 0x00000001;
            public const uint ES_DISPLAY_REQUIRED = 0x00000002;
        }
        ////////////////////////////////////////////
        bool _physicsHasRun;
        private void FixedUpdate()
        {
            _physicsHasRun |= true;
            _generatorManager.myFixedUpdate(Time.fixedDeltaTime);
        }
        public static MainInterface Instance { get; private set; }
        public Vector2 getMoveVector() => _moveVector;
        public bool getPrimary() => _primaryPressed;
        public bool getSecondary() => _secondaryPressed;
        public bool getCancel() => _cancelPressed;
        private Vector2 _moveVector = Vector2.zero;
        private bool _primaryPressed = false;
        private bool _secondaryPressed = false;
        private bool _cancelPressed = false;

        // Used for communication with other side of interface
        AwariaClient _client;
        int _currentLevel;

        GeneratorManager _generatorManager;
        ComponentMachineManager _componentManager;
        GhostManager _ghostManager;
        HazardManager _hazardManager;
        LevelLayout _levelManager;

        private PlayerScript _playerScript;
        private List<GhostAiScript> _ghostReferences = new List<GhostAiScript>();
        private List<MonoBehaviour> _hazardReferences = new List<MonoBehaviour>();

        void Awake()
        {
            if (Instance != null && Instance != this)
            {
                Destroy(this);
                return;
            }
            Instance = this;
            DontDestroyOnLoad(gameObject);

            // Get full diagnostics
            Application.SetStackTraceLogType(LogType.Log, StackTraceLogType.Full);
            Application.SetStackTraceLogType(LogType.Warning, StackTraceLogType.Full);
            Application.SetStackTraceLogType(LogType.Error, StackTraceLogType.Full);
            Application.SetStackTraceLogType(LogType.Exception, StackTraceLogType.Full);
            var stdoutPath = Path.Combine(Application.persistentDataPath, "stdout.txt");
            var stderrPath = Path.Combine(Application.persistentDataPath, "stderr.txt");
            var stdoutWriter = new StreamWriter(stdoutPath) { AutoFlush = true };
            var stderrWriter = new StreamWriter(stderrPath) { AutoFlush = true };
            Console.SetOut(stdoutWriter);
            Console.SetError(stderrWriter);
            //

            string userProfile = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
            string localLow = Path.Combine(userProfile, "AppData", "LocalLow");
            string path = Path.Combine(localLow, "vanripper", "Awaria", "testfile.txt");
            File.WriteAllText(path, "Byron's .dll has been loaded");
            Debug.Log("Byron's .dll has been loaded 2");

            _client = new AwariaClient();
            _client.initialize();
            _currentLevel = 0;
            _generatorManager = new GeneratorManager();
            _componentManager = new ComponentMachineManager();
            _ghostManager = new GhostManager();
            _hazardManager = new HazardManager();
            _levelManager = new LevelLayout();

            _playerScript = GameObject.FindObjectOfType<PlayerScript>();
            if (_testDesync)
                base.StartCoroutine(testDesyncCoroutine());
            if (_runSpinThread)
            {
                KeepAliveThread spinThread = new KeepAliveThread();
                spinThread.Start();
            }
        }
        void OnDestroy()
        {
            base.StopAllCoroutines();
            _client?.shutdown();
        }
        //Position2D oldPlayerPos = new Position2D(0, 0);
        //Vector2 oldMoveInput = new Vector2(0, 0);
        void Update()
        {
            // The physics loop (FixedUpdate()) of all classes in Assembly-CSharp are guaranteed to run exactly once per game state capture
            // Because the game state capture is in Update(), the render loop (Update()) of all classes in Assembly-CSharp is guaranteed to run at least once per game state capture.
            // Because DefaultExecutionOrder(-1000), all classes' Update() runs the same number of times per game state
            /* in other words,
            1. S0 capture->send game state->receive A0->store A0
            2. 1 or more render loops using A0
            3. exactly 1 physics loop using A0
            4. S1 capture->send game state->receive A1->store A1
            5. 1 or more render loops using A1
            ...
            */
            // Boolean controls are expected by Update() to be false unless there was a rising edge THAT RENDER FRAME, so they need to be reset even when a game state capture is not needed
            _primaryPressed = false;
            _secondaryPressed = false;
            _cancelPressed = false;
            if (_physicsHasRun || Time.timeScale == 0)
            {
                // Forces a desync every 2 seconds
                if (_testDesync)
                {
                    if (_testDesync_skipRead)
                        _testDesync_skipRead = false;
                    else
                    {
                        captureGameState();
                        sendGameState();
                    }
                    if (_testDesync_skipWrite)
                        _testDesync_skipWrite = false;
                    else
                    {
                        getExternalAction();
                    }
                    _physicsHasRun = false;
                }
                // When not testing, this is all that runs inside Update()
                else
                {
                    captureGameState();
                    sendGameState();
                    getExternalAction();
                    _physicsHasRun = false;
                }
            }
        }
        // Most of the game state is already captured by hard coding or setter functions. The only thing that needs to
        // be captured here is data changed by FixedUpdate() which I believe is only ghost/player/hazard positions
        private void captureGameState()
        {
            checkReferences();
            capturePlayerStatus();
            captureGhostStatus();
            // The generators and machines do NOT need to be referenced or explicitly captured here. This is ONLY because
            // 1. The generators and machines do not need to know player inputs
            // 2. The generators and machines do not change the game state between the start of the frame and the first Update() call
            // They do still need to call the updater functions, though.
            captureHazardStatus();
        }
        private void checkReferences()
        {
            if (_playerScript == null)
            {
                _playerScript = GameObject.FindObjectOfType<PlayerScript>();
            }
            for (int i = 0; i < _ghostReferences.Count; i++)
            {
                if (_ghostReferences[i] == null)
                {
                    _ghostReferences.RemoveAt(i);
                }
            }
            for (int i = 0; i < _hazardReferences.Count; i++)
            {
                if (_hazardReferences[i] == null)
                {
                    _hazardManager.removeHazard(i);
                    _hazardReferences.RemoveAt(i);
                }
            }
        }
        private void capturePlayerStatus()
        {
            {
                Vector3 pos;
                int[] hands;
                bool dashReady;
                Vector2 moveDir;
                if (_playerScript == null)
                {
                    pos = new Vector3(-1000, -1000, 0);
                    hands = new int[2];
                    hands[0] = 0;
                    hands[1] = 0;
                    dashReady = false;
                    moveDir = new Vector2(-1000, -1000);
                }
                else
                {
                    pos = _playerScript.byronGetPlayerPosition();
                    hands = _playerScript.byronGetHands();
                    dashReady = _playerScript.byronGetDash();
                    moveDir = _playerScript.byronGetMoveDir();
                }
                _client.setPlayerStatus(_levelManager.normalizePosition(new Position2D(pos.x, pos.y)), hands[0], hands[1], dashReady, new Position2D(moveDir.x, moveDir.y));
            }
        }
        private void captureGhostStatus()
        {
            for (int i = 0; i < _ghostReferences.Count; i++)
            {
                ghostID ghostName;
                if (_ghostReferences[i] is DogoAiScript)
                    ghostName = ghostID.bull;
                else if (_ghostReferences[i] is CutwireAiScript)
                    ghostName = ghostID.cutwire1;
                else if (_ghostReferences[i] is DoppelAiScript)
                    ghostName = ghostID.cutwire2;
                else if (_ghostReferences[i] is NikitaAiScript)
                    ghostName = ghostID.nikita;
                else if (_ghostReferences[i] is ZmoraAiScript && _currentLevel == 1)
                    ghostName = ghostID.zmora1;
                else if (_ghostReferences[i] is ZmoraAiScript && (_currentLevel != 9))
                    ghostName = ghostID.zmora2;
                else if (_ghostReferences[i] is ZmoraAiScript)
                    ghostName = ghostID.zmora3;
                else if (_ghostReferences[i] is StrigaAiScript)
                    ghostName = ghostID.striga;
                else
                    ghostName = ghostID.nullVal;
                Vector3 position = _ghostReferences[i].byronGetGhostPosition();
                _ghostManager.updateGhost(ghostName, position);
            }
        }
        private void captureHazardStatus()
        {
            for (int i = 0; i < _hazardReferences.Count; i++)
            {
                Vector3 pos = _hazardReferences[i].transform.position;
                Position2D temp = _levelManager.normalizePosition(new Position2D(pos.x, pos.y));
                _hazardManager.updateHazard(i, temp);
            }
        }
        private void getExternalAction()
        {
            // recieve action
            _client.waitForAction();
            // store action and yield
            _client.getMostRecent(ref _moveVector, ref _primaryPressed, ref _secondaryPressed, ref _cancelPressed);
        }
        private void sendGameState()
        {
            _client.updateGhosts(_ghostManager.getData(ref _levelManager));
            _client.updateGenerators(_generatorManager.getData());
            _client.updateComponentMachines(_componentManager.getData());
            _client.updateHazards(_hazardManager.getData(ref _levelManager, _client.getPlayerPosition()));
            _client.onUpdate();
        }
        ////////////////// Lets MainInterface know when new objects are created
        // PlayerScript is done inside checkReferences
        public void addGhostReference(GhostAiScript myScript) { _ghostReferences.Add(myScript); }
        public void addHazard(MonoBehaviour myScript) { 
            for (int i = 0; i < _hazardReferences.Count; i++)
            {
                if (myScript == _hazardReferences[i])
                    return;
            }
            _hazardReferences.Add(myScript);
            _hazardManager.addNewHazard();
        }
        ////////////////// External Game State Updaters
        public void setCurrentLevel(int level)
        {
            _client.setCurrentLevel(level);
            _currentLevel = level;
        }
        public void setupGenerators()
        {
            _generatorManager.setup(ref _levelManager, _currentLevel);
            _componentManager.setup(ref _levelManager, _currentLevel);
        }
        public void levelEndCleanup()
        {
            _generatorManager.reset();
            _componentManager.reset();
            _ghostManager.reset();
        }
        public void setPlayerDeath()
        { _client.setPlayerDeath(); }

        public void takeItem(string componentMachineName)
        { _componentManager.takeItem(componentMachineName); }
        public void placeItem(string componentMachineName)
        { _componentManager.placeItem(componentMachineName); }  
        public void finishedWorking(string componentMachineName)
        { _componentManager.finishedWorking(componentMachineName); }

        public void damageGenerator(string engineName)
        { _generatorManager.damageGenerator(engineName); }
        public void checkDamagedGenerator(string engineName, int want1, int want2)
        { _generatorManager.checkDamagedGenerator(engineName, want1, want2); }
        public void partialFixGenerator(string engineName, int want1, int want2)
        { _generatorManager.checkDamagedGenerator(engineName, want1, want2); }
        public void fixGenerator(string engineName)
        { _generatorManager.fixGenerator(engineName); }
    }
}
