//
// Created by comppoter on 8/28/23.
//
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

using namespace glm;

#include "../loadShader.hpp"
#include "../control.hpp"
#include "../loadOBJ.hpp"
#include "../loadDDS.hpp"
#include "../Physics/StellarBody.hpp"

GLFWwindow* window;

float width = 1920.0f;
float height = 1080.0f;

int main() {
    // Collapsable block for default initialization code
    {
        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            return -1;
        }

        glfwWindowHint(GLFW_SAMPLES, 4); // 4x AA
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 2.1
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, "Test", nullptr, nullptr);
        if (window == nullptr) {
            fprintf(stderr, "Failed to open GLFW window\n");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            return -1;
        }
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
        glfwPollEvents();
        glfwSetCursorPos(window, width/2, height/2);
        glClearColor(1.0f,1.0f,1.0f,0.3f);
    }

    std::map<StellarBody,unsigned short> planetMap;
    std::map<
}