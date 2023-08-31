#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "control.hpp"

glm::vec3 position = glm::vec3(0, 0, 5);
float horizontalAngle = 3.14f; // Towards -Z
float verticalAngle = 0.0f;
float initialFoV = 80.0f;

float speed = 0.1;
float mouseSpeed = 0.003f;
glm::mat4 ViewMatrix = glm::lookAt(
        vec3(0,0,0),
        vec3(0,0,-1),
        vec3(0,1,0));
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
    return ProjectionMatrix;
}


void computeMatricesFromInputs(GLFWwindow* window) {
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    auto deltaTime = 0.5f;//float(currentTime - lastTime);
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, width/2.0, height/2.0);

    horizontalAngle += mouseSpeed * deltaTime * float(width/2.0 - xpos);
    verticalAngle += mouseSpeed * deltaTime * float(height/2.0 - ypos);

    verticalAngle = clamp((double)verticalAngle, -3.14/2, 3.14/2.0);

    // Spherical to Cartesian direction
    vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos (horizontalAngle));

    vec3 right = vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f));

    vec3 up = glm::cross(right, direction);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * speed;}
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        position -= up * deltaTime * speed;}
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        position += up * deltaTime * speed;}
    float FoV = initialFoV;

    ProjectionMatrix = glm::perspective(glm::radians(FoV), 16.0f / 9.0f, 0.1f, 100.0f);
    ViewMatrix = glm::lookAt(
            position,
            position+direction,
            up);
}