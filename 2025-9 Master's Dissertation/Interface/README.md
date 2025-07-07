# Game Interface
The purpose of the game interface is to autonomously send control inputs to Awaria.exe from another application while getting game state updates in return each frame.
Ideally the implementation would also modify the game to the least extent possible, both for ease of programming and to minimise the risk of altering the gameplay.
This was accomplished by creating a .dll file to run in Awaria.exe, which contained a class with a variety of setter functions, and 1 getter function for the control inputs.
The game code was then modified to call the setter functions periodically to update the .dll's 230 byte representation of the current frame's game state.
As the game's state is decentralised in the game code, this unfortunately did require modification of many different files, and not always in the same way.
However, despite the extensive modification, the .dll and modifications are still not capable of directly changing the game state.
![](Interface_Diagram)
