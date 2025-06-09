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
Core Functionality (marked out of 10)	77% </break>
Newtonian Physics	 </break>
Objects are treated as particles with dimensions. They do not rotate.	10</break>
When gravity is applied, objects should move according to the appropriate gravitational force.	10</break>
Linear Newtonian physics is simulated with a fixed timestep using Euler integration.	10</break>
Collision Detection	 </break>
Collision detection between moving spheres and fixed spheres.	10</break>
Collision detection between moving spheres and fixed capsules or fixed cylinders.	10</break>
Collision detection between moving spheres and moving spheres.	10</break>
Collision detection between moving spheres and room	10</break>
Collision Response	 </break>
Inelastic collision response between moving spheres and fixed objects (including room)	10</break>
Inelastic collision response between moving spheres and moving spheres with the same mass.	10</break>
Scenarios	 </break>
1. Add moving spheres one by one from random points above a scene filled with several fixed spheres with different positions and radii.	10</break>
2. Add moving spheres one by one from random points above a scene filled with several fixed capsules or cylinders with different positions, radii, lengths and orientations.	10</break>
3. Add many balls from random points in a scene with no fixed objects (apart from the surrounding cube). Moving spheres should be of different radii.	9</break>
4. Add many balls from random points above a scene filled with several fixed spheres with different positions and radii, and fixed capsules or cylinders with different positions, radii, lengths and orientations.</break>	9
At least two custom test scenarios for detailed collisions	10</break>
Distributed	 </break>
Peer to peer architecture (2 peers)	10</break>
Randomly assign objects to peers and colour code them	10</break>
Global state for shapes	0</break>
Global state for the controls specified in the UI (e.g. frequencies, time, etc)	0</break>
Data integrity (ie. How much if any corruption of the data occurs due to the distributed architecture)	0</break>
Network performance	0</break>
Parallel	 </break>
Graphics threads (main thread)	10</break>
Networking threads	10</break>
Simulation threads	10</break>
Frequency adjustment of graphics	10</break>
Frequency adjustment of networking	10</break>
Frequency adjustment of simulation	10</break>
Processor affinity	10</break>
User Interface Controls (marks only available if the controls use live data)	 </break>
Display the total number of objects within the simulation, by type (*).	5</break>
Display target delta time within the simulation step (in secs), minimum 0.005s [changeable in 0.005s intervals] (*)	5</break>
Display/adjust target frequency of the simulation (in Hz), minimum 1Hz [changeable in 1Hz intervals] (*)	5</break>
Display actual frequency of the simulation (in Hz),	10</break>
Display/adjust frequency of the graphics (in Hz), minimum 1Hz [changeable in 1Hz intervals]	10</break>
Display actual frequency of the graphics (in Hz),	10</break>
Display/adjust target frequency of the networking (in Hz), minimum 1Hz [changeable in 1Hz intervals] (*)	5</break>
Display actual frequency of the networking (in Hz)	10</break>
Interact to pause the simulation (*)	5</break>
Interact to turn on or off gravity (*)	5</break>
Interact to reverse gravity (*)	5</break>
Selection and changing of scenarios at runtime	5</break>
 	 
Advanced Functionality (marked out of 10)	44%</break>
Newtonian Physics	 </break>
Include at least 2 additional integration methods and be able to switch between integration methods locally at any time.	10</break>
Moving objects have angular velocity.	0</break>
Collision Detection	 </break>
Add collision detection for fixed cubes	0</break>
Add a method for spatial segmentation to reduce unnecessary calculation	0</break>
Collision Response	 </break>
Spheres have different masses, where the collision response between moving spheres takes into account the mass of each sphere.	10</break>
Objects have materials with elasticity. Collision response between moving spheres takes into account elasticity between different pairs of materials.	8</break>
Moving spheres should be modelled as rigid bodies and therefore rotation should be fully modelled within the physics equations, including inertia and friction.	0</break>
Moving capsules or cylinders should be modelled as rigid bodies and therefore rotation should be included within the physics equations along with inertia and friction.	0</break>
Moving cubes modelled as rigid bodies and therefore rotation should be included within the physics equations along with inertia and friction.	0</break>
Distributed	 </break>
Peer to peer architecture (4 peers)	10</break>
Correction algorithms to fix errors in shape position/orientation, e.g. interpolation, extrapolation.	3</break>
Robustness of networking due to packet loss and/or latency	0</break>
Automated peer location system, using UDP broadcast mechanism	10</break>
Parallel	 </break>
Elements of the simulation can be implemented on the GPU using Compute Shaders	0</break>
User Interface Controls (marks only available if the controls use live data)	 </break>
User interface elements to set the coefficient of elasticity for pairs of materials. (*)	0</break>
User interface elements to set static and dynamic friction for pairs of materials. (*)	0</break>
User interface elements to set the density of materials. (*)	0</break>
Advanced Features attempted	26</break>
 	 
Lab book (marked out of 3)	100%</break>
Lab 1 - SJG	3</break>
Lab 2 - SJG	3</break>
Lab 3 - SJG	3</break>
Lab 4 - SJG	3</break>
Lab 5 - SJG	3</break>
Lab 6 - SJG	 </break>
Lab A - WJV	3</break>
Lab B - WJV	3</break>
Lab C - WJV	3</break>
Lab D - WJV	3</break>
Lab E - WJV	3</break>
Lab F - WJV	3</break>
Best 4 marks from SJG Labs	12</break>
Best 4 marks from WJV Labs	12</break>
Lab I (final) - PBM	3</break>
Lab I (final) - networking and concurrency	3</break>
 	 
Deliverable (marked out of 10)	100%</break>
Video	10</break>
Quality of demonstration	10</break>
 	 
Core Functionality (marked out of 10)	77%</break>
Advanced Functionality (marked out of 10)	44%</break>
Lab book (marked out of 3)	100%</break>
Deliverable (marked out of 10)	100%</break>
Total	75%
