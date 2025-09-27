using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace AwariaInterface
{
    internal class GeneratorManager
    {
        private List<Generator> _generators;
        public GeneratorManager()
        {
            _generators = new List<Generator>();
            this.reset();
        }
        public void setup(ref LevelLayout levelManager, int currentLevel)
        {
            _generators = levelManager.getNormalizedGenerators(currentLevel);
        }
        public void reset()
        {
            _generators.Clear();
            for (int i = 0; i < 4; i++)
            {
                _generators.Add(new Generator(-1, new Position2D(-1000, -1000)));
            }
        }
        public void myFixedUpdate(float deltaTime)
        {
            for (int i = 0; i < _generators.Count; i++)
            {
                Generator temp = _generators[i];
                if (temp._time > 0)
                {
                    temp._time -= (deltaTime) / 30.0f;
                }
                _generators[i] = temp;
            }
        }
        public void damageGenerator(string generatorName)
        {
            int index = (generatorName[generatorName.Length - 1] - '0') - 1;
            Generator temp = _generators[index];
            temp._broken = true;
            temp._want1 = 0;
            temp._want2 = 0;
            temp._time = 1.0f;
            _generators[index] = temp;
        }
        public void checkDamagedGenerator(string generatorName, int want1, int want2)
        {
            int index = (generatorName[generatorName.Length - 1] - '0') - 1;
            if (!_generators[index]._broken)
                return;
            Generator temp = _generators[index];
            temp._want1 = want1;
            temp._want2 = want2;
            _generators[index] = temp;
        }
        public void fixGenerator(string generatorName)
        {
            int index = (generatorName[generatorName.Length - 1] - '0') - 1;
            if (!_generators[index]._broken)
                return;
            Generator temp = _generators[index];
            temp._want1 = 10;
            temp._want2 = 10;
            temp._broken = false;
            temp._time = -1.0f;
            _generators[index] = temp;
        }
        public List<Generator> getData()
        {
            return _generators;
        }
    }
}
