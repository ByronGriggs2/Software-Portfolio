# Training a Neural Network to Play a Skill Based Game
# Introduction
The goal of this dissertation project was to train a neural network to play the bullet hell game Awaria by Vanripper. Extensive documentation about the project can 
be found in the dissertation report, but the short version is that the non-fuzzy aspects of the design work great, while the neural network was found lacking. The
system can teach the network to solve some simple tasks, but nothing as complicated as completing a level of Awaria. 

# Instructions
If you don't care about the report and just want to launch the application or look at the code, you can follow the instructions in /Source Code/readme.txt, which is also included
below. "networkTest" is the only mode that will function without installing and modding Awaria, but Awaria is free on Steam. </br> </br>

The source code for this project is located in the following folders:
DLL_Mod (AwariaInterface.dll)
Main_Application2
vsdecompile (modified Assembly-CSharp.dll) </br>
////////////////////////////////////////////////////////////////////// </br>
For the system to function, Awaria must be modded. To mod Awaria with the provided code there are 2 options:

A. (Direct overwrite, Recommended) 
	1. Install Steam and install Awaria (free).
	2. Replace the folder ...steamapps/common/Awaria with the Awaria folder provided in this zip file.

B. (Using source code)
	1. Install Steam and install Awaria (free).
	2. Build the VS solutions found in DLL_Mod and vsdecompile.
	3. Run the pushMod batch script. </br>
///////////////////////////////////////////////////////////////////// </br>
Once Awaria has been modded, MainApplication2.exe can be compiled via its Visual Studio solution and launched with any of the valid command line arguments in "Main Application2.cpp". It is recommended to launch the application via Visual Studio by specifying command line arguments in [Main Application2]->[Properties]->[Configuration Properties]->[Debugging]. Release mode is recommended.

The important command line arguments for the system are as follows:
1. "startTraining" - creates a new NN and begins training with hyperparameters set in definitions.h

2. "resumeTraining <filename>" - continues training from a completed session state. The filename is the name of the DQN stored in Code/Main_Application2/Network_Backup

3. "resumeTraining <filename> error" - continues training from an error state.

4. "load <filename>" - loads the DQN stored in Code/Main_Application2/Network_Backup/filename and evaluates its winrate across 20 episodes. This zip file includes a copy of the DQN that was trained over 63 hours, and it can be loaded up using the argument "load network.txt". 

5. "networkTest" - The test used for Goal 2. Runtime of less than 10 seconds. 

Note that MainApplication.exe is to be launched before Awaria.exe. The console should inform you when it is ready to connect.