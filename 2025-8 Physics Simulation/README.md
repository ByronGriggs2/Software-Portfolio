# Networked Antigravity Chamber
## Introduction
This was my final project for the Simulation and Concurrency course of my master's programme. The goal was to simulate the motion and collision of various objects while leveraging multithreading as well as networking using sockets.

My mark for the course was 75% (An overall mark/grade of 70% is distinction/honours in the UK).
## Video demonstration 
As part of the project, I was asked to record a short demonstration that closely followed the grading rubric. You can view it here: https://youtu.be/IeWwTFaKpDw

## Diagrams
Class diagram
![Class diagram](https://github.com/user-attachments/assets/ecbd7590-9df2-4c15-8535-cd21ef35d2ca)
Sequence diagram
![Sequence diagram](https://github.com/user-attachments/assets/f78261f0-d474-40d3-96ca-9c2e01de460a)
## Grading rubric
Core Functionality (marked out of 10)	77%  
Newtonian Physics	   
Objects are treated as particles with dimensions. They do not rotate.	10  
When gravity is applied, objects should move according to the appropriate gravitational force.	10  
Linear Newtonian physics is simulated with a fixed timestep using Euler integration.	10  
Collision Detection	   
Collision detection between moving spheres and fixed spheres.	10  
Collision detection between moving spheres and fixed capsules or fixed cylinders.	10  
Collision detection between moving spheres and moving spheres.	10  
Collision detection between moving spheres and room	10  
Collision Response	   
Inelastic collision response between moving spheres and fixed objects (including room)	10  
Inelastic collision response between moving spheres and moving spheres with the same mass.	10  
Scenarios	   
1. Add moving spheres one by one from random points above a scene filled with several fixed spheres with different positions and radii.	10  
2. Add moving spheres one by one from random points above a scene filled with several fixed capsules or cylinders with different positions, radii, lengths and orientations.	10  
3. Add many balls from random points in a scene with no fixed objects (apart from the surrounding cube). Moving spheres should be of different radii.	9  
4. Add many balls from random points above a scene filled with several fixed spheres with different positions and radii, and fixed capsules or cylinders with different positions, radii, lengths and orientations.  	9
At least two custom test scenarios for detailed collisions	10  
Distributed	   
Peer to peer architecture (2 peers)	10  
Randomly assign objects to peers and colour code them	10  
Global state for shapes	0  
Global state for the controls specified in the UI (e.g. frequencies, time, etc)	0  
Data integrity (ie. How much if any corruption of the data occurs due to the distributed architecture)	0  
Network performance	0  
Parallel	   
Graphics threads (main thread)	10  
Networking threads	10  
Simulation threads	10  
Frequency adjustment of graphics	10  
Frequency adjustment of networking	10  
Frequency adjustment of simulation	10  
Processor affinity	10  
User Interface Controls (marks only available if the controls use live data)	   
Display the total number of objects within the simulation, by type (*).	5  
Display target delta time within the simulation step (in secs), minimum 0.005s [changeable in 0.005s intervals] (*)	5  
Display/adjust target frequency of the simulation (in Hz), minimum 1Hz [changeable in 1Hz intervals] (*)	5  
Display actual frequency of the simulation (in Hz),	10  
Display/adjust frequency of the graphics (in Hz), minimum 1Hz [changeable in 1Hz intervals]	10  
Display actual frequency of the graphics (in Hz),	10  
Display/adjust target frequency of the networking (in Hz), minimum 1Hz [changeable in 1Hz intervals] (*)	5  
Display actual frequency of the networking (in Hz)	10  
Interact to pause the simulation (*)	5  
Interact to turn on or off gravity (*)	5  
Interact to reverse gravity (*)	5  
Selection and changing of scenarios at runtime	5  
 	 
Advanced Functionality (marked out of 10)	44%  
Newtonian Physics	   
Include at least 2 additional integration methods and be able to switch between integration methods locally at any time.	10  
Moving objects have angular velocity.	0  
Collision Detection	   
Add collision detection for fixed cubes	0  
Add a method for spatial segmentation to reduce unnecessary calculation	0  
Collision Response	   
Spheres have different masses, where the collision response between moving spheres takes into account the mass of each sphere.	10  
Objects have materials with elasticity. Collision response between moving spheres takes into account elasticity between different pairs of materials.	8  
Moving spheres should be modelled as rigid bodies and therefore rotation should be fully modelled within the physics equations, including inertia and friction.	0  
Moving capsules or cylinders should be modelled as rigid bodies and therefore rotation should be included within the physics equations along with inertia and friction.	0  
Moving cubes modelled as rigid bodies and therefore rotation should be included within the physics equations along with inertia and friction.	0  
Distributed	   
Peer to peer architecture (4 peers)	10  
Correction algorithms to fix errors in shape position/orientation, e.g. interpolation, extrapolation.	3  
Robustness of networking due to packet loss and/or latency	0  
Automated peer location system, using UDP broadcast mechanism	10  
Parallel	   
Elements of the simulation can be implemented on the GPU using Compute Shaders	0  
User Interface Controls (marks only available if the controls use live data)	   
User interface elements to set the coefficient of elasticity for pairs of materials. (*)	0  
User interface elements to set static and dynamic friction for pairs of materials. (*)	0  
User interface elements to set the density of materials. (*)	0  
Advanced Features attempted	26  
 	 
Lab book (marked out of 3)	100%  
Lab 1 - SJG	3  
Lab 2 - SJG	3  
Lab 3 - SJG	3  
Lab 4 - SJG	3  
Lab 5 - SJG	3  
Lab 6 - SJG	   
Lab A - WJV	3  
Lab B - WJV	3  
Lab C - WJV	3  
Lab D - WJV	3  
Lab E - WJV	3  
Lab F - WJV	3  
Best 4 marks from SJG Labs	12  
Best 4 marks from WJV Labs	12  
Lab I (final) - PBM	3  
Lab I (final) - networking and concurrency	3  
 	 
Deliverable (marked out of 10)	100%  
Video	10  
Quality of demonstration	10  
 	 
Core Functionality (marked out of 10)	77%  
Advanced Functionality (marked out of 10)	44%  
Lab book (marked out of 3)	100%  
Deliverable (marked out of 10)	100%  
Total	75%
