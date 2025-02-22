# Creation: Voxel Engine

<p align="center">
 <img src ="https://github.com/user-attachments/assets/588bdab1-290c-4679-a7ce-f0068c76dc9c">
</p>

Unfinished c++ voxel engine that I wanted to make a game with. The codebase became a slog to work with which demotivated me to continue. 
I've decided to remake this but still unsure how.

# Features
- Optimized voxel mesh chunk system (No greedy meshing)
- Chunk render distance
- Procedural noise based terrain
- Block breaking and placing
- DDA Voxel traversal algorithm.
- Mouse picking.
- Centered mouse or free mouse by pressing the X button.
- Texture atlas generator.
- Data driven blocks, biomes and items using JSON.
- Enet server and client multiplayer.
- Billboarded sprites with look directions.
- BSP Dungeon generation with item spawning.
- 2D world generator with lattitude based biomes.
- Simple grid based physics.
- Distance based fog.
- Runs smoothly on a i5 7th gen intel cpu and intergrated graphics.

# Libraries used
- lua
- enet
- glad
- SDL2
- FastNoiseLight

# Requirements:
- w64devkit

# Textures from
- https://opengameart.org/content/16x16-block-texture-set

# Screenshots:
Block breaking and Placing:

![break_place](https://github.com/user-attachments/assets/7f4defe7-9211-46af-9dc3-4952c5174588)

Dungeon Algorithm:

![dungeon](https://github.com/user-attachments/assets/6f279748-d557-4ff0-8dc7-aa881c4b685c)

2D World generator with biomes:

![world](https://github.com/user-attachments/assets/48c8e2ff-8b1c-45cc-a4ac-14accf807f32)

