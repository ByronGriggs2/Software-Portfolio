# Game Interface
As the game has already been thoroughly tested and part of the goal of the project is to change it as little as possible, I developed a server/client architecture to isolate the game from my code. The client runs as a .dll inside of Awaria.exe, and communicates via pipes with an external application.

The external application contains the neural network and training algorithm, and is implemented in C++ rather than C#. This is just so that I can code in a familiar environment.

The client is expected to send game state data (e.g. player position) to the server, while the server is expected to send inputs. As of 9th June, only the inputs have been implemented, and the communication is thus one way.

![one way server client](https://github.com/user-attachments/assets/b7002a8d-ce3d-415a-b0d0-7235de3b38bc)
