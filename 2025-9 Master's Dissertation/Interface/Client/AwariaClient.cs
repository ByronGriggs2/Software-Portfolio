using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Pipes;
using UnityEngine;
using System.Collections;
using System.Threading;

namespace AwariaInterface
{
    internal class AwariaClient
    {
        NamedPipeClientStream _pipe;
        byte[] _buf = new byte[32];
        List<byte> _cumulativeBuf = new List<byte>();

        bool _shutdownThread = false;
        Thread _readerThread;
        readonly object _lock = new object();

        public AwariaClient()
        {
            _pipe = new NamedPipeClientStream(".", "AwariaControlPipe", PipeDirection.In);
        }
        public void initialize()
        {
            _pipe.Connect();
            Debug.Log("Client connected to server");
            _readerThread = new Thread(receiveLoop);
            _readerThread.IsBackground = true;
            _readerThread.Start();
        }
        private void receiveLoop()
        {
            while (_pipe != null && _pipe.IsConnected)
            {
                // Read a chunk of data and add to cumulative buffer
                int bytesRead = _pipe.Read(_buf, 0, _buf.Length);
                if (bytesRead == 0)
                    break;
                lock (_lock)
                {
                    if (_shutdownThread)
                        break;
                    _cumulativeBuf.AddRange(_buf.Take(bytesRead));
                    // Ensure the first 11 bytes of the cumulative buffer are the most recent input package
                    while (_cumulativeBuf.Count >= 22)
                    {
                        _cumulativeBuf.RemoveRange(0, 11);
                    }
                }
                Thread.Sleep(10);
            }
        }
        public void getMostRecent(ref Vector2 moveVector, ref bool primary, ref bool secondary, ref bool cancel)
        {
            if (_cumulativeBuf.Count < 11)
                return;
            // bytes 0-3: a single float x
            // bytes 4-7: a single float y
            // bytes 8-10: 3 bools
            // If you do not read exactly 11 bytes every time the server and client will get out of sync!
            byte[] snapshot;
            lock (_lock)
            {
                snapshot = _cumulativeBuf.ToArray();
                // Once the input has been read, delete the record so it does not get read twice
                _cumulativeBuf.RemoveRange(0, 11);
            }
            moveVector.x = BitConverter.ToSingle(snapshot, 0);
            moveVector.y = BitConverter.ToSingle(snapshot, 4);
            // |= because of rising edge
            primary |= snapshot[8] != 0;
            secondary |= snapshot[9] != 0;
            cancel |= snapshot[10] != 0;
        }
        public void shutdown()
        {
            lock (_lock)
            {
                _shutdownThread = true;
            }
            _pipe?.Close();
            _readerThread?.Join();
        }
    }
}
