## Interpolating Piecewise Quadratic Bezier Curves with G1 Continuity

## Overview
This project implements interpolating piecewise quadratic Bezier curves with G1 continuity using C++, OpenGL, and ImGui. The program allows users to interactively create and modify curves while maintaining smooth transitions between curve segments.

## Features
- Interactive addition of control points
- Real-time calculation of Bezier curves
- Enforcement of G1 continuity between curve segments
- Dynamic curve updates based on user modifications of control points
- Visualization of control points and resulting curves

## Requirements
- C++
- OpenGL
- ImGui (for user interaction)

## Implementation Details

### 1. C1 Control Points Calculation
The `calculateC1controlpoint()` function computes the C1 control points to maintain G1 continuity:
- Clears previous control points
- Ensures a minimum of two segments
- Computes control points for the first and subsequent segments
- Stores results in the `c1controlpoints` vector

### 2. Bezier Curve Calculation
The `calculatePiecewiseQuadraticBezier()` function generates the points for the piecewise quadratic Bezier curve:
- Clears previous curve data
- Samples points using the quadratic Bezier formula:
  B(t) = (1-t)²x₀ + 2(1-t)tx₁ + t²x₂
- Stores computed curve points in the `quadraticBezier` vector

### 3. User Interaction
- Add control points by clicking on the canvas
- Right-click to stop adding points
- Move control points by clicking and dragging
- Real-time curve recalculation based on control point updates

### 4. Rendering
- Uses OpenGL for rendering points and curves
- Employs VAO (Vertex Array Object) and VBO (Vertex Buffer Object) for vertex data management
- Draws curves using `glDrawArrays()` in line strip mode

## Data Structures
- Control Points: `std::vector<float>`
- Bezier Curve Points: `std::vector<float>` (for both C1 control points and quadratic Bezier curve points)

## Usage
1. Compile the project with your preferred C++ compiler, linking against the required libraries (OpenGL, ImGui).
2. Run the executable.
3. Click on the canvas to add control points.
4. Right-click to finish adding points.
5. Click and drag control points to modify the curve.
6. Observe real-time updates of the curve with maintained G1 continuity.

## Key Functions
- `calculateC1controlpoint()`: Calculates C1 control points for G1 continuity
- `calculatePiecewiseQuadraticBezier()`: Generates points for the piecewise quadratic Bezier curve

## Future Improvements
- Add interactive tangent modification
- Allow users to change curve properties
- Implement more advanced curve editing features

## Note
This project was completed as part of an assignment at Indraprastha Institute of Information Technology, Delhi. It demonstrates the implementation of piecewise quadratic Bezier curves with G1 continuity and real-time user interaction.
For more information you may view my [report](https://github.com/FakePickle/Computer-Graphics/blob/main/Assignment01/CG_Assignment_1.pdf)