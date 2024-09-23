/******************************************************************************
 *                                                                            *
 *  Copyright (c) 2023 Ojaswa Sharma. All rights reserved.                    *
 *                                                                            *
 *  Author: Ojaswa Sharma                                                     *
 *  E-mail: ojaswa@iiitd.ac.in                                                *
 *                                                                            *
 *  This code is provided solely for the purpose of the CSE 333/533 course    *
 *  at IIIT Delhi. Unauthorized reproduction, distribution, or disclosure     *
 *  of this code, in whole or in part, without the prior written consent of   *
 *  the author is strictly prohibited.                                        *
 *                                                                            *
 *  This code is provided "as is", without warranty of any kind, express      *
 *  or implied, including but not limited to the warranties of                *
 *  merchantability, fitness for a particular purpose, and noninfringement.   *
 *                                                                            *
 ******************************************************************************/

#include "utils.h"

#define DRAW_QUADRATIC_BEZIER 1 // Use to switch Linear and quadratic bezier curves
#define SAMPLES_PER_BEZIER 10   // Sample each Bezier curve as N=10 segments and draw as connected lines

// Global variables
std::vector<float> controlPoints;
std::vector<float> linearBezier;
std::vector<float> quadraticBezier;
std::vector<float> c1controlpoints;
float selectionThresholdC1 = 0.1f; // Select any control point within 3 pixels of vicinity.
int width = 640, height = 640;
bool controlPointsUpdated = false;
bool controlPointsFinished = false;
int selectedControlPoint = -1;
int selectedC1ControlPoint = -1;

void calculatePiecewiseLinearBezier()
{
    // Since linearBezier is just a polyline, we can just copy the control points and plot.
    // However to show how a piecewise parametric curve needs to be plotted, we sample t and
    // evaluate all linear bezier curves.
    // linearBezier.assign(controlPoints.begin(), controlPoints.end());

    linearBezier.clear();
    int sz = controlPoints.size(); // Contains 3 points/vertex. Ignore Z
    float x[2], y[2];
    float delta_t = 1.0 / (SAMPLES_PER_BEZIER - 1.0);
    float t;
    for (int i = 0; i < (sz - 3); i += 3)
    {
        x[0] = controlPoints[i];
        y[0] = controlPoints[i + 1];
        x[1] = controlPoints[i + 3];
        y[1] = controlPoints[i + 4];
        linearBezier.push_back(x[0]);
        linearBezier.push_back(y[0]);
        linearBezier.push_back(0.0);
        t = 0.0;
        for (float j = 1; j < (SAMPLES_PER_BEZIER - 1); j++)
        {
            t += delta_t;
            linearBezier.push_back(x[0] + t * (x[1] - x[0]));
            linearBezier.push_back(y[0] + t * (y[1] - y[0]));
            linearBezier.push_back(0.0);
        }
        // No need to add the last point for this segment, since it will be added as first point in next.
    }
    // However, add last point of entire piecewise curve here (i.e, the last control point)
    linearBezier.push_back(x[1]);
    linearBezier.push_back(y[1]);
    linearBezier.push_back(0.0);
}

std::vector<float> calculateC1controlpoint()
{
    c1controlpoints.clear();
    int sz = controlPoints.size(); // Contains 3 points/vertex. Ignore Z

    if (sz < 6) // Need at least two segments
    {
        return c1controlpoints;
    }

    float x[3], y[3];
    for (int i = 0; i < (sz - 3); i += 3)
    {
        // Current segment points
        x[0] = controlPoints[i];
        y[0] = controlPoints[i + 1];
        x[1] = controlPoints[i + 3];
        y[1] = controlPoints[i + 4];

        float x1, y1;

        if (i == 0) // For the first segment
        {
            x1 = x[0] + (x[1] - x[0]) / 3.0;
            y1 = y[0] + (y[1] - y[0]) / 3.0 + 0.2;
        }
        else // For subsequent segments
        {
            // Reflect the previous control point across the shared point
            float prev_x1 = c1controlpoints[i - 3];
            float prev_y1 = c1controlpoints[i - 2];

            x1 = 2 * x[0] - prev_x1;
            y1 = 2 * y[0] - prev_y1;
        }

        c1controlpoints.push_back(x1);
        c1controlpoints.push_back(y1);
        c1controlpoints.push_back(0.0); // Z-coordinate
    }

    return c1controlpoints;
}

void calculatePiecewiseQuadraticBezier()
{
    quadraticBezier.clear();
    int sz = controlPoints.size(); // Contains 3 points/vertex. Ignore Z
    float x[3], y[3];
    float delta_t = 1.0 / (SAMPLES_PER_BEZIER - 1.0);
    float t;
    std::vector<float> c1controlpoints = calculateC1controlpoint();
    int j = 0;
    for (int i = 0; i < (sz - 3); i += 3)
    {
        x[0] = controlPoints[i];
        y[0] = controlPoints[i + 1];
        x[1] = c1controlpoints[j];
        y[1] = c1controlpoints[j + 1];
        j += 3;
        x[2] = controlPoints[i + 3];
        y[2] = controlPoints[i + 4];
        quadraticBezier.push_back(x[0]);
        quadraticBezier.push_back(y[0]);
        quadraticBezier.push_back(0.0);
        t = 0.0;
        for (float j = 1; j < (SAMPLES_PER_BEZIER - 1); j++)
        {
            t += delta_t;
            quadraticBezier.push_back((1 - t) * (1 - t) * x[0] + 2 * (1 - t) * t * x[1] + t * t * x[2]);
            quadraticBezier.push_back((1 - t) * (1 - t) * y[0] + 2 * (1 - t) * t * y[1] + t * t * y[2]);
            quadraticBezier.push_back(0.0);
        }
        // No need to add the last point for this segment, since it will be added as first point in next.
    }
    // However, add last point of entire piecewise curve here (i.e, the last control point)
    quadraticBezier.push_back(x[2]);
    quadraticBezier.push_back(y[2]);
    quadraticBezier.push_back(0.0);
}

bool searchNearestC1Controlpoint(float x, float y)
{
    // std::cout << "Searching nearest C1 control point" << std::endl;
    // std::cout << "Point x: " << x << " y: " << y << std::endl;
    float rescaled_x = -1.0 + ((1.0 * x - 0) / (width - 0)) * (1.0 - (-1.0));
    float rescaled_y = -1.0 + ((1.0 * (height - y) - 0) / (height - 0)) * (1.0 - (-1.0));
    // std::cout << "Rescaled Point x: " << rescaled_x << " y: " << rescaled_y << std::endl;
    size_t npts = c1controlpoints.size() / 3;
    if (npts > 0)
    {
        float _x, _y, dist2 = 0.0f;
        float thresh2 = selectionThresholdC1 * selectionThresholdC1;
        for (size_t i = 0; i < npts; i++)
        {
            _x = c1controlpoints[3 * i];
            _y = c1controlpoints[3 * i + 1];
            // std::cout << "C1 Control Point x: " << _x << " y: " << _y << std::endl;
            dist2 = (rescaled_x - _x) * (rescaled_x - _x) + (rescaled_y - _y) * (rescaled_y - _y);
            if (dist2 <= thresh2)
            {
                selectedC1ControlPoint = i;
                // std::cout << "Selected C1 control point: " << selectedC1ControlPoint << std::endl;
                return 1;
            }
        }
    }

    selectedC1ControlPoint = -1;
    return 0;
}

void editC1ControlPoint(std::vector<float> &points, float x, float y, int w, int h)
{
    std::cout << "Editing C1 control point" << std::endl;
    std::cout << "X: " << x << " Y: " << y << std::endl;

    if (selectedC1ControlPoint < 0)
        return;
    if (selectedC1ControlPoint >= points.size() / 3)
        return;

    float rescaled_x = -1.0 + ((1.0 * x - 0) / (w - 0)) * (1.0 - (-1.0));
    float rescaled_y = -1.0 + ((1.0 * (h - y) - 0) / (h - 0)) * (1.0 - (-1.0));

    std::cout << "Rescaled X: " << rescaled_x << " Y: " << rescaled_y << std::endl;

    c1controlpoints[selectedC1ControlPoint * 3] = rescaled_x;
    c1controlpoints[selectedC1ControlPoint * 3 + 1] = rescaled_y;
    c1controlpoints[selectedC1ControlPoint * 3 + 2] = 0.0; // Z-coordinate
}

int main(int, char *argv[])
{
    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO &io = ImGui::GetIO(); // Create IO object

    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
    glUseProgram(shaderProgram);

    // Create VBOs, VAOs
    unsigned int VBO_controlPoints, VBO_linearBezier, VBO_quadraticBezier, VBO_c1controlpoints;
    unsigned int VAO_controlPoints, VAO_linearBezier, VAO_quadraticBezier, VAO_c1controlpoints;
    glGenBuffers(1, &VBO_controlPoints);
    glGenVertexArrays(1, &VAO_controlPoints);
    glGenBuffers(1, &VBO_linearBezier);
    glGenVertexArrays(1, &VAO_linearBezier);
    glGenBuffers(1, &VBO_quadraticBezier);
    glGenVertexArrays(1, &VAO_quadraticBezier);
    glGenBuffers(1, &VBO_c1controlpoints);
    glGenVertexArrays(1, &VAO_c1controlpoints);

    int button_status = 0;

    // Display loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering
        showOptionsDialog(controlPoints, io);
        ImGui::Render();

        // Add a new point on mouse click
        float x, y;
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        if (!ImGui::IsAnyItemActive())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                x = io.MousePos.x;
                y = io.MousePos.y;
                if (!controlPointsFinished)
                {
                    // Add main control points
                    addControlPoint(controlPoints, x, y, width, height);
                    controlPointsUpdated = true;
                }
                else
                {
                    // Select main or C1 control points for editing
                    searchNearestControlPoint(x, y);
                    searchNearestC1Controlpoint(x, y);
                    // std::cout << "Selected control point: " << selectedControlPoint << std::endl;
                }
            }

            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && controlPointsFinished)
            {
                // Edit the selected control point
                if (selectedControlPoint >= 0)
                {
                    x = io.MousePos.x;
                    y = io.MousePos.y;
                    editControlPoint(controlPoints, x, y, width, height);
                    calculatePiecewiseQuadraticBezier();
                    controlPointsUpdated = true;
                }
                else if (selectedC1ControlPoint >= 0)
                {
                    x = io.MousePos.x;
                    y = io.MousePos.y;
                    editC1ControlPoint(c1controlpoints, x, y, width, height);
                    calculatePiecewiseQuadraticBezier();
                    controlPointsUpdated = true;
                }
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                // Finish adding points
                controlPointsFinished = true;
            }
        }

        if (controlPointsUpdated)
        {
            // Update VAO/VBO for control points (since we added a new point)
            glBindVertexArray(VAO_controlPoints);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_controlPoints);
            glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(GLfloat), &controlPoints[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0); // Enable first attribute buffer (vertices)

            // Update VAO/VBO for piecewise linear Bezier curve (since we added a new point)
            calculatePiecewiseLinearBezier();
            glBindVertexArray(VAO_linearBezier);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_linearBezier);
            glBufferData(GL_ARRAY_BUFFER, linearBezier.size() * sizeof(GLfloat), &linearBezier[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0); // Enable first attribute buffer (vertices)

            // Update VAO/VBO for piecewise quadratic Bezier curve
            calculatePiecewiseQuadraticBezier();
            glBindVertexArray(VAO_quadraticBezier);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_quadraticBezier);
            glBufferData(GL_ARRAY_BUFFER, quadraticBezier.size() * sizeof(GLfloat), &quadraticBezier[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0); // Enable first attribute buffer (vertices)

            // Update VAO/VBO for C1 control points
            glBindVertexArray(VAO_c1controlpoints);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_c1controlpoints);
            glBufferData(GL_ARRAY_BUFFER, c1controlpoints.size() * sizeof(GLfloat), &c1controlpoints[0], GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0); // Enable first attribute buffer (vertices)

            controlPointsUpdated = false; // Finish all VAO/VBO updates before setting this to false.
        }

        glUseProgram(shaderProgram);

        // Draw control points
        glBindVertexArray(VAO_controlPoints);
        glDrawArrays(GL_POINTS, 0, controlPoints.size() / 3); // Draw points

        // Draw C1 control points
        glBindVertexArray(VAO_c1controlpoints);
        glDrawArrays(GL_POINTS, 0, c1controlpoints.size() / 3); // Draw points

#if DRAW_QUADRATIC_BEZIER
        glBindVertexArray(VAO_quadraticBezier);
        glDrawArrays(GL_LINE_STRIP, 0, quadraticBezier.size() / 3); // Draw lines
#else
        // Draw linear Bezier
        glBindVertexArray(VAO_linearBezier);
        glDrawArrays(GL_LINE_STRIP, 0, linearBezier.size() / 3); // Draw lines
#endif

        glUseProgram(0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Delete VBO buffers
    glDeleteBuffers(1, &VBO_controlPoints);
    glDeleteBuffers(1, &VBO_linearBezier);
    glDeleteBuffers(1, &VBO_quadraticBezier);
    glDeleteBuffers(1, &VBO_c1controlpoints);

    // Cleanup
    cleanup(window);
    return 0;
}
