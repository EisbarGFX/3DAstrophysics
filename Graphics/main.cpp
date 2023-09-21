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
#include "../Common/commands.hpp"
#include "../Common/control.hpp"
#include "../Common/loadOBJ.hpp"
#include "../Common/loadDDS.hpp"
#include "../Common/structs.hpp"
#include "../Physics/Particle.hpp"
#include "../Physics/Particle.hpp"
#include "../Physics/Cell.hpp"

GLFWwindow* window;


std::vector<std::pair<uint_fast64_t, glm::vec4>> transformQ;


const float width = 1920.0f;
const float height = 1080.0f;
const char * vsPath = "Graphics/VertexShader.glsl";
const char * fsPath = "Graphics/FragmentShader.glsl";

template class std::map<uint_fast64_t, Particle*>;


[[maybe_unused]] auto printvec4 = [](glm::vec4 vec) {
    std::cout<<std::endl;
    std::cout<<"x: "<<vec.x<<std::endl;
    std::cout<<"y: "<<vec.y<<std::endl;
    std::cout<<"z: "<<vec.z<<std::endl;
    std::cout<<"w: "<<vec.w<<std::endl;
};


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


    std::map<uint_fast64_t,Particle*> particleMap;
    std::map<uint_fast64_t,wrappedObject> vboMap;
    std::map<uint_fast64_t,Cell*> cellMap;


    bool res[256];
    res[0] = createSun(particleMap, vboMap, "highVertTest.obj", "planet.bmp", idList, "Sun 1", 10000000.0,
                            glm::vec4(10, 100, 100, 1), glm::vec4(1, 0, 0, 0));
//    std::cout<<"Planet 1 Check : "<<res[0]<<std::endl;
    res[1] = createPlanet(particleMap, vboMap, "highVertTest.obj", "planet.bmp", idList, "Planet 2", 1000000.0,
                       glm::vec4(0, 300, 0, 1), glm::vec4(1, 0, 0, 0));
//    std::cout<<"Planet 2 check : "<<res[1]<<std::endl;


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

        for (auto & particleIT : particleMap) { // Apply ind. particle tests here
            Particle* particle = particleIT.second;
            if (particle->getName() == "Planet 2") {
                transformQ.emplace_back(particle->getIndex(), glm::vec4(0.025, 25, 0.0, 0.0));
            }


            if (!particle->grouped) {
                if (cellMap.empty()) {
                    auto cell = new Cell(particle);
                    cellMap.emplace(0,cell);
                    particle->setCell(0);
                }
                else {
                    int_fast64_t closest = -1;
                    for (auto cmIT : cellMap) {
                        if (glm::distance(cmIT.second->getPosition(),particle->getPPosition())
                        <= Cell::cellWidth && cmIT.second->addParticle(particle)) {
                            if (closest == -1
                            || glm::distance(cmIT.second->getPosition(), particle->getPPosition())
                            < glm::distance(cellMap[closest]->getPosition(), particle->getPPosition())) {
                                closest = cmIT.first;
                            }
                        }
                    }
                    cellMap[closest]->addParticle(particle);
                    particle->setCell(closest);
                }
//                printvec4(cellMap[0]->getPosition());
            }
        }


        auto transformIT = transformQ.begin();
        while (transformIT != transformQ.end()) {
            uint_fast64_t indexToFind = transformIT->first;
            Particle* obj = particleMap.at(indexToFind);
            assert(obj->getIndex() == indexToFind);
            moveObject(vboMap[transformIT->first], obj, idList, transformIT->second);
            auto stor = cellMap[obj->getCell()]->validateCell();
            if(!stor.first) {
                std::cout<<"INVALID"<<std::endl;
                for (auto p : stor.second) {

                    int_fast64_t closest = -1;
                    for (auto cmIT : cellMap) {
                        if (glm::distance(cmIT.second->getPosition(),p->getPPosition())
                            <= Cell::cellWidth && cmIT.second->addParticle(p)) {
                            if (closest == -1
                                || glm::distance(cmIT.second->getPosition(), p->getPPosition())
                                   < glm::distance(cellMap[closest]->getPosition(), p->getPPosition())) {
                                closest = cmIT.first;
                            }
                        }
                    }
                    if (closest == -1) {
                        p->setCell(cellMap.size());
                        cellMap.emplace(cellMap.size(), new Cell(p));
                    }
                    else {
                        cellMap[closest]->addParticle(p);
                        p->setCell(closest);
                    }

                }
            }

            printvec4(cellMap[0]->getPosition());
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