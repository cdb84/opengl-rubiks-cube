OpenGL representation of a Rubik's cube, using Thumrongsak Kosiyatrakul's C implementation of Thistlethwaite's 52-move solving algorithm.

Only known issue: cannot move parts of cube if cube is oriented off of the starting view--the CTMS don't reflect the current viewing matrix. All moves most be done from the starting orientation of the cube in 3D space.

![](rubiks.webm)
