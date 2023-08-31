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

const float width = 1920.0f;
const float height = 1080.0f;
const char * vsPath = "VertexShader.glsl";
const char * fsPath = "FragmentShader.glsl";

struct IDList{
    GLuint MatrixID;
    GLuint ModelMatrixID;
    GLuint ViewMatrixID;
    GLuint TextureID;
    GLuint LightID;
    GLuint ProgramID;
};

struct wrappedObject{
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    GLuint VAO{};
    GLuint Texture{};
    glm::mat4 Model = glm::mat4(1.0f);

    GLuint vertexBuffer{};
    GLuint uvBuffer{};
    GLuint normalBuffer{};
    GLuint elementBuffer{};
};

bool createBody(
        // out
        std::map<unsigned short,StellarBody> & planetMap,
        std::map<unsigned short,wrappedObject> & vboMap,

        // in
        const char * objectPath,
        const char * texturePath,

        IDList & idList,

        const std::string& name,
        double mass,
        glm::vec4 position,
        glm::vec4 direction)
{
    unsigned short index = planetMap.size() + 1;
    StellarBody body = StellarBody(name, mass, position, direction, index);
    planetMap.insert(pair<unsigned short,StellarBody>(index, body));

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool loaded = loadOBJ(objectPath, vertices, uvs, normals);
    if (loaded) {
        wrappedObject vbo = *new wrappedObject();

        GLuint texture = loadBMP_custom(texturePath);
        vbo.Texture = texture;

        indexVBO(vertices, uvs, normals, vbo.indices, vbo.vertices, vbo.uvs, vbo.normals);

        glGenVertexArrays(1, &vbo.VAO);
        glBindVertexArray(vbo.VAO);

        glGenBuffers(1, &vbo.vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vbo.vertices.size() * sizeof(glm::vec3), &vbo.vertices[0], GL_STATIC_DRAW);
        glGenBuffers(1, &vbo.uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, vbo.uvs.size() * sizeof(glm::vec2), &vbo.uvs[0], GL_STATIC_DRAW);
        glGenBuffers(1, &vbo.normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, vbo.normals.size() * sizeof(glm::vec3), &vbo.normals[0], GL_STATIC_DRAW);
        glGenBuffers(1, &vbo.elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vbo.indices.size() * sizeof(unsigned int), &vbo.indices[0], GL_STATIC_DRAW);

        vboMap.insert(pair<unsigned short,wrappedObject>(index, vbo));
        return true;
    }
    else {
        return false;
    }
}

void drawObject(
        wrappedObject & vbo,
        IDList & idList,

        glm::mat4 & ProjectionMatrix,
        glm::mat4 & ViewMatrix)
{
    glUseProgram(idList.ProgramID);

    glm::mat4 mvp = ProjectionMatrix * ViewMatrix * vbo.Model;

    glUniformMatrix4fv(idList.MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(idList.ModelMatrixID, 1, GL_FALSE, &vbo.Model[0][0]);
    glUniformMatrix4fv(idList.ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);


    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, vbo.Texture);
    glUniform1i(idList.TextureID, 0);

//    std::cout<<"Texture bound"<<std::endl;

    glBindVertexArray(vbo.VAO);
//    std::cout<<"VAO bound"<<std::endl;
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.vertexBuffer);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.uvBuffer);
    glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            nullptr                          // array buffer offset
    );
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.normalBuffer);
    glVertexAttribPointer(
            2,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.elementBuffer);
    glDrawElements(
            GL_TRIANGLES,
            vbo.indices.size(),
            GL_UNSIGNED_SHORT,
            nullptr
    );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
     * Default Setup
    */

    std::map<unsigned short,StellarBody> planetMap;
    std::map<unsigned short,wrappedObject> vboMap;
    bool res = createBody(planetMap, vboMap, "8kVertPlanet.obj", "planet.bmp", idList, "Planet 1", 10000000.0, glm::vec4(0,0,0,1), glm::vec4(1,0,0,0));
    std::cout<<res<<std::endl;


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 1st attribute buffer : vertices
        glUseProgram(idList.ProgramID);

        glm::vec3 lightPos = glm::vec3(5,5,5);
        glUniform3f(idList.LightID, lightPos.x, lightPos.y, lightPos.z);

        computeMatricesFromInputs(window);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();

        for (auto & it : vboMap) {
            const auto tmp = it.first;
//            std::cout << planetMap[tmp].getName() << std::endl;
            drawObject(it.second, idList, ProjectionMatrix, ViewMatrix);
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