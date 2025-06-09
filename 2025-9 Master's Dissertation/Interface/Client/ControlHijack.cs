using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using UnityEngine;
using System.Collections;

namespace AwariaInterface
{
    // Note: No effort has been made to ensure anything in this solution is thread safe
    public class ControlHijack : MonoBehaviour
    {
        public static ControlHijack Instance { get; private set; }
        // These control values are set by ControlHijack each frame
        // and are given to Awaria when it requests them.
        private Vector2 _moveVector = Vector2.zero;
        // NOTE: These booleans are meant to represent the rising edge. They should be set to false whenever the getter functions grab them, and never set to false by anything else.
        private bool _primaryPressed = false;
        private bool _secondaryPressed = false;
        private bool _cancelPressed = false;

        // Used for communication with other side of interface
        AwariaClient _client;

        void Awake()
        {
            if (Instance != null && Instance != this)
            {
                Destroy(this);
                return;
            }
            Instance = this;
            DontDestroyOnLoad(gameObject);

            string userProfile = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
            string localLow = Path.Combine(userProfile, "AppData", "LocalLow");
            string path = Path.Combine(localLow, "vanripper", "Awaria", "testfile.txt");
            File.WriteAllText(path, "Byron's .dll has been loaded");
            Debug.Log("Byron's .dll has been loaded 2");

            _client = new AwariaClient();
            _client.initialize();

            // Debugging
            //StartCoroutine(testSequence());
        }
        void Update()
        {
            _client.getMostRecent(ref _moveVector, ref _primaryPressed, ref _secondaryPressed, ref _cancelPressed);
        }
        public Vector2 getMoveVector() { return _moveVector; }
        public bool getPrimary() { 
            if (_primaryPressed)
            {
                _primaryPressed = false;
                return true;
            }
            return false;
        }
        public bool getSecondary() { 
            if (_secondaryPressed)
            {
                _secondaryPressed = false;
                return true;
            }
            return false;
        }
        public bool getCancel() { 
            if (_cancelPressed)
            {
                _cancelPressed = false;
                return true;
            }
            return false;
        }

        private IEnumerator testSequence()
        {
            Debug.Log("testSequence started");
            yield return new WaitForSecondsRealtime(5f);
            for (int i = 0; i < 3; i++)
            {
                _primaryPressed = true;
                yield return new WaitForSecondsRealtime(0.5f);
            }
            while (true)
            {
                _moveVector.x = 0.5f;
                _moveVector.y = 0;
                yield return new WaitForSecondsRealtime(0.3f);
                _moveVector.x = 0;
                _moveVector.y = 0.5f;
                yield return new WaitForSecondsRealtime(0.3f);
                _moveVector.x = -0.5f;
                _moveVector.y = 0;
                yield return new WaitForSecondsRealtime(0.3f);
                _moveVector.x = 0;
                _moveVector.y = -0.5f;
                yield return new WaitForSecondsRealtime(0.3f);
            }
        }
        void OnDestroy()
        {
            _client?.shutdown();
        }
    }
}
