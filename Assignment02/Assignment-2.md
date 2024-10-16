## Transformations, Viewing, and Projections

## Overview

This project implements various 3D transformations, viewing techniques, and projections on a cube using C++ and OpenGL. It demonstrates matrix construction for rotation about an arbitrary axis, development of a custom lookAt function, and application of different perspective views.

## Features

- Rotation of a 3D cube about an arbitrary axis
- Custom implementation of the lookAt function for camera views
- Various perspective projections of the cube:
    1. One-point perspective
    2. Two-point perspective
    3. Three-point (bird's eye) perspective
    4. Three-point (rat's eye) perspective
- Comparison with GLM's built-in functions

## Files

- `main.cpp`: Contains the main application logic, OpenGL setup, and rendering loop (not provided in the report, but assumed to exist)
- Other necessary shader files (not mentioned in the report, but likely required for the project)

## Requirements

- C++
- OpenGL
- GLM (OpenGL Mathematics library)
- ImGui (for user interaction, mentioned in the introduction)

## Implementation Details

### 1. Rotation Matrix Derivation and Implementation

- Derives a 4x4 rotation matrix for rotating 30 degrees counterclockwise about the axis (1, 2, 2)
- Implements the rotation using a change of basis approach
- Function `createRotationMatrix()` constructs the custom rotation matrix

### 2. Custom LookAt Function

- Implements a custom `lookAt` function to create view matrices
- Calculates view matrix using eye position, gaze direction, and up vector
- Verifies the custom implementation against GLM's built-in `lookAt()` function

### 3. Perspective Views

- Implements four different perspective views of the cube:
    - One-point perspective
    - Two-point perspective
    - Three-point (bird's eye) perspective
    - Three-point (rat's eye) perspective
- Uses custom `lookAt` function with different camera parameters for each view

## Usage

1. Compile the project with your preferred C++ compiler, linking against the required libraries (OpenGL, GLM, ImGui).
2. Run the executable.
3. The program will display:
    - A rotated cube (30 degrees about axis (1, 2, 2))
    - Comparison of custom `lookAt` function with GLM's `lookAt()`
    - Four different perspective views of the cube

## Key Functions

- `createRotationMatrix()`: Creates the custom rotation matrix
- `customLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)`: Custom implementation of the lookAt function

## Note

This project was completed as part of an assignment at Indraprastha Institute of Information Technology, Delhi. The code demonstrates various computer graphics concepts including matrix transformations, viewing transformations, and perspective projections.
For more information you may view my [report](https://github.com/FakePickle/Computer-Graphics/blob/main/Assignment02/CG_Assignment_2.pdf)