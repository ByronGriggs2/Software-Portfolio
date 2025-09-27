using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace AwariaInterface
{
    internal class GhostManager
    {
        List<GhostData> _ghosts;
        public GhostManager()
        {
            _ghosts = new List<GhostData>();
            for (int i = 0; i < 3; i++)
            {
                _ghosts.Add(new GhostData((int)ghostID.nullVal, new Vector2(-1000, -1000)));
            }
        }
        public void reset()
        {
            for (int i = 0; i < 3; i++)
            {
                GhostData temp = _ghosts[i];
                temp._ID = ghostID.nullVal;
                temp._position = new Position2D(-1000, -1000);
                _ghosts[i] = temp;
            }
        }
        public List<GhostData> getData(ref LevelLayout levelManager)
        {
            List<GhostData> normGhosts = new List<GhostData>();
            for (int i = 0; i < _ghosts.Count; i++)
            {
                if (_ghosts[i]._position != new Position2D(-1000, -1000))
                {
                    GhostData temp = _ghosts[i];
                    temp._position = levelManager.normalizePosition(temp._position);
                    normGhosts.Add(temp);
                }
                else
                    normGhosts.Add(_ghosts[i]);
            }
            return normGhosts;
        }
        public void updateGhost(ghostID ID, Vector3 pos)
        {
            int found = -1;
            int firstEmptySlot = -1;
            for (int i = 0; i < _ghosts.Count; i++)
            {
                if (_ghosts[i]._ID == ID)
                    found = i;
                if (firstEmptySlot == -1 && _ghosts[i]._ID == ghostID.nullVal)
                    firstEmptySlot = i;
            }
            if (found == -1 && firstEmptySlot == -1)
                return;
            else if (found == -1 && firstEmptySlot != -1)
            {
                GhostData temp = new GhostData((int)ID, pos);
                _ghosts[firstEmptySlot] = temp;
            }
            else
            {
                _ghosts[found]._position = new Position2D(pos.x, pos.y);
            }
        }
    }
}
