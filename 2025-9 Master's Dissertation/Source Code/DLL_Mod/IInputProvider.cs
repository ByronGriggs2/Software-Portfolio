using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace AwariaInterface
{
    public interface IInputProvider
    {
        Vector2 getMoveVector();
        bool getPrimary();
        bool getSecondary();
        bool getCancel();
    }
}
