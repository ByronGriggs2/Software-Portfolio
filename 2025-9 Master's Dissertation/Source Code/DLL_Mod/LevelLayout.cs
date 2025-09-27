using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace AwariaInterface
{
    internal class LevelLayout
    {
        public struct Layout
        {
            public readonly float _minX;
            public readonly float _maxX;
            public readonly float _minY;
            public readonly float _maxY;

            public readonly List<Generator> _generators;
            public readonly List<ComponentMachine> _components;
            public Layout(float minX, float maxX, float minY, float maxY, List<Generator> generators, List<ComponentMachine> components)
            {
                _minX = minX;
                _maxX = maxX;
                _minY = minY;
                _maxY = maxY;
                _generators = generators;
                _components = components;
            }
        }
        List<Layout> _layouts;
        float _minX;
        float _maxX;
        float _minY;
        float _maxY;
        public LevelLayout()
        {
            initializeLevelLayouts();
            rescale();
            softVerify();
            fillInGaps();
        }
        public List<Generator> getNormalizedGenerators(int level)
        {
            level--;
            List<Generator> temp = new List<Generator>();
            float xRange = _maxX - _minX;
            float yRange = _maxY - _minY;
            float centerX = (_minX + _maxX) / 2.0f;
            float centerY = (_minY + _maxY) / 2.0f;
            for (int i = 0; i < _layouts[level]._generators.Count; i++)
            {
                if (_layouts[level]._generators[i]._position != new Position2D(-1000, -1000))
                {
                    float newX = (_layouts[level]._generators[i]._position._X - centerX) * 2.0f / xRange;
                    float newY = (_layouts[level]._generators[i]._position._Y - centerY - (4.0f - 2.55499f)) * 2.0f / yRange;
                    int newID = _layouts[level]._generators[i]._ID;
                    temp.Add(new Generator(newID, new Position2D(newX, newY)));
                }
                else
                    temp.Add(_layouts[level]._generators[i]);
            }
            return temp;
        }
        public List<ComponentMachine> getNormalizedComponents(int level)
        {
            level--;
            List<ComponentMachine> temp = new List<ComponentMachine>();
            float xRange = _maxX - _minX;
            float yRange = _maxY - _minY;
            float centerX = (_minX + _maxX) / 2.0f;
            float centerY = (_minY + _maxY) / 2.0f;
            for (int i = 0; i < _layouts[level]._components.Count; i++)
            {
                if (_layouts[level]._components[i]._position != new Position2D(-1000, -1000))
                {
                    float newX = (_layouts[level]._components[i]._position._X - centerX) * 2.0f / xRange;
                    float newY = (_layouts[level]._components[i]._position._Y - centerY - (4 - 2.55499f)) * 2.0f / yRange;
                    int newID = (int)_layouts[level]._components[i]._ID;
                    temp.Add(new ComponentMachine(newID, new Position2D(newX, newY)));
                }
                else
                    temp.Add(_layouts[level]._components[i]);
            }
            return temp;
        }
        public Position2D normalizePosition(Position2D pos)
        {
            float xRange = _maxX - _minX;
            float yRange = _maxY - _minY;
            float centerX = (_minX + _maxX) / 2.0f;
            float centerY = (_minY + _maxY) / 2.0f;
            float newX = (pos._X - centerX) * 2.0f / xRange;
            float newY = (pos._Y - centerY) * 2.0f / yRange;
            return new Position2D(newX, newY);
        }
        // I changed my mind and want minX and minY to be the same for every stage
        private void rescale()
        {
            _minX = 100;
            _maxX = -100;
            _minY = 100;
            _maxY = -100;
            for (int i = 0; i < _layouts.Count; i++)
            {
                if (_minX > _layouts[i]._minX)
                    _minX = _layouts[i]._minX;
                if (_maxX < _layouts[i]._maxX)
                    _maxX = _layouts[i]._maxX;
                if (_minY > _layouts[i]._minY)
                    _minY = _layouts[i]._minY;
                if (_maxY < _layouts[i]._maxY)
                    _maxY = _layouts[i]._maxY;
            }
        }
        private void fillInGaps()
        {
            for (int i = 0; i < _layouts.Count; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    // end of array
                    if (_layouts[i]._generators.Count - 1 < j)
                        _layouts[i]._generators.Add(new Generator(j, new Position2D(-1000, -1000)));
                    // middle of array
                    if (_layouts[i]._generators[j]._ID != j)
                        _layouts[i]._generators.Insert(j, new Generator(j, new Position2D(-1000, -1000)));
                }
            }
            for (int i = 0; i < _layouts.Count; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    // end of array
                    if (_layouts[i]._components.Count - 1 < j)
                        _layouts[i]._components.Add(new ComponentMachine((int)component.repaired, new Position2D(-1000, -1000)));
                }
            }
        }
        private void softVerify()
        {
            if (_layouts.Count != 13)
                Debug.Log("**********ERROR: Layouts list contains not 13 levels");
            for (int i = 0; i < _layouts.Count; i++)
            {
                for (int j = 0; j < _layouts[i]._generators.Count; j++)
                {
                    if (_layouts[i]._generators[j]._position._X < _layouts[i]._minX - 0.01 ||
                        _layouts[i]._generators[j]._position._X > _layouts[i]._maxX + 0.01 ||
                        _layouts[i]._generators[j]._position._Y - (4-2.55499f) < _layouts[i]._minY - 0.01 ||
                        _layouts[i]._generators[j]._position._Y - (4-2.55499f) > _layouts[i]._maxY + 0.01)
                    {
                        string str = "**********ERROR: generator " + (j+1) + " at level " + (i+1) + " is out of bounds";
                        string str2 = "(" + _layouts[i]._generators[j]._position._X + ", " + (_layouts[i]._generators[j]._position._Y - (4 - 2.55499f)) + ")";
                        Debug.Log(str);
                        Debug.Log(str2);
                    }
                }
                for (int j = 0; j < _layouts[i]._components.Count; j++)
                {
                    if (_layouts[i]._components[j]._position._X < _layouts[i]._minX - 0.01 ||
                        _layouts[i]._components[j]._position._X > _layouts[i]._maxX + 0.01 ||
                        _layouts[i]._components[j]._position._Y - (4 - 2.55499f) < _layouts[i]._minY - 0.01 ||
                        _layouts[i]._components[j]._position._Y - (4 - 2.55499f) > _layouts[i]._maxY + 0.01)
                    {
                        string str = "**********ERROR: component " + (j+1) + " at level " + (i+1) + " is out of bounds";
                        string str2 = "(" + _layouts[i]._components[j]._position._X + ", " + (_layouts[i]._components[j]._position._Y - (4 - 2.55499f)) + ")";
                        Debug.Log(str);
                        Debug.Log(str2);
                    }
                }
            }
        }
        private void initializeLevelLayouts()
        {
            _layouts = new List<Layout>();

            float minX, maxX, minY, maxY;
            List<Generator> generators = new List<Generator>();
            List<ComponentMachine> components = new List<ComponentMachine>();
            // Level 1
            {
                minX = -4.55499f;
                maxX = 4.555f;
                minY = -2.55497f;
                maxY = 2.55498f;
                generators.Add(new Generator(2, new Position2D(4.5f, 1.0f)));

                components.Add(new ComponentMachine(1, new Position2D(-4.5f, 3.0f)));
                components.Add(new ComponentMachine(2, new Position2D(-1.5f, 3.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 2
            {
                minX = -4.55504f;
                maxX = 4.55499f;
                minY = -2.55497f;
                maxY = 2.55499f;
                generators.Add(new Generator(1, new Position2D(-4.5f, 4.0f)));
                generators.Add(new Generator(2, new Position2D(4.5f, 0.0f)));

                components.Add(new ComponentMachine(1, new Position2D(3.5f, 4.0f)));
                components.Add(new ComponentMachine(2, new Position2D(-3.5f, 0.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 3
            {
                minX = -4.55502f;
                maxX = 4.55502f;
                minY = -2.555f;
                maxY = 2.55499f;
                generators.Add(new Generator(1, new Position2D(-4.5f, 1.0f)));
                generators.Add(new Generator(2, new Position2D(2.5f, 0.0f)));

                components.Add(new ComponentMachine(4, new Position2D(-2.5f, 3.0f)));
                components.Add(new ComponentMachine(5, new Position2D(4.0f, 0.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 4
            {
                minX = -5.55501f;
                maxX = 5.55501f;
                minY = -2.55499f;
                maxY = 2.55499f;
                generators.Add(new Generator(1, new Position2D(-5.5f, 1.0f)));
                generators.Add(new Generator(2, new Position2D(5.5f, 1.0f)));

                components.Add(new ComponentMachine(4, new Position2D(2.5f, 3.0f)));
                components.Add(new ComponentMachine(5, new Position2D(-3.0f, 3.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 5
            {
                minX = -4.55501f;
                maxX = 4.55499f;
                minY = -3.55497f;
                maxY = 3.55499f;
                generators.Add(new Generator(1, new Position2D(-4.5f, 0.0f)));
                generators.Add(new Generator(2, new Position2D(4.5f, 5.0f)));
                
                components.Add(new ComponentMachine(2, new Position2D(-3.5f, 4.0f)));
                components.Add(new ComponentMachine(5, new Position2D(0.0f, 0.0f)));
                components.Add(new ComponentMachine(4, new Position2D(3.5f, 0.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 6
            {
                minX = -5.55266f;
                maxX = 5.55499f;
                minY = -3.55497f;
                maxY = 3.55498f;
                generators.Add(new Generator(1, new Position2D(-5.5f, 1.0f)));
                generators.Add(new Generator(2, new Position2D(5.5f, 0.0f)));

                components.Add(new ComponentMachine(1, new Position2D(5.5f, 4.0f)));
                components.Add(new ComponentMachine(4, new Position2D(-4.5f, 4.0f)));
                components.Add(new ComponentMachine(5, new Position2D(-1.0f, 5.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 7
            {
                minX = -7.05498f;
                maxX = 7.05498f;
                minY = -3.555f;
                maxY = 3.55497f;
                generators.Add(new Generator(1, new Position2D(1.5f, 0.0f)));
                generators.Add(new Generator(2, new Position2D(1.0f, 5.0f)));

                components.Add(new ComponentMachine(1, new Position2D(-2.0f, 5.0f)));
                components.Add(new ComponentMachine(2, new Position2D(1.0f, 5.0f)));
                components.Add(new ComponentMachine(4, new Position2D(-3.0f, 0.0f)));
                components.Add(new ComponentMachine(5, new Position2D(1.5f, 0.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 8
            {
                minX = -6.555f;
                maxX = 6.55502f;
                minY = -3.55498f;
                maxY = 3.55173f;
                generators.Add(new Generator(0, new Position2D(-6.5f, 4.0f)));
                generators.Add(new Generator(1, new Position2D(-6.5f, 0.0f)));

                components.Add(new ComponentMachine(1, new Position2D(1.5f, -1.0f)));
                components.Add(new ComponentMachine(2, new Position2D(6.5f, 3.0f)));
                components.Add(new ComponentMachine(3, new Position2D(4.0f, 5.0f)));
                components.Add(new ComponentMachine(4, new Position2D(6.5f, -1.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 9
            {
                minX = -7.55501f;
                maxX = 7.55501f;
                minY = -3.55497f;
                maxY = 3.55497f;
                generators.Add(new Generator(0, new Position2D(-6.5f, 5.0f)));
                generators.Add(new Generator(1, new Position2D(-7.5f, 0.0f)));
                generators.Add(new Generator(2, new Position2D(6.5f, 5.0f)));
                generators.Add(new Generator(3, new Position2D(7.5f, 0.0f)));

                components.Add(new ComponentMachine(2, new Position2D(1.5f, 5.0f)));
                components.Add(new ComponentMachine(3, new Position2D(-2.0f, 0.0f)));
                components.Add(new ComponentMachine(4, new Position2D(-1.5f, 5.0f)));
                components.Add(new ComponentMachine(5, new Position2D(2.0f, 0.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 10
            {
                minX = -7.55499f;
                maxX = 7.55499f;
                minY = -3.55497f;
                maxY = 3.55499f;
                generators.Add(new Generator(0, new Position2D(-7.5f, 4.0f)));
                generators.Add(new Generator(1, new Position2D(-7.5f, 0.0f)));
                generators.Add(new Generator(2, new Position2D(7.5f, 4.0f)));
                generators.Add(new Generator(3, new Position2D(7.5f, 0.0f)));

                components.Add(new ComponentMachine(1, new Position2D(-4.5f, 5.0f)));
                components.Add(new ComponentMachine(2, new Position2D(4.5f, 0.0f)));
                components.Add(new ComponentMachine(3, new Position2D(3.0f, 5.0f)));
                components.Add(new ComponentMachine(4, new Position2D(-0.5f, 0.0f)));
                components.Add(new ComponentMachine(5, new Position2D(-4.0f, 0.0f)));
                
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 11
            {
                minX = -7.555f;
                maxX = 7.55092f;
                minY = -3.55499f;
                maxY = 3.55498f;
                generators.Add(new Generator(0, new Position2D(-6.5f, 4.0f)));
                generators.Add(new Generator(1, new Position2D(-7.5f, 1.0f)));
                generators.Add(new Generator(2, new Position2D(6.5f, 4.0f)));
                generators.Add(new Generator(3, new Position2D(7.5f, 1.0f)));

                components.Add(new ComponentMachine(1, new Position2D(-0.5f, 1.0f)));
                components.Add(new ComponentMachine(2, new Position2D(3.5f, 4.0f)));
                components.Add(new ComponentMachine(3, new Position2D(-2.0f, 5.0f)));
                components.Add(new ComponentMachine(4, new Position2D(-3.5f, 0.0f)));
                components.Add(new ComponentMachine(5, new Position2D(4.0f, -1.0f)));
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
            // Level 12
            {
                minX = -8.55499f;
                maxX = 8.555f;
                minY = -4.55497f;
                maxY = 0.55497f;
                generators.Add(new Generator(1, new Position2D(-8.5f, -1.0f)));
                generators.Add(new Generator(3, new Position2D(8.5f, -1.0f)));
                
                components.Add(new ComponentMachine(2, new Position2D(-3.5f, 1.0f)));
                components.Add(new ComponentMachine(3, new Position2D(-7.0f, 2.0f)));
                components.Add(new ComponentMachine(4, new Position2D(3.5f, 1.0f)));
                components.Add(new ComponentMachine(5, new Position2D(7.0f, 2.0f)));
                
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                // Level 13
                _layouts.Add(new Layout(minX, maxX, minY, maxY, new List<Generator>(generators), new List<ComponentMachine>(components)));
                generators.Clear();
                components.Clear();
            }
        }

    }
}
