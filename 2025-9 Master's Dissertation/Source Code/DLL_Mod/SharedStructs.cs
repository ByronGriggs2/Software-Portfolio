using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using UnityEngine;
using static AwariaInterface.GhostManager;

namespace AwariaInterface
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Position2D
    {
        readonly public float _X;
        readonly public float _Y;
        public Position2D(float x, float y)
        {
            _X = x;
            _Y = y;
        }
        public static bool operator==(Position2D left, Position2D right){ return left._X == right._X && left._Y == right._Y; }
        public static bool operator!=(Position2D left, Position2D right) { return left._X != right._X || left._Y != right._Y; }
        public override bool Equals(object o) { return o is Position2D other && this == other; }
        public override int GetHashCode()
        {
            unchecked
            {
                int hash = 17;
                hash = hash * 31 + _X.GetHashCode();
                hash = hash * 31 + _Y.GetHashCode();
                return hash;
            }
        }
        }
    public class Generator
    {
        public readonly int _ID;
        public readonly Position2D _position;
        public bool _broken;
        public int _want1;
        public int _want2;
        public float _time;
        public Generator(int ID, Position2D pos)
        {
            _ID = ID;
            _position = pos;
            _broken = false;
            _want1 = 10;
            _want2 = 10;
            _time = -1.0f;
        }
    }
    public enum component
    {
        none,
        gear,
        pipe,
        redBattery,
        blueBattery,
        blueThing,
        repaired = 10
    }
    public enum componentStatus
    {
        empty,
        working,
        ready
    }
    public class ComponentMachine
    {
        public readonly component _ID;
        public readonly Position2D _position;
        public componentStatus _status;
        public ComponentMachine(int ID, Position2D pos)
        {
            _ID = (component)ID;
            _position = pos;
            _status = componentStatus.ready;
        }
    }
    public enum ghostID
    {
        bull,
        cutwire1,
        cutwire2,
        nikita,
        zmora1,
        zmora2,
        zmora3,
        striga,
        nullVal,
        enumSize
    }
    public class GhostData
    {
        public ghostID _ID;
        public Position2D _position;
        public GhostData(int ID, Vector3 pos)
        {
            _ID = (ghostID)ID;
            _position = new Position2D(pos.x, pos.y);
        }
        public GhostData(int ID, Position2D pos)
        {
            _ID = (ghostID)ID;
            _position = pos;
        }
    }
}
