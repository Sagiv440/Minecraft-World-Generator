Minecraft-World-Generator
===

This is a A prototype I made to learn about 3D engines and the rendering pipeline. 
It is a small application that procedurally generates terrain similar to the terrain in the game Minecraft. 

# Bulit points
- Terrain is divided into small chunks of 100x100x256 blocks. that are stored in memory using a key-value pair (key: chunk position, value: memory address).
- World rendering is done using the OpenGL rendering API and shader scripts that dictate the flow of data to and from the GPU.
- Custom game engine handles data sorting and provides simple first-person controls to explore the generated terrain.
- The application uses a Perlin noise algorithm to generate the continuous terrain

# Dependence
To compile the project, you will need to link the following libraries:
- GLFW, GLEW for OpenGL,
- libnoise for the Randomizer,
- SOIL2 for handling images.

  # Screen Shots
![Image_1](https://github.com/Sagiv440/Minecraft-World-Generator/blob/main/screenshot/Screenshot%20from%202024-04-11%2013-04-45.png?raw=true)
![Image_2](https://github.com/Sagiv440/Minecraft-World-Generator/blob/main/screenshot/Screenshot%20from%202024-04-11%2013-06-53.png?raw=true)
![Image 3](https://github.com/Sagiv440/Minecraft-World-Generator/blob/main/screenshot/Screenshot%20from%202024-04-11%2013-08-55.png?raw=true)


