using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Pipes;
using UnityEngine;
using System.Collections;
using System.Threading;
using System.Runtime.InteropServices;
using System.IO;

namespace AwariaInterface
{
    internal class AwariaClient
    {
        // pipe
        NamedPipeClientStream _pipeServerToClient;
        NamedPipeClientStream _pipeClientToServer;
        byte[] _buf = new byte[32];
        List<byte> _cumulativeBuf = new List<byte>();
        // game state
        readonly object _stateLock = new object();
        StatePacket _currentState = new StatePacket();

        ////////////////////////////////////////////////////////////
        // Pipe management
        public AwariaClient()
        {
#if DEBUG
            if (Marshal.SizeOf<StatePacket>() != 254)
                Debug.Log("StatePacket must be exactly 254 bytes.");
#endif
            _pipeServerToClient = new NamedPipeClientStream(".", "AwariaServerToClientPipe", PipeDirection.In);
            _pipeClientToServer = new NamedPipeClientStream(".", "AwariaClientToServerPipe", PipeDirection.Out);
        }
        public void initialize()
        {
            _pipeServerToClient.Connect();
            _pipeClientToServer.Connect();
            Debug.Log("Client connected to server");
        }
        
        public void shutdown()
        {
            _pipeServerToClient?.Close();
            _pipeClientToServer?.Close();
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private readonly struct ClientGeneratorData
        {
            readonly private Position2D _position;
            // 0 = broken and unknown
            // 1-5 component
            // 10 fixed OR DNE
            readonly private int _want1;
            readonly private int _want2;
            readonly private byte _broken;
            readonly private float _time;
            public ClientGeneratorData(Generator gen)
            {
                _position = gen._position;
                _want1 = gen._want1;
                _want2 = gen._want2;
                _broken = gen._broken ? (byte)1 : (byte)0;
                _time = gen._time;
            }
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private readonly struct ClientComponentMachineData
        {
            readonly private int _ID;
            readonly private Position2D _position;
            readonly private int _status;
            public ClientComponentMachineData(ComponentMachine comp)
            {
                _ID = (int)comp._ID;
                _position = comp._position;
                _status = (int)comp._status;
            }
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private readonly struct ClientGhostData
        {
            readonly private Position2D _position;
            readonly private int _ID;
            public ClientGhostData (GhostData ghost)
            {
                _ID = (int)ghost._ID;
                _position = ghost._position;
            }
        }
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        private struct StatePacket
        {
            public Position2D _playerPosition;
            public int _heldItem0;
            public int _heldItem1;
            public ClientGhostData _ghost0;
            public ClientGhostData _ghost1;
            public ClientGhostData _ghost2;
            public ClientGeneratorData _generator0;
            public ClientGeneratorData _generator1;
            public ClientGeneratorData _generator2;
            public ClientGeneratorData _generator3;
            public ClientComponentMachineData _machine0;
            public ClientComponentMachineData _machine1;
            public ClientComponentMachineData _machine2;
            public ClientComponentMachineData _machine3;
            public ClientComponentMachineData _machine4;
            public Position2D _hazard0;
            public Position2D _hazard1;
            public Position2D _hazard2;
            public int _currentLevel;
            public byte _dashReady;
            public byte _playerDeath;
            public Position2D _inputDrag;
        }

        public void onUpdate()
        {
            byte[] toSend;
            lock (_stateLock)
            {
                toSend = structToByteArray();
                _currentState._playerDeath = (byte)0;
            }
            try
            {
                _pipeClientToServer.Write(toSend, 0, toSend.Length);
            }
            catch (IOException ex)
            {
                string str = "Unhandled error on pipe write: " + ex.Message + " " + (ex.HResult & 0xFFFF);
                Debug.Log(str);
            }
        }

        public void waitForAction()
        {
            _cumulativeBuf.Clear();
            try
            {
                while (_pipeServerToClient != null && _pipeServerToClient.IsConnected && _cumulativeBuf.Count < 11)
                {
                    // Read a chunk of data and add to cumulative buffer
                    // blocking
                    int bytesRead = _pipeServerToClient.Read(_buf, 0, _buf.Length);
                    if (bytesRead == 0)
                        return;
                    _cumulativeBuf.AddRange(_buf.Take(bytesRead));
                }
            }
            catch (IOException ex) when ((ex.HResult & 0xFFFF) == 233)
            {
                Debug.Log("Server closed pipe (Win32 233), exiting read loop cleanly.");
            }
            catch (IOException ex) when ((ex.HResult & 0xFFFF) != 233)
            {
                string str = "Unhandled error on pipe read: " + ex.Message + " " + (ex.HResult & 0xFFFF);
                Debug.Log(str);
            }
        }
        
        // IHATEC#IHATEC#IHATEC#IHATEC#
        private byte[] structToByteArray()
        {
            int size = Marshal.SizeOf<StatePacket>();
            // allocate memory for ptr to raw data
            IntPtr ptr = Marshal.AllocHGlobal(size);
            // copy data from parameter to ptr
            Marshal.StructureToPtr(_currentState, ptr, true);
            // allocate memory for byte array of the correct size
            byte[] arr = new byte[size];
            // copy data from ptr to byte array
            Marshal.Copy(ptr, arr, 0, size);
            // delete ptr
            Marshal.FreeHGlobal(ptr);
            // return copy of byte array
            return arr;
            // delete byte array
        }

        //////////////////////////////////////////////////////////////////////////////////
        // Getters
        public void getMostRecent(ref Vector2 moveVector, ref bool primary, ref bool secondary, ref bool cancel)
        {
            if (_cumulativeBuf.Count < 11)
                Debug.Log("###########ERROR: invalid packet size");
            // bytes 0-3: a single float x
            // bytes 4-7: a single float y
            // bytes 8-10: 3 bools
            byte[] snapshot;
            snapshot = _cumulativeBuf.ToArray();
            moveVector.x = BitConverter.ToSingle(snapshot, 0);
            moveVector.y = BitConverter.ToSingle(snapshot, 4);
            // |= because of rising edge
            primary |= snapshot[8] != 0;
            secondary |= snapshot[9] != 0;
            cancel |= snapshot[10] != 0;
        }
        public Position2D getPlayerPosition()
        {
            Position2D temp;
            lock (_stateLock)
            {
                temp = _currentState._playerPosition;
            }
            return temp;
        }

        /////////////////////////////////////////////////
        // Updaters
        // These use data from other classes in the .dll
        public void updateGhosts(List<GhostData> ghosts)
        {
#if DEBUG
            if (ghosts.Count != 3)
                Debug.Log("**********ERROR: AwariaClient should always receive data for all 3 ghosts");
#endif
            lock (_stateLock)
            {
                _currentState._ghost0 = new ClientGhostData(ghosts[0]);
                _currentState._ghost1 = new ClientGhostData(ghosts[1]);
                _currentState._ghost2 = new ClientGhostData(ghosts[2]);
            }
        }
        public void updateGenerators(List<Generator> generators)
        {
#if DEBUG
            if (generators.Count != 4)
                Debug.Log("**********ERROR: AwariaClient should always receive data for all 4 generators");
#endif
            lock (_stateLock)
            {
                _currentState._generator0 = new ClientGeneratorData(generators[0]);
                _currentState._generator1 = new ClientGeneratorData(generators[1]);
                _currentState._generator2 = new ClientGeneratorData(generators[2]);
                _currentState._generator3 = new ClientGeneratorData(generators[3]);
            }
        }
        public void updateComponentMachines(List<ComponentMachine> components)
        {
#if DEBUG
            if (components.Count != 5)
                Debug.Log("**********ERROR: AwariaClient should always receive data for all 5 components");
#endif
            lock (_stateLock)
            {
                _currentState._machine0 = new ClientComponentMachineData(components[0]);
                _currentState._machine1 = new ClientComponentMachineData(components[1]);
                _currentState._machine2 = new ClientComponentMachineData(components[2]);
                _currentState._machine3 = new ClientComponentMachineData(components[3]);
                _currentState._machine4 = new ClientComponentMachineData(components[4]);
            }
        }
        public void updateHazards(List<Position2D> hazards)
        {
#if DEBUG
            if (hazards.Count != 3)
                Debug.Log("**********ERROR: AwariaClient should always receive data for 3 hazards");
#endif
            lock (_stateLock)
            {
                _currentState._hazard0 = hazards[0];
                _currentState._hazard1 = hazards[1];
                _currentState._hazard2 = hazards[2];
            }
        }
        /////////////////////////////////////////////////
        // Setters
        // For reading game state. This class cannot go ask other classes for their data without creating a dependency. Instead it makes public functions to update its state each frame
        public void setCurrentLevel(int level)
        {
            lock (_stateLock)
            {
                _currentState._currentLevel = level;
            }
        }
        public void setPlayerStatus(Position2D pos, int holding0, int holding1, bool dashReady, Position2D inputDrag)
        {
            lock (_stateLock)
            {
                _currentState._playerPosition = pos;
                _currentState._heldItem0 = holding0;
                _currentState._heldItem1 = holding1;
                _currentState._dashReady = dashReady ? (byte)1 : (byte)0;
                _currentState._inputDrag = inputDrag;
            }
        }
        public void setPlayerDeath()
        {
            lock (_stateLock)
            {
                _currentState._playerDeath = (byte)1;
            }
        }
    }
}
