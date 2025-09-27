using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AwariaInterface
{
    internal class ComponentMachineManager
    {
        private List<ComponentMachine> _components;
        public ComponentMachineManager()
        {
            _components = new List<ComponentMachine>();
            this.reset();
        }
        public void setup(ref LevelLayout levelManager, int currentLevel)
        {
            _components = levelManager.getNormalizedComponents(currentLevel);
        }
        public void reset()
        {
            _components.Clear();
            for (int i = 0; i < 5; i++)
            {
                _components.Add(new ComponentMachine((int)component.repaired, new Position2D(-1000, -1000)));
            }
        }
        public void takeItem(string machineName)
        {
            component ID = (component)(machineName[machineName.Length - 1] - '0');
            if (ID == component.blueBattery)
                return;
            int foundIndex = -1;
            for (int i = 0; i < 5; i++)
            {
                if (_components[i]._ID == ID)
                    foundIndex = i;
            }
            if (foundIndex == -1)
                return;
            ComponentMachine temp = _components[foundIndex];
            temp._status = componentStatus.empty;
            _components[foundIndex] = temp;
        }
        public void placeItem(string machineName)
        {
            component ID = (component)(machineName[machineName.Length - 1] - '0');
            int foundIndex = -1;
            for (int i = 0; i < 5; i++)
            {
                if (_components[i]._ID == ID)
                    foundIndex = i;
            }
            if (foundIndex == -1)
                return;
            ComponentMachine temp = _components[foundIndex];
            temp._status = componentStatus.working;
            _components[foundIndex] = temp;
        }
        public void finishedWorking(string machineName)
        {
            component ID = (component)(machineName[machineName.Length - 1] - '0');
            int foundIndex = -1;
            for (int i = 0; i < 5; i++)
            {
                if (_components[i]._ID == ID)
                    foundIndex = i;
            }
            if (foundIndex == -1)
                return;
            ComponentMachine temp = _components[foundIndex];
            temp._status = componentStatus.ready;
            _components[foundIndex] = temp;
        }
        public List<ComponentMachine> getData()
        {
            return _components;
        }
    }
}
