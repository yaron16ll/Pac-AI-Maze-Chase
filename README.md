# Pac-AI-Maze-Chase
This project implements a **Pac-Man game in a two-dimensional maze** where the gameplay is entirely **AI vs. AI** with no human interaction.
Pac-Man must navigate the maze, collect coins, and avoid three ghosts. The ghosts determine their chase path using the **A\*** search algorithm to efficiently pursue Pac-Man.
Pac-Man’s movement is handled by a **bounded-depth BFS variation**: when no ghosts are nearby, Pac-Man seeks out the nearest coin, but when a ghost is detected, he moves in the opposite direction of the closest ghost to maximize survival. This setup demonstrates the use of classic search algorithms for autonomous decision-making in a dynamic game environment.


## Demo Video
https://github.com/user-attachments/assets/aa884b2d-08a2-44e2-af34-0c80cd3a7992

