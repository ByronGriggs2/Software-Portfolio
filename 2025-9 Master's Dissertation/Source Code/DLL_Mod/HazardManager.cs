using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace AwariaInterface
{
    internal class HazardManager
    {
        List<Position2D> _hazards = new List<Position2D>();
        public void addNewHazard()
        {
            _hazards.Add(new Position2D(-1000, -1000));
        }
        public void updateHazard(int index, Position2D pos)
        {
            _hazards[index] = pos;
        }
        public void removeHazard(int index)
        {
            _hazards.RemoveAt(index);
        }
        public List<Position2D> getData(ref LevelLayout levelManager, Position2D playerPos)
        {
            // If there are less than 3 elements in the hazard list, add filler data and return
            if (_hazards.Count <= 3)
            {
                List<Position2D> temp0 = _hazards;
                for (int i = _hazards.Count; i < 3; i++)
                {
                    temp0.Add(new Position2D(-1000, -1000));
                }
                return temp0;
            }

            float closestDistance = 10000;
            int closestElement = -1;
            for (int i = 0; i < _hazards.Count; i++)
            {
                float tempDist = dist(playerPos, levelManager.normalizePosition(_hazards[i]));
                if (tempDist < closestDistance)
                {
                    closestElement = i;
                    closestDistance = tempDist;
                }
            }

            float secondDistance = 10000;
            int secondElement = -1;
            for (int i = 0; i < _hazards.Count; i++)
            {
                float tempDist = dist(playerPos, levelManager.normalizePosition(_hazards[i]));
                if (i != closestElement && tempDist < secondDistance)
                {
                    secondElement = i;
                    secondDistance = tempDist;
                }
            }

            float thirdDistance = 10000;
            int thirdElement = -1;
            for (int i = 0; i < _hazards.Count; i++)
            {
                float tempDist = dist(playerPos, levelManager.normalizePosition(_hazards[i]));
                if (i != closestElement && i != secondElement && tempDist < thirdDistance)
                {
                    thirdElement = i;
                    thirdDistance = tempDist;
                }
            }
            List<Position2D> temp = new List<Position2D>();
            temp.Add(_hazards[closestElement]);
            temp.Add(_hazards[secondElement]);
            temp.Add(_hazards[thirdElement]);
            return temp;
        }
        private float dist(Position2D left, Position2D right)
        {
            double deltaX = (double)(left._X - right._X);
            double deltaY = (double)(left._Y - right._Y);
            return (float)Math.Sqrt(Math.Pow(deltaX, 2.0) + Math.Pow(deltaY, 2.0));
        }
    }
}
