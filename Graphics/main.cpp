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

#include "../Graphics/loadShader.hpp"
#include "../Common/control.hpp"
#include "../Common/loadOBJ.hpp"
#include "../Common/loadDDS.hpp"
#include "../Physics/StellarBody.hpp"
#include "../Graphics/commands.hpp"
#include "../Common/structs.hpp"

GLFWwindow* window;


std::vector<std::pair<uint_fast64_t, glm::vec4>> transformQ;


const float width = 1920.0f;
const float height = 1080.0f;
const char * vsPath = "Graphics/VertexShader.glsl";
const char * fsPath = "Graphics/FragmentShader.glsl";


int main() {

    /*
     * Default setup
    */

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
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    IDList idList;

    glfwPollEvents();
    glfwSetCursorPos(window, width/2, height/2);
    glClearColor(1.0f,1.0f,1.0f,0.3f);
    idList.ProgramID = LoadShaders(vsPath, fsPath);

    // Get MVP handle, only during initialisation
    idList.MatrixID = glGetUniformLocation(idList.ProgramID, "MVP");
    idList.ViewMatrixID = glGetUniformLocation(idList.ProgramID, "V");
    idList.ModelMatrixID = glGetUniformLocation(idList.ProgramID, "M");

    glUseProgram(idList.ProgramID);
    idList.LightID = glGetUniformLocation(idList.ProgramID, "LightPosition_worldspace");

    /*
     * End Default Setup
    */


    std::map<uint_fast64_t,StellarBody> planetMap;
    std::map<uint_fast64_t,wrappedObject> vboMap;
    bool res = createObject(planetMap, vboMap, "highVertTest.obj", "planet.bmp", idList, "Planet 1", 10000000.0,
                            glm::vec4(1, 1, 1, 1), glm::vec4(1, 0, 0, 0));
    std::cout<<"Planet 1 Check : "<<res<<std::endl;
    res = createObject(planetMap, vboMap, "highVertTest.obj", "planet.bmp", idList, "Planet 2", 1000000.0,
                       glm::vec4(0, 3, 0, 1), glm::vec4(1, 0, 0, 0));
    std::cout<<"Planet 2 check : "<<res<<std::endl;


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(idList.ProgramID);

        glm::vec3 lightPos = glm::vec3(5,5,5);
        glUniform3f(idList.LightID, lightPos.x, lightPos.y, lightPos.z);

        computeMatricesFromInputs(window);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();

        for (auto & planetIT : planetMap) { // Apply ind. planet tests here
            if (planetIT.second.getName() == "Planet 2") {
                transformQ.emplace_back(planetIT.second.getIndex(), glm::vec4(0.025, 0.0, 0.0, 0.0));
            }
        }



        auto transformIT = transformQ.begin();
        while (transformIT != transformQ.end()) {
            uint_fast64_t indexToFind = transformIT->first;
            StellarBody * obj = & planetMap.at(indexToFind);
            assert(obj->getIndex() == indexToFind);
            moveObject(vboMap[transformIT->first], idList, transformIT->second);
            transformIT = transformQ.erase(transformIT);
        }

        for (auto & vboIT : vboMap) {
            drawObject(vboIT.second, idList, ProjectionMatrix, ViewMatrix);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while (
            glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    // Cleanup VBO & shader
    // TODO: cleanup VBOs
    glDeleteProgram(idList.ProgramID);
//    glDeleteTextures(1, &Texture);
//    glDeleteVertexArrays(1, &cubeVertexArray);

    // Close OpenGL
    glfwTerminate();
    return 0;
}