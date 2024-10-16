## Lighting and Shading

## Overview

This project implements various lighting and shading techniques using OpenGL and GLSL. It creates a 3D sphere object and applies different lighting models to demonstrate various illumination effects.

## Features

- Spherical object generation using parametric equations
- Multiple lighting and shading modes:
    1. Colored Normals
    2. Gouraud Shading using Phong Lighting
    3. Phong Shading using Phong Lighting
    4. Spotlight Source
    5. Spotlight with Outer and Inner Cone
    6. Headlight with Moving Camera
- Interactive camera movement (WASD keys for movement, QE for up/down)
- Trackball-style rotation of the object
- ImGui-based user interface for mode selection

## Files

1. `main.cpp`: Contains the main application logic, OpenGL setup, and rendering loop.
2. `vshader.vs`: Vertex shader implementing vertex transformations and Gouraud shading.
3. `fshader.fs`: Fragment shader implementing various lighting models and effects.

## Requirements

- OpenGL 3.3+
- GLFW
- GLM
- ImGui

## Controls

- Left Mouse Button: Rotate the sphere
- W/A/S/D: Move camera forward/left/backward/right
- Q/E: Move camera down/up

## Lighting Modes

1. **Colored Normals**: Displays the surface normals as colors.
2. **Gouraud Shading**: Implements Phong lighting calculated per-vertex.
3. **Phong Shading**: Implements Phong lighting calculated per-fragment.
4. **Spotlight**: Implements a spotlight effect.
5. **Spotlight with Outer and Inner Cone**: Adds smooth edges to the spotlight effect.
6. **Headlight**: Attaches the light source to the camera position.

## Usage

1. Compile the project with your preferred C++ compiler, linking against the required libraries.
2. Run the executable.
3. Use the ImGui interface to switch between different lighting modes.
4. Interact with the sphere using mouse and keyboard controls.

## Implementation Details

- The sphere is generated using spherical coordinates and tessellation.
- Vertex and fragment shaders implement the core lighting calculations.
- The main application handles user input, updates uniform variables, and manages the rendering loop.

## Note

This code is provided for educational purposes as part of the CSE 333/533 course at IIIT Delhi. Unauthorized reproduction or distribution is prohibited.
For more information you may view my attached [report](https://github.com/FakePickle/Computer-Graphics/blob/main/Assignment03/CG_Assignment_3.pdf)