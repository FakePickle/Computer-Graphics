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

//Assignment 03: Lighting and shading

/*References
  Trackball: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/

#include "utils.h"

#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//Globals
int screen_width = 640, screen_height=640;
GLint vModel_uniform, vView_uniform, vProjection_uniform;
GLint vColor_uniform;
glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations

double oldX, oldY, currentX, currentY;
bool isDragging=false;

size_t createSphereObject(unsigned int &, unsigned int &);

void setupModelTransformation(unsigned int &);
void setupViewTransformation(unsigned int &);
void setupProjectionTransformation(unsigned int &);
glm::vec3 getTrackBallVector(double x, double y);

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(screen_width, screen_height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");

    //Get handle to color variable in shader
    vColor_uniform = glGetUniformLocation(shaderProgram, "vColor");
    if(vColor_uniform == -1){
        fprintf(stderr, "Could not bind location: vColor\n");
        exit(0);
    }

    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram);

    size_t IBO_nelems = createSphereObject(shaderProgram, VAO);
		fprintf(stderr, "Size of IBO %lu\n", IBO_nelems);

    oldX = oldY = currentX = currentY = 0.0;
    int prevLeftButtonState = GLFW_RELEASE;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Get current mouse position
        int leftButtonState = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT);
        double x,y;
        glfwGetCursorPos(window,&x,&y);
        if(leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_RELEASE){
            isDragging = true;
            currentX = oldX = x;
            currentY = oldY = y;
        }
        else if(leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_PRESS){
            currentX = x;
            currentY = y;
        }
        else if(leftButtonState == GLFW_RELEASE && prevLeftButtonState == GLFW_PRESS){
            isDragging = false;
        }

        // Rotate based on mouse drag movement
        prevLeftButtonState = leftButtonState;
        if(isDragging && (currentX !=oldX || currentY != oldY))
        {
            glm::vec3 va = getTrackBallVector(oldX, oldY);
            glm::vec3 vb = getTrackBallVector(currentX, currentY);

            float angle = acos(std::min(1.0f, glm::dot(va,vb)));
            glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
            glm::mat3 camera2object = glm::inverse(glm::mat3(viewT*modelT));
            glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
            modelT = glm::rotate(modelT, angle, axis_in_object_coord);
            glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));

            oldX = currentX;
            oldY = currentY;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);

        {
            ImGui::Begin("Information");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);

        glUniform3f(vColor_uniform, 0.5, 0.5, 0.5);
				glDrawElements(GL_TRIANGLES, IBO_nelems, GL_UNSIGNED_SHORT, 0);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;
}

size_t createSphereObject(unsigned int &program, unsigned int &sphere_VAO)
{
    glUseProgram(program);
    // Bind shader variables
    int vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }

    // Sphere data: use spherical parameterization to produce points
    #define DELTA_ANGLE 18
    int nTheta = 180/DELTA_ANGLE + 1;
    int nPhi = 360/DELTA_ANGLE + 1;
    GLfloat *sphere_vertices = new GLfloat[nTheta*nPhi*3];
    GLfloat *sphere_normals = new GLfloat[nTheta*nPhi*3];  // We'll calculate normals, even if not used in shaders
    float theta, phi, x, y, z;
    float radius = 10.0f;

    for (int j = 0; j < nTheta; j++) {
        for(int i = 0; i < nPhi; i++) {
            theta = float(j*DELTA_ANGLE)*M_PI/180.0;
            phi = float(i*DELTA_ANGLE)*M_PI/180.0;
            x = sinf(theta)*cosf(phi);
            y = sinf(theta)*sinf(phi);
            z = cosf(theta);

            // Vertex positions
            sphere_vertices[(i + j*nPhi)*3 + 0] = radius*x;
            sphere_vertices[(i + j*nPhi)*3 + 1] = radius*y;
            sphere_vertices[(i + j*nPhi)*3 + 2] = radius*z;

            // Normals (unit vector from center to surface point)
            sphere_normals[(i + j*nPhi)*3 + 0] = x;
            sphere_normals[(i + j*nPhi)*3 + 1] = y;
            sphere_normals[(i + j*nPhi)*3 + 2] = z;
        }
    }

    // Generate index array (unchanged)
    GLushort *sphere_indices = new GLushort[2*(nTheta-1)*(nPhi-1)*3];
    for(int j = 0; j < (nTheta-1); j++) {
        for(int i = 0; i < (nPhi-1); i++) {
            // Upper triangle
            sphere_indices[(i + j*(nPhi-1))*6 + 0] = i + j*nPhi;
            sphere_indices[(i + j*(nPhi-1))*6 + 1] = i + (j+1)*nPhi;
            sphere_indices[(i + j*(nPhi-1))*6 + 2] = i + 1 + j*nPhi;
            // Lower triangle
            sphere_indices[(i + j*(nPhi-1))*6 + 3] = i + 1 + j*nPhi;
            sphere_indices[(i + j*(nPhi-1))*6 + 4] = i + (j+1)*nPhi;
            sphere_indices[(i + j*(nPhi-1))*6 + 5] = i + 1 + (j+1)*nPhi;
        }
    }

    // Generate VAO object
    glGenVertexArrays(1, &sphere_VAO);
    glBindVertexArray(sphere_VAO);

    // Create VBOs for the VAO
    GLuint vertex_VBO, normal_VBO;

    // Vertex VBO
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nTheta*nPhi*3*sizeof(GLfloat), sphere_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal VBO (we'll create it even if not used in shaders)
    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, nTheta*nPhi*3*sizeof(GLfloat), sphere_normals, GL_STATIC_DRAW);

    // Create IBO
    GLuint indices_IBO;
    glGenBuffers(1, &indices_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_IBO);
    size_t IBO_nelems = (nTheta-1)*(nPhi-1)*6; // Number of elements in the IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IBO_nelems*sizeof(GLushort), sphere_indices, GL_STATIC_DRAW);

    // Clean up
    delete[] sphere_vertices;
    delete[] sphere_normals;
    delete[] sphere_indices;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind the VAO to disable changes outside this function.

    return IBO_nelems;
}

void setupModelTransformation(unsigned int &program)
{
    //Modelling transformations (Model -> World coordinates)
    modelT = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));//Model coordinates are the world coordinates

    //Pass on the modelling matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if(vModel_uniform == -1){
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}


void setupViewTransformation(unsigned int &program)
{
    //Viewing transformations (World -> Camera coordinates
    //Camera at (0, 0, 100) looking down the negative Z-axis in a right handed coordinate system
    viewT = glm::lookAt(glm::vec3(40.0, -40.0, 40.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    //Pass-on the viewing matrix to the vertex shader
    glUseProgram(program);
    vView_uniform = glGetUniformLocation(program, "vView");
    if(vView_uniform == -1){
        fprintf(stderr, "Could not bind location: vView\n");
        exit(0);
    }
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
}

void setupProjectionTransformation(unsigned int &program)
{
    //Projection transformation
    projectionT = glm::perspective(45.0f, (GLfloat)screen_width/(GLfloat)screen_height, 0.1f, 1000.0f);

    //Pass on the projection matrix to the vertex shader
    glUseProgram(program);
    vProjection_uniform = glGetUniformLocation(program, "vProjection");
    if(vProjection_uniform == -1){
        fprintf(stderr, "Could not bind location: vProjection\n");
        exit(0);
    }
    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

glm::vec3 getTrackBallVector(double x, double y)
{
	glm::vec3 p = glm::vec3(2.0*x/screen_width - 1.0, 2.0*y/screen_height - 1.0, 0.0); //Normalize to [-1, +1]
	p.y = -p.y; //Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

	float mag2 = p.x*p.x + p.y*p.y;
	if(mag2 <= 1.0f)
		p.z = sqrtf(1.0f - mag2);
	else
		p = glm::normalize(p); //Nearest point, close to the sides of the trackball
	return p;
}

